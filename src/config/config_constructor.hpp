#ifndef KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#define KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#include <string>
#include <vector>
#include "kromblast_lib_config.hpp"
#include <map>

namespace Kromblast
{
    namespace Config {
        ::Kromblast::Core::ConfigKromblast create_config(
            ::Kromblast::Core::ConfigKromblastWindow window,
            bool debug,
            std::vector<Kromblast::Core::ConfigKromblastPlugin> plugins,
            std::vector<std::string> approved_registry,
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
            std::map<std::string, std::map<std::string, std::string>> plugins_config);
    }
}

#endif