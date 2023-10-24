#ifndef KROMBLAST_UTILS_LOAD_LIB_H
#define KROMBLAST_UTILS_LOAD_LIB_H

#include "kromblast_api_logger.hpp"
#include <map>

namespace Kromblast
{
    namespace Utils
    {
        template <typename T>
        class Library
        {
            private:
                std::map<std::string, void *> lib_map;

                void *open_lib_file(const std::string& lib_path, Kromblast::Api::LoggerInterface *logger);
                void *get_function(void *handle, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);
            public:
                /**
                 * @brief Load a library
                 * @param lib_name Name of the library
                 * @param function_name Name of the function
                 * @param logger Logger
                 * @return Pointer to the function
                 */
                T *get_lib(const std::string& lib_path, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);
        };
    }
}

#endif