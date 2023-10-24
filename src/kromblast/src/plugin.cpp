#include "plugin.hpp"

namespace Kromblast
{
    Plugin::Plugin(Api::KromblastInterface *kromblast)
    {
        this->kromblast = kromblast;
    }
    Plugin::~Plugin()
    {
    }
    bool Plugin::claim_callback(Core::kromblast_callback_t *callback)
    {
        if (handle_callback_function.find(callback->name) != handle_callback_function.end())
        {
            return false;
        }
        handle_callback_function[callback->name] = callback;
    }

    void Plugin::create_js_function(const Core::kromblast_callback_t &function)
    {
        std::string function_name = std::string(function.name);
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


    std::string Plugin::call_function(const Kromblast::Core::kromblast_callback_called_t& function_called)
    {
        if (handle_callback_function.find(function_called.name) != handle_callback_function.end())
            {
                auto func = handle_callback_function[function_called.name];
                return func->callback(&function_called);
            }
            kromblast->get_logger()->log("Function", "Function not found");
            return (char *)"{\"Error\": \"Function not found\"}";
    }

    void Plugin::start(std::vector<std::string> plugins)
    {
        for (std::string plugin : plugins)
        {
            Class::kromblast_lib_get_class_t *callback = library_loader.get_lib(plugin, "kromblast_lib_get_class", kromblast->get_logger());
            if (callback == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot load plugin: " + plugin);
                continue;
            }
            Class::KromLib *lib = callback();
            if (lib == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot callback the plugin: " + plugin);
                continue;
            }
            lib->set_kromblast(kromblast);
            lib->load_functions();
        }
        for (auto function : this->handle_callback_function)
        {
        }
    }

    void Plugin::stop()
    {
    }
}