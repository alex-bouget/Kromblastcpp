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
            std::map<std::string, Kromblast::Core::kromblast_callback> handle_callback_function;
            Utils::Library<Class::kromblast_lib_get_class_t> library_loader;
            Api::KromblastInterface *kromblast;
        
        public:
            Plugin();
            ~Plugin();
            bool claim_callback(const std::string& callback_name);
            void start(Api::KromblastInterface *kromblast, std::vector<std::string> plugins);
            void stop();

    }

}

#endif