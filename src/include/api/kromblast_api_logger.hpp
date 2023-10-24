#ifndef KRBLST_API_LOGGER_H
#define KRBLST_API_LOGGER_H

#include <string>

namespace Kromblast
{
    namespace Api
    {
        class LoggerInterface
        {
            public:
                virtual ~LoggerInterface(){};
                virtual void log(const std::string& tag, const std::string& message) = 0;
        };
    }
}

#endif