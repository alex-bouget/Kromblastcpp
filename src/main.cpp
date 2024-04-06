#include "kromblast.hpp"
#include "kromblast_lib_config.hpp"
#include "kromblast_compiler_utils.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <httplib.h>
#include <thread>

#include "kromblast_lib_config.hpp"
#include "argsparse_config.hpp"
#include "kromblast_mode.hpp"

int main(const int argc, const char *argv[])
{
    const Kromblast::Core::ConfigKromblast &config = Kromblast::Config::argsparse(argc, argv);

    Kromblast::KromblastMode blast(config);
    blast.run();
    return 0;
}