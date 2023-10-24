#ifndef KROMBLAST_PLUGIN_HPP
#define KROMBLAST_PLUGIN_HPP

#include "kromblast_api_plugin.hpp"
#include "kromblast_api.hpp"
#include <map>
#include <string>
#include <list>
#include "kromblast_lib_plugin_callback.hpp"
#include "kromblast_lib_plugin.hpp"
#include "load_lib.hpp"


namespace Kromblast
{

    class Plugin : public Api::PluginInterface
    {
        private:
            std::map<std::string, Core::kromblast_callback_t*> handle_callback_function;
            Utils::Library<Class::kromblast_lib_get_class_t> library_loader;
            Api::KromblastInterface *kromblast;

            void create_js_function(const Core::kromblast_callback_t& function);
        
        public:
            Plugin(Api::KromblastInterface *kromblast);
            ~Plugin();
            bool claim_callback(Core::kromblast_callback_t callback);
            std::string call_function(Core::kromblast_callback_called_t* function_called);
            void start(std::vector<std::string> plugins);
            void stop();
    };

}

#endif