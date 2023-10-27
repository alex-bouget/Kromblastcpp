#include "kromblast.hpp"
#include "kromblast_lib_config.hpp"
#include "kromblast_compiler_utils.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <httplib.h>
#include <thread>

#include "decode_config.cpp"

int main() //(const int argc, const char *argv[])
{
    std::string cwd = std::string(getcwd(NULL, 0));
    std::cout << cwd << std::endl;
    if (!std::experimental::filesystem::exists(cwd + "/kromblast.json"))
    {
        std::cout << "kromblast.json not found" << std::endl;
        return 1;
    }
    const Kromblast::Core::ConfigKromblast &config = Kromblast::create_config_path(cwd + "/kromblast.json");

    Kromblast::Kromblast blast(config);
    Kromblast::Api::WindowInterface *window = blast.get_window();

    switch (config.mode)
    {
    case Kromblast::Core::Mode::SERVER: // Server
        blast.log("MAIN", "Server mode");
        blast.log("MAIN", "Navigating to " + config.host);
        window->navigate(config.host.c_str());
        break;
    case Kromblast::Core::Mode::LOCAL: // Local
        blast.log("MAIN", "Local mode");
        if (std::experimental::filesystem::exists(cwd + "/" + config.host))
        {
            window->navigate("file://" + cwd + "/" + config.host);
            break;
        }
        if (std::experimental::filesystem::exists(config.host))
        {
            window->navigate("file://" + config.host);
            break;
        }
        std::cout << "Host not found" << std::endl;
        return 1;
#ifdef KROMBLAST_DEBUG
    case Kromblast::Core::Mode::DBUG: // Debug
        blast.log("MAIN", "Debug mode");
        return 0;
#endif
    case Kromblast::Core::Mode::HOSTED: // Hosted
        blast.log("MAIN", "Hosted mode");
        std::string host = config.host;
        if (std::experimental::filesystem::exists(cwd + "/" + config.host))
        {
            host = cwd + "/" + config.host;
        }
        httplib::Server srv;
        srv.set_mount_point("/", config.host);
        int port = srv.bind_to_any_port("localhost");
        blast.log("MAIN", "Listening on port " + std::to_string(port));
        std::thread server_thread([&srv]()
                                  { srv.listen_after_bind(); });
        window->navigate("http://localhost:" + std::to_string(port));
        blast.run();
        srv.stop();
        server_thread.join();
        return 0;
    }
    blast.run();
    return 0;
}