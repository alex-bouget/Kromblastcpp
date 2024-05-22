#include "plugin.hpp"
#include "kromblast_lib_config.hpp"
#include <algorithm>

namespace Kromblast
{
    Plugin::Plugin(Api::KromblastInterface *kromblast)
    {
        this->kromblast = kromblast;
    }

    Plugin::~Plugin()
    {
        // TODO FIX VALGRIND MEMORY LEAK - DELETE approved_registry in handle_callback_function
    }

    
    const Core::kromblast_callback_t *Plugin::get_callback(const std::string &name)
    {
        if (handle_callback_function.contains(name))
        {
            return handle_callback_function[name];
        }
        return nullptr;
    }

    bool Plugin::is_approved_registry(const std::string &name, const std::string &url)
    {
        const Core::kromblast_callback_t *callback = get_callback(name);
        if (callback == nullptr || callback->approved_registry == nullptr)
        {
            return false;
        }
        return std::ranges::any_of(*callback->approved_registry, [&url](const std::regex &exp) {
            return std::regex_match(url, exp);
        });
    }

    bool Plugin::claim_callback(Core::kromblast_callback_t callback)
    {
        if (handle_callback_function.find(callback.name) != handle_callback_function.end())
        {
            return false;
        }

        handle_callback_function[callback.name] = new Core::kromblast_callback_t(callback);
        return true;
    }

    bool Plugin::claim_callback(const std::string &name, int nb_args, const Core::kromblast_callback_f &callback)
    {
        return claim_callback({name, nb_args, callback, nullptr});
    }

    bool Plugin::claim_callback(const std::string &name, int nb_args, const Core::kromblast_callback_f &callback, std::vector<std::regex> *approved_registry)
    {
        return claim_callback({name, nb_args, callback, approved_registry});
    }

    void Plugin::create_js_function(const Core::kromblast_callback_t &function)
    {
        kromblast->get_logger()->log("LibLinker", "Creating function: " + function.name);
        std::string function_name = function.name;
        kromblast->get_logger()->log("LibLinker", "Registering function: " + function_name);
        if (function_name.find(".") != std::string::npos)
        {
            int i = 0;
            while (function_name.find(".", i) != std::string::npos)
            {
                kromblast->get_logger()->log("LibLinker", "Creating namespace: " + function_name.substr(0, function_name.find(".", i)));
                std::string js = "if (window." + function_name.substr(0, function_name.find(".", i)) + " == undefined) { window." + function_name.substr(0, function_name.find(".", i)) + " = {}; }";
                kromblast->get_window()->init_inject(js);
                i = function_name.find(".", i) + 1;
            }
        }
        std::string args = "";
        std::string args_array = "";
        for (int k = 0; k < function.args_nb; k++)
        {
            if (k > 0)
            {
                args += ", ";
                args_array += ", ";
            }
            args += "arg" + std::to_string(k);
            args_array += "String(arg" + std::to_string(k) + ")";
        }
        std::string js = "window." + function_name + " = function(" + args + ") { return kromblast(\"" + function_name + "\", [" + args_array + "] ) }";
        kromblast->get_window()->init_inject(js);
    }

    std::string Plugin::call_function(Core::kromblast_callback_called_t *function_called)
    {
        const Core::kromblast_callback_t *function = get_callback(function_called->name);
        if (function == nullptr)
        {
            kromblast->get_logger()->log("Function", "Function not found");
            return R"({"Error": "Function not found"})";
        }
        if ((std::size_t)function->args_nb != function_called->args.size())
        {
            kromblast->get_logger()->log("Function", "Invalid number of arguments");
            return R"({"Error": "Invalid number of arguments"})";
        }
        return function->callback(function_called);
    }

    void Plugin::start(const std::vector<Core::ConfigKromblastPlugin> &plugins)
    {
        for (const Core::ConfigKromblastPlugin &plugin : plugins)
        {
            Class::kromblast_lib_get_class_t callback = *library_loader.get_lib<Class::kromblast_lib_get_class_t>(plugin.lib_path, "kromblast_lib_get_class", kromblast->get_logger());
            if (callback == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot load plugin: " + plugin.lib_path);
                continue;
            }
            Class::KromLib *lib = callback();
            if (lib == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot callback the plugin: " + plugin.lib_path);
                continue;
            }
            lib->set_kromblast(kromblast, plugin.config);
            lib->load_functions();
        }
        for (auto function : this->handle_callback_function)
        {
            create_js_function(*function.second);
        }
    }

    void Plugin::stop()
    {
    }
}