#ifndef KRBLST_API_H
#define KRBLST_API_H

#include "kromblast_api_logger.hpp"
#include "kromblast_api_dispatcher.hpp"
#include "kromblast_api_window.hpp"
#include "kromblast_api_plugin.hpp"

namespace Kromblast
{
    namespace Api
    {
        class KromblastInterface
        {
        public:
            virtual ~KromblastInterface(){};
            virtual LoggerInterface *get_logger() const = 0;
            virtual WindowInterface *get_window() const = 0;
            virtual DispatcherInterface *get_dispatcher() const = 0;
            virtual PluginInterface *get_plugin() const = 0;

            virtual void log(const std::string lib, const std::string message) = 0;

            virtual const std::string get_version() = 0;
            virtual void run() = 0;
        };
    }
}

#endif