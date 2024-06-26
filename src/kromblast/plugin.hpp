#ifndef KROMBLAST_PLUGIN_HPP
#define KROMBLAST_PLUGIN_HPP

#include "kromblast_api_plugin.hpp"
#include "kromblast_api.hpp"
#include <map>
#include <string>
#include <list>
#include <memory>
#include "kromblast_lib_plugin_callback.hpp"
#include "kromblast_lib_plugin.hpp"
#include "kromblast_lib_config.hpp"
#include "load_lib.hpp"

namespace Kromblast
{

    class Plugin : public Api::PluginInterface
    {
    private:
        // use pointer because it cause a segfault when the destructor is called
        // why ? I don't know
        std::map<std::string, Core::kromblast_callback_t*, std::less<>> handle_callback_function;
        Utils::Library library_loader;
        Api::KromblastInterface *kromblast;

        void create_js_function(const Core::kromblast_callback_t &function);
        const Core::kromblast_callback_t *get_callback(const std::string &name);

    public:
        Plugin(Api::KromblastInterface *kromblast);
        ~Plugin();
        bool is_approved_registry(const std::string &name, const std::string &url);
        bool claim_callback(const std::string &name, int nb_args, const Core::kromblast_callback_f &callback) override;
        bool claim_callback(const std::string &name, int nb_args, const Core::kromblast_callback_f &callback, const std::vector<std::regex> &approved_registry) override;
        std::string call_function(Core::kromblast_callback_called_t *function_called) override;
        void start(const std::vector<Core::ConfigKromblastPlugin> &plugins) override;
        void stop() override;
    };

}

#endif