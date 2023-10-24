#include "logger.hpp"
#include <ctime>
#include <iostream>

namespace Kromblast
{
    Logger::Logger(bool debug)
    {
        this->debug = debug;
    }

    Logger::~Logger()
    {
    }

    void Logger::log(const std::string& tag, const std::string& message)
    {
        if (!debug)
            return;
        time_t now = time(nullptr);

        // Convert the time to a tm struct
        tm *timeinfo = localtime(&now);
        // Print the hour, minute, and second
        std::cout << "{" << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << "}";
        std::cout << "[" << tag << "] " << message << std::endl;
        std::cout.flush();
    }
}