#include <vector>
#include <string>
#include <iostream>
#include <experimental/filesystem>
#include "kromblast_lib_config.hpp"

namespace fs = std::experimental::filesystem;

void decode_plugins_folder(std::vector<std::string>* lib_name, const std::string &plugin_folder)
{
    fs::path cwd = fs::current_path();
    const auto path = fs::directory_iterator(cwd.string() + "/" + plugin_folder + "/");
    for (const auto &entry : path)
    {
        lib_name->push_back(entry.path());
    }
}

Kromblast::Core::Mode get_mode(const std::string &json_mode)
{
    if (json_mode == "SERVER")
    {
        return Kromblast::Core::Mode::SERVER;
    }
    else if (json_mode == "LOCAL")
    {
        return Kromblast::Core::Mode::LOCAL;
    }
    else if (json_mode == "HOSTED")
    {
        return Kromblast::Core::Mode::HOSTED;
    }
    #ifdef KROMBLAST_DEBUG
    else if (json_mode == "DEBUG")
    {
        return ::Kromblast::Core::Mode::DBUG;
    }
    #endif
    else
    {
        std::cout << "Mode not found" << std::endl;
        exit(1);
    }
}