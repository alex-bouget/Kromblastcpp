#include "kromblast.hpp"
#include <httplib.h>
#include <thread>

namespace Kromblast
{
    class KromblastMode: public ::Kromblast::Kromblast
    {
        private:
            std::thread *server_http_thread;
            httplib::Server *server;
            void run_server();
            void stop_server();
            void navig_to_host();
        public:
            KromblastMode(const Core::ConfigKromblast &config): Kromblast(config) {}
            void run() override;
    };
}