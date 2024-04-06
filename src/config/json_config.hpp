#ifndef KROMBLAST_JSON_CONFIG_HPP
#define KROMBLAST_JSON_CONFIG_HPP
#include "kromblast_lib_config.hpp"


namespace Kromblast
{
    namespace Config {
        ::Kromblast::Core::ConfigKromblast get_from_json(const std::string &path);
    }
}

#endif