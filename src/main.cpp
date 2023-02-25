#include "kromblast.hpp"
#include <iostream>
#include <experimental/filesystem>
#include "Mini.h"

/**
 * @brief Load the config from the ini file
 * @param path Path to the ini file
 * @return Return the config
 */
struct Kromblast::ConfigKromblast load_config(std::string path)
{
    mINI::INIFile file(path);
    mINI::INIStructure ini;
    file.read(ini);
    // Test all errors cases and exit if an error is found
    if (!ini.has("Window"))
    {
        std::cout << "Window section not found" << std::endl;
        exit(1);
    }
    if (!ini.has("Kromblast"))
    {
        std::cout << "Kromblast section not found" << std::endl;
        exit(1);
    }
    if (!ini["Window"].has("title"))
    {
        std::cout << "Window title not found" << std::endl;
        exit(1);
    }
    if (!ini["Window"].has("width"))
    {
        std::cout << "Window width not found" << std::endl;
        exit(1);
    }
    if (!ini["Window"].has("height"))
    {
        std::cout << "Window height not found" << std::endl;
        exit(1);
    }
    if (!ini["Kromblast"].has("lib"))
    {
        std::cout << "Library path not found" << std::endl;
        exit(1);
    }
    if (!ini["Kromblast"].has("path"))
    {
        std::cout << "Host path not found" << std::endl;
        exit(1);
    }
    if (!ini["Kromblast"].has("mode"))
    {
        std::cout << "Mode not found" << std::endl;
        exit(1);
    }
    std::vector<std::string> approved_registry;
    if (ini.has("Registry")) {
        for (const auto &entry : ini["Registry"]) {
            approved_registry.push_back(entry.second);
        }
    }
    std::string title = ini["Window"]["title"].substr(1, ini["Window"]["title"].length() - 2);
    int width = std::stoi(ini["Window"]["width"], nullptr, 10);
    int height = std::stoi(ini["Window"]["height"], nullptr, 10);
    if (width == 0 || height == 0)
    {
        std::cout << "Invalid window size" << std::endl;
        exit(1);
    }

    bool fullscreen = (ini["Window"].has("fullscreen") ? std::strcmp(ini["Window"]["fullscreen"].c_str(), "true") == 0 : false);
    bool frameless = (ini["Window"].has("frameless") ? std::strcmp(ini["Window"]["frameless"].c_str(), "true") == 0 : false);
    bool debug = (ini["Kromblast"].has("debug") ? std::strcmp(ini["Kromblast"]["debug"].c_str(), "true") == 0 : false);

    std::string lib_path = ini["Kromblast"]["lib"].substr(1, ini["Kromblast"]["lib"].length() - 2);
    std::string cwd = std::string(getcwd(NULL, 0));

    std::vector<std::string> lib_name;
    int i = 0;
    for (const auto &entry : std::experimental::filesystem::directory_iterator(cwd + "/" + lib_path + "/"))
    {
        std::string name = entry.path();
        lib_name.insert(lib_name.begin() + i, name);
        i++;
    }
    std::string mode = "server";
    if (ini["Kromblast"].has("mode"))
    {
        mode = ini["Kromblast"]["mode"].substr(1, ini["Kromblast"]["mode"].length() - 2);
    }
    int mode_id = -1;
    if (mode == "server")
    {
        mode_id = 0;
    }
    else if (mode == "local")
    {
        mode_id = 1;
    }
    else if (mode == "hosted")
    {
        mode_id = 2;
    }
    else
    {
        std::cout << "Invalid mode" << std::endl;
        exit(1);
    }
    std::string host = ini["Kromblast"]["path"].substr(1, ini["Kromblast"]["path"].length() - 2);

    if (debug)
    {
        std::cout << "------------------------------" << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Fullscreen: " << fullscreen << std::endl;
        std::cout << "Frameless: " << frameless << std::endl;
        std::cout << "Debug: " << debug << std::endl;
        std::cout << "Library Path: " << lib_path << std::endl;
        std::cout << "Libraries: " << lib_name.size() << std::endl;
        for (int i = 0; i < (int)lib_name.size(); i++)
        {
            std::cout << "    Library " << i << ": " << lib_name.at(i) << std::endl;
        }
        std::cout << "Registry: " << approved_registry.size() << std::endl;
        for (int i = 0; i < (int)approved_registry.size(); i++)
        {
            std::cout << "    Registry " << i << ": " << approved_registry.at(i) << std::endl;
        }
        std::cout << "Mode: " << mode << std::endl;
        std::cout << "Mode ID: " << mode_id << std::endl;
        std::cout << "Host: " << host << std::endl;
        std::cout << "------------------------------" << std::endl << std::endl;
    }
    struct Kromblast::ConfigKromblast result = {
        title,
        width,
        height,
        fullscreen,
        frameless,
        debug,
        lib_path,
        lib_name,
        approved_registry,
        mode_id,
        host};
    return result;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow)
{
#else
int main()
{
#endif
    std::string cwd = std::string(getcwd(NULL, 0));
    std::cout << cwd << std::endl;
    if (!std::experimental::filesystem::exists(cwd + "/kromblast.ini"))
    {
        std::cout << "kromblast.ini not found" << std::endl;
        return 1;
    }
    struct Kromblast::ConfigKromblast config = load_config(cwd + "/kromblast.ini");

    Kromblast::Kromblast blast(config);

    switch (config.mode)
    {
    case 0: // Server
        blast.navigate(config.host.c_str());
        break;
    case 1: // Local
        if (std::experimental::filesystem::exists(cwd + "/" + config.host))
        {
            blast.navigate(("file://" + cwd + "/" + config.host).c_str());
            break;
        }
        if (std::experimental::filesystem::exists(config.host))
        {
            blast.navigate(("file://" + config.host).c_str());
            break;
        }
        std::cout << "Host not found" << std::endl;
        return 1;
    case 2: // Hosted
        break;
    }
    blast.run();
    return 0;
}