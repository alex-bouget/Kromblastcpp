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
    const std::map<std::string, ::Kromblast::Class::kromlib_config_t, std::less<>> &plugins_config)
{
    std::vector<Kromblast::Core::ConfigKromblastPlugin> plugins;
    for (const auto &lib : libraries)
    {
        std::string filename = std::experimental::filesystem::path(lib).filename();
        if (std::regex_match(filename, std::regex(".*-\\d+\\.\\d+\\.\\d+(\\.(dll|so|dylib)|$)")))
        {
            filename = filename.substr(0, filename.find_last_of("-"));
        } else if (std::regex_match(filename, std::regex(".*\\.(dll|so|dylib)")))
        {
            filename = filename.substr(0, filename.find_last_of("."));
        }
        if (plugins_config.contains(filename))
        {
            int priority = (plugins_config.at(filename).contains("priority") ? std::stoi(plugins_config.at(filename).at("priority")) : 10);
            plugins.emplace_back(Kromblast::Core::ConfigKromblastPlugin{
                lib,
                plugins_config.at(filename),
                priority});
        }
        else
        {
            plugins.emplace_back(Kromblast::Core::ConfigKromblastPlugin{
                lib,
                {},
                10});
        }
    }
    return plugins;
}