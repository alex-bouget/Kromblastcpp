#ifndef KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#define KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#include <string>
#include <vector>
#include "kromblast_lib_config.hpp"
#include "kromblast_lib_plugin.hpp"
#include <map>

namespace Kromblast
{
    namespace Config {
        ::Kromblast::Core::ConfigKromblast create_config(
            ::Kromblast::Core::ConfigKromblastWindow window,
            bool debug,
            const std::vector<Kromblast::Core::ConfigKromblastPlugin> &plugins,
            const std::vector<std::string> &approved_registry,
            ::Kromblast::Core::Mode mode,
            std::string host);

        ::Kromblast::Core::ConfigKromblastWindow create_config_window(
            std::string title,
            int width,
            int height,
            bool fullscreen,
            bool frameless,
            bool debug);

        std::vector<::Kromblast::Core::ConfigKromblastPlugin> create_config_plugins(
            std::vector<std::string> libraries,
            const std::map<std::string, ::Kromblast::Class::kromlib_config_t, std::less<>> &plugins_config);
    }
}

#endif