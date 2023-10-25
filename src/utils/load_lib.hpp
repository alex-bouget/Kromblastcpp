#ifndef KROMBLAST_UTILS_LOAD_LIB_H
#define KROMBLAST_UTILS_LOAD_LIB_H

#include "kromblast_api_logger.hpp"
#include <list>
#include <iostream>
#include "dlfcn.h"

namespace Kromblast
{
    namespace Utils
    {
        class Library
        {
            private:
                std::list<void *> libs;

                void *open_lib_file(const std::string& lib_path, Kromblast::Api::LoggerInterface *logger);
                void *get_function(void *handle, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);
            public:
                Library();
                ~Library();
                /**
                 * @brief Load a library
                 * @param lib_name Name of the library
                 * @param function_name Name of the function
                 * @param logger Logger
                 * @return Pointer to the function
                 */
                void* get_lib(const std::string& lib_path, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);

                template <typename T>
                T get_lib(const std::string& lib_path, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);
        };
        
        template <typename T>
        T Library::get_lib<T>(const std::string &lib_path, const std::string &function_name, Kromblast::Api::LoggerInterface *logger)
        {
            T function = (T)get_lib(lib_path, function_name, logger);
            return function;
        }
    }
}

#endif