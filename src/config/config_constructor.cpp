#include <string>
#include <vector>
#include "config_constructor.hpp"
#include <experimental/filesystem>
#include <regex>

Kromblast::Core::ConfigKromblast Kromblast::Config::create_config(
    Kromblast::Core::ConfigKromblastWindow window,
    bool debug,
    std::vector<Kromblast::Core::ConfigKromblastPlugin> plugins,
    std::vector<std::string> approved_registry,
    Kromblast::Core::Mode mode,
    std::string host)
{
    return {
        window,
        debug,
        plugins,
        approved_registry,
        mode,
        host};
}

Kromblast::Core::ConfigKromblastWindow Kromblast::Config::create_config_window(
    std::string title,
    int width,
    int height,
    bool fullscreen,
    bool frameless,
    bool debug)
{
    return {
        title,
        width,
        height,
        fullscreen,
        frameless,
        debug};
}

std::vector<::Kromblast::Core::ConfigKromblastPlugin> Kromblast::Config::create_config_plugins(
    std::vector<std::string> libraries,
    std::map<std::string, std::map<std::string, std::string>> plugins_config)
{
    std::vector<Kromblast::Core::ConfigKromblastPlugin> plugins;
    for (const auto &lib : libraries)
    {
        std::string filename = std::experimental::filesystem::path(lib).filename();
        if (std::regex_match(filename, std::regex(".*-\\d+\\.\\d+\\.\\d+(\\.(dll|so,dylib)|$)")))
        {
            filename = filename.substr(0, filename.find_last_of("-"));
        }
        if (plugins_config.contains(filename))
        {
            int priority = (plugins_config[filename].contains("priority") ? std::stoi(plugins_config[filename]["priority"]) : 10);
            plugins.push_back(Kromblast::Core::ConfigKromblastPlugin{
                lib,
                plugins_config[filename],
                priority});
        }
        else
        {
            plugins.push_back(Kromblast::Core::ConfigKromblastPlugin{
                lib,
                {},
                10});
        }
    }
    return plugins;
}