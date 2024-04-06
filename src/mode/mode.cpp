#include "kromblast_mode.hpp"
#include "kromblast_lib_config.hpp"
#include "experimental/filesystem"
#include "kromblast_compiler_utils.hpp"

void Kromblast::KromblastMode::run_server()
{
    std::string cwd = std::string(getcwd(NULL, 0));
    std::string host = this->config.host;
    if (std::experimental::filesystem::exists(cwd + "/" + this->config.host))
    {
        host = cwd + "/" + this->config.host;
    }
    this->server = new httplib::Server();
    this->server->set_mount_point("/", this->config.host);
    int port = this->server->bind_to_any_port("localhost");
    this->log("MAIN", "Listening on port " + std::to_string(port));
    this->server_http_thread = new std::thread([&]()
                                               { this->server->listen_after_bind(); });
    window->navigate("http://localhost:" + std::to_string(port));
}

void Kromblast::KromblastMode::stop_server()
{
    this->server->stop();
    this->server_http_thread->join();
}

void Kromblast::KromblastMode::navig_to_host()
{
    this->log("MAIN", "Navigating to " + this->config.host);
    if (this->config.mode == ::Kromblast::Core::Mode::SERVER)
    {
        window->navigate(this->config.host.c_str());
    }
    else if (this->config.mode == ::Kromblast::Core::Mode::LOCAL)
    {
       std::string cwd = std::string(getcwd(NULL, 0));
        if (std::experimental::filesystem::exists(cwd + "/" + this->config.host))
        {
            window->navigate("file://" + cwd + "/" + this->config.host);
        }
        else if (std::experimental::filesystem::exists(this->config.host))
        {
            window->navigate("file://" + this->config.host);
        }
        else
        {
            this->log("MAIN", "Host not found");
        }
    }
}

void Kromblast::KromblastMode::run()
{
    switch (this->config.mode)
    {
        #ifdef KROMBLAST_DEBUG
        case ::Kromblast::Core::Mode::DBUG:
            break;
        #endif
        case ::Kromblast::Core::Mode::HOSTED:
            this->run_server();
            Kromblast::run();
            this->stop_server();
            break;
        default:
            this->navig_to_host();
            Kromblast::run();
            break;
    }
}