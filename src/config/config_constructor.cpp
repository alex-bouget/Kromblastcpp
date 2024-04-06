#include <string>
#include <vector>
#include "config_constructor.hpp"

Kromblast::Core::ConfigKromblast Kromblast::Config::create_config(
    Kromblast::Core::ConfigKromblastWindow window,
    bool debug,
    std::vector<std::string> lib_name,
    std::vector<std::string> approved_registry,
    Kromblast::Core::Mode mode,
    std::string host)
{
    return {
        window,
        debug,
        lib_name,
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