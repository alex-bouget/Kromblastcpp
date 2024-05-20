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

    std::vector<std::string> libraries;
    if (config.contains("plugin_folder"))
    {
        decode_plugins_folder(&libraries, config["plugin_folder"]);
    }
    if (config.contains("libraries"))
    {
        decode_libraries<nlohmann::json>(&libraries, config["libraries"]);
    }
    std::map<std::string, ::Kromblast::Class::kromlib_config_t, std::less<>> plugins_config;
    if (config.contains("plugin_config"))
    {
        for (const auto &conf : config["plugin_config"].items())
        {
            ::Kromblast::Class::kromlib_config_t conf_vector;
            for (const auto &conf_value : conf.value().items())
            {
                conf_vector[conf_value.key()] = conf_value.value();
            }
            plugins_config[conf.key()] = conf_vector;
        }
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
    return Kromblast::Config::create_config(
        window,
        debug,
        Kromblast::Config::create_config_plugins(libraries, plugins_config),
        approved_registry,
        mode,
        host);
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
