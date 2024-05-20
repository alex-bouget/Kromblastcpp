#ifndef KROMBLAST_CONFIG_UTILS_HPP
#define KROMBLAST_CONFIG_UTILS_HPP
#include <vector>
#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <experimental/filesystem>
#include "kromblast_lib_config.hpp"

namespace fs = std::experimental::filesystem;

void decode_plugins_folder(std::vector<std::string>* lib_name, const std::string &plugin_folder);


template<typename T>
void decode_libraries(std::vector<std::string> *decoded_libraries, const T &libraries)
{
    fs::path cwd = fs::current_path();
    for (const auto &lib : libraries)
    {
        std::string lib_string = lib;
        fs::path lib_path(lib_string);
        if (lib_path.is_absolute())
        {
            if (!fs::exists(lib_string))
            {
                std::cout << "Library not found" << std::endl;
                exit(1);
            }
            decoded_libraries->push_back(lib);
        }
        else
        {
            fs::path lib_path2(cwd.string() + "/" + lib_string);
            if (!fs::exists(lib_path2))
            {
                std::cout << "Library not found" << std::endl;
                exit(1);
            }
            decoded_libraries->push_back(lib_path2);
        }
    }
}

Kromblast::Core::Mode get_mode(const std::string &json_mode);

#endif