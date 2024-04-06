#include "json_config.hpp"
#include "config_constructor.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <experimental/filesystem>
#include <iostream>

#include "utils.hpp"

namespace fs = std::experimental::filesystem;

Kromblast::Core::ConfigKromblastWindow create_config_window_json(const nlohmann::json &json_window, bool debug)
{
    if (!json_window.contains("title"))
    {
        std::cout << "Window title not found" << std::endl;
        exit(1);
    }
    std::string title = json_window["title"];
    if (!json_window.contains("size"))
    {
        std::cout << "Window size not found" << std::endl;
        exit(1);
    }
    int width, height = 0;
    if (!json_window["size"].is_array() || json_window["size"].size() != 2)
    {
        std::cout << "Window size must be an array of size 2" << std::endl;
        exit(1);
    }
    width = json_window["size"][0];
    height = json_window["size"][1];
    bool fullscreen = false;
    bool frameless = false;
    if (json_window.contains("fullscreen"))
    {
        fullscreen = json_window["fullscreen"];
    }
    if (json_window.contains("frameless"))
    {
        frameless = json_window["frameless"];
    }
    return Kromblast::Config::create_config_window(title, width, height, fullscreen, frameless, debug);
}


void decode_libraries(std::vector<std::string>* lib_name, const nlohmann::json &json_libraries)
{
    fs::path cwd = fs::current_path();
    for (const auto &lib : json_libraries)
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
            lib_name->push_back(lib);
        }
        else
        {
            fs::path lib_path(cwd.string() + "/" + lib_string);
            if (!fs::exists(lib_path))
            {
                std::cout << "Library not found" << std::endl;
                exit(1);
            }
            lib_name->push_back(lib_path);
        }
    }
}


Kromblast::Core::ConfigKromblast create_config_from_json(const nlohmann::json &config)
{
    bool debug = false;
    if (config.contains("debug"))
    {
        debug = config["debug"];
    }
    if (!config.contains("window"))
    {
        std::cout << "Window not found" << std::endl;
        exit(1);
    }
    Kromblast::Core::ConfigKromblastWindow window = create_config_window_json(config["window"], debug);
    std::string cwd(std::experimental::filesystem::current_path());
    if (!config.contains("plugin_folder") && !config.contains("libraries"))
    {
        std::cout << "Plugin folder or libraries not found" << std::endl;
        exit(1);
    }
    std::vector<std::string> lib_name;
    if (config.contains("libraries"))
    {
        decode_libraries(&lib_name, config["libraries"]);
    }
    if (config.contains("plugin_folder"))
    {
        decode_plugins_folder(&lib_name, config["plugin_folder"]);   
    }
    std::vector<std::string> approved_registry;
    if (config.contains("registry"))
    {
        for (const auto &registry : config["registry"])
        {
            approved_registry.push_back(registry);
        }
    }
    if (!config.contains("mode"))
    {
        std::cout << "Mode not found" << std::endl;
        exit(1);
    }
    Kromblast::Core::Mode mode = get_mode(config["mode"]);
    if (!config.contains("path"))
    {
        std::cout << "Path not found" << std::endl;
        exit(1);
    }
    std::string host = config["path"];
    return Kromblast::Config::create_config(window, debug, lib_name, approved_registry, mode, host);
}


Kromblast::Core::ConfigKromblast Kromblast::Config::get_from_json(const std::string &path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        std::cout << "File not found" << std::endl;
        exit(1);
    }
    nlohmann::json config_json = nlohmann::json::parse(f);
    return create_config_from_json(config_json);
}
