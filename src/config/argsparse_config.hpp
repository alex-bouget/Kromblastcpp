#ifndef KROMBLAST_PARSER_CONFIG_HPP
#define KROMBLAST_PARSER_CONFIG_HPP
#include "kromblast_lib_config.hpp"


namespace Kromblast
{
    namespace Config {
        ::Kromblast::Core::ConfigKromblast argsparse(int argc, const char *argv[]);
    }
}

#endif