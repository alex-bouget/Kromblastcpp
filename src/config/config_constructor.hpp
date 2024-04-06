#ifndef KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#define KROMBLAST_CONFIG_CONFIG_CONSTRUCTOR_HPP
#include <string>
#include <vector>
#include "kromblast_lib_config.hpp"

namespace Kromblast
{
    namespace Config {
        ::Kromblast::Core::ConfigKromblast create_config(
            ::Kromblast::Core::ConfigKromblastWindow window,
            bool debug,
            std::vector<std::string> lib_name,
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
    }
}

#endif