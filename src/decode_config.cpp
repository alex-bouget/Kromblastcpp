#include "kromblast_lib_config.hpp"
#include <nlohmann/json.hpp>

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>

/**
 * @brief Namespace of the kromblast library
 * @class KromblastInterface Interface of the kromblast library
 */
namespace Kromblast
{
    void config_window(Core::ConfigKromblast *config, const nlohmann::basic_json<> &json_window)
    {
        if (!json_window.contains("title"))
        {
            std::cout << "Window title not found" << std::endl;
            exit(1);
        }
        if (!json_window.contains("size"))
        {
            std::cout << "Window size not found" << std::endl;
            exit(1);
        }
        config->title = json_window["title"];
        if (!json_window["size"].is_array() || json_window["size"].size() != 2)
        {
            std::cout << "Window size must be an array of size 2" << std::endl;
            exit(1);
        }
        config->width = json_window["size"][0];
        config->height = json_window["size"][1];
        config->fullscreen = false;
        config->frameless = false;
        if (json_window.contains("fullscreen"))
        {
            config->fullscreen = json_window["fullscreen"];
        }
        if (json_window.contains("frameless"))
        {
            config->frameless = json_window["frameless"];
        }
    }

    void config_root(Core::ConfigKromblast *config, const nlohmann::basic_json<> &json_root)
    {
        if (!json_root.contains("plugin_folder"))
        {
            std::cout << "Plugin folder not found" << std::endl;
            exit(1);
        }
        std::string cwd(std::experimental::filesystem::current_path());
        config->lib_path = json_root["plugin_folder"];
        std::cout << cwd + "/" + config->lib_path + "/" << std::endl;
        const auto path = std::experimental::filesystem::directory_iterator(cwd + "/" + config->lib_path + "/");
        config->lib_name.size = std::distance(path, std::experimental::filesystem::directory_iterator{});
        config->lib_name.list = new std::string[config->lib_name.size];
        int i = 0;
        for (const auto &entry : path)
        {
            std::cout << entry.path() << std::endl;
            config->lib_name.list[i] = entry.path();
            std::cout << config->lib_name.list[i] << std::endl;
            i++;
        }
        config->debug = false;
        if (json_root.contains("debug"))
        {
            config->debug = json_root["debug"];
        }
        if (!json_root.contains("mode"))
        {
            std::cout << "Mode not found" << std::endl;
            exit(1);
        }
        const std::string &mode = json_root["mode"];
        if (mode == "server")
        {
            config->mode = Core::Mode::SERVER;
        }
        else if (mode == "local")
        {
            config->mode = Core::Mode::LOCAL;
        }
        else if (mode == "hosted")
        {
            config->mode = Core::Mode::HOSTED;
        }
#ifdef KROMBLAST_DEBUG
        else if (mode == "debug")
        {
            config->mode = Core::Mode::DBUG;
        }
#endif
        else
        {
            std::cout << "Invalid mode" << std::endl;
            exit(1);
        }
        if (!json_root.contains("path"))
        {
            std::cout << "Path not found" << std::endl;
            exit(1);
        }
        config->host = json_root["path"];
    }

    void config_approved_registry(Core::ConfigKromblast *config, const nlohmann::basic_json<> &json_approved)
    {
        if (!json_approved.is_array())
        {
            std::cout << "Registry must be an array" << std::endl;
            exit(1);
        }
        config->approved_registry.size = json_approved.size();
        config->approved_registry.list = new std::string[config->approved_registry.size];
        for (int i = 0; i < config->approved_registry.size; i++)
        {
            config->approved_registry.list[i] = json_approved[i];
        }
    }

    const Core::ConfigKromblast create_config(const nlohmann::basic_json<> &config_json)
    {
        Core::ConfigKromblast config;
        if (!config_json.contains("window"))
        {
            std::cout << "Window not found" << std::endl;
            exit(1);
        }
        config_window(&config, config_json["window"]);
        config_root(&config, config_json);
        if (config_json.contains("registry"))
        {
            config_approved_registry(&config, config_json["registry"]);
        }
        else
        {
            config.approved_registry.size = 0;
            config.approved_registry.list = nullptr;
        }
        return config;
    }


    const Core::ConfigKromblast create_config_path(std::string path)
    {
        std::ifstream f(path);
        nlohmann::json config_json = nlohmann::json::parse(f);
        std::cout << config_json.dump(4) << std::endl;
        return create_config(config_json);
    }
}