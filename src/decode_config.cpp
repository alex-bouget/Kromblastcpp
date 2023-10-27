#include "kromblast_lib_config.hpp"
#include <nlohmann/json.hpp>

#include <string>
#include <experimental/filesystem>

/**
 * @brief Namespace of the kromblast library
 * @class KromblastInterface Interface of the kromblast library
 */
namespace Kromblast
{
    void decode_window(Core::ConfigKromblast* config, const nlohmann::basic_json<>& json_window)
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
        if (json_window.contains("fullscreen")) {
            config->fullscreen = json_window["fullscreen"];
        }
        if (json_window.contains("frameless")) {
            config->frameless = json_window["frameless"];
        }
    }

    void config_root(Core::ConfigKromblast* config, const nlohmann::basic_json<>& json_root)
    {
        if (!json_root.contains("plugin_folder"))
        {
            std::cout << "Plugin folder not found" << std::endl;
            exit(1);
        }
        config->lib_path = json_root["plugin_folder"];
        const auto path = std::experimental::filesystem::directory_iterator(config->lib_path);
        config->lib_name_size = std::distance(path, std::experimental::filesystem::directory_iterator{});
        config->lib_name = new std::string[config->lib_name_size];
        int i = 0;
        for (const auto &entry : path)
        {
            config->lib_name[i] = entry.path();
            i++;
        }


    }

    const Core::ConfigKromblast &create_config(const nlohmann::basic_json<>& config_json)
    {
        Core::ConfigKromblast config;
        if (!config_json.contains("window"))
        {
            std::cout << "Window not found" << std::endl;
            exit(1);
        }
        decode_window(&config, config_json["window"]);
        decode_root(&config, config_json);
    }
}