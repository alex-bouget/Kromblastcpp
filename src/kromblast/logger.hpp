#ifndef KROMBLAST_LOGGER_H
#define KROMBLAST_LOGGER_H

#include "kromblast_api_logger.hpp"

namespace Kromblast
{
    class Logger: Api::LoggerInterface
    {
        private:
            bool debug;

        public:
            Logger(bool debug);
            ~Logger();
            void log(const std::string& tag, const std::string& message);
    }
}

#endif