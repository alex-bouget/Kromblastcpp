#include "kromblast.hpp"
#include "kromblast_lib_config.hpp"
#include "kromblast_compiler_utils.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <httplib.h>
#include <thread>

#include "kromblast_lib_config.hpp"
#include "json_config.hpp"
#include "kromblast_mode.hpp"

int main() //(const int argc, const char *argv[])
{
    std::string cwd = std::string(getcwd(NULL, 0));
    std::cout << cwd << std::endl;
    if (!std::experimental::filesystem::exists(cwd + "/kromblast.json"))
    {
        std::cout << "kromblast.json not found" << std::endl;
        return 1;
    }
    const Kromblast::Core::ConfigKromblast &config = Kromblast::Config::get_from_json(cwd + "/kromblast.json");

    Kromblast::KromblastMode blast(config);
    Kromblast::Api::WindowInterface *window = blast.get_window();
    blast.run();
    return 0;
}