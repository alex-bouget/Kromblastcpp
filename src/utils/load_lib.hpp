#ifndef KROMBLAST_UTILS_LOAD_LIB_H
#define KROMBLAST_UTILS_LOAD_LIB_H

#include "kromblast_api_logger.hpp"
#include <map>
#include <iostream>
#include "dlfcn.h"

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
                Library();
                ~Library();
                /**
                 * @brief Load a library
                 * @param lib_name Name of the library
                 * @param function_name Name of the function
                 * @param logger Logger
                 * @return Pointer to the function
                 */
                T *get_lib(const std::string& lib_path, const std::string& function_name, Kromblast::Api::LoggerInterface *logger);
        };


        template <typename T>
        Library<T>::Library()
        {
        }

        template <typename T>
        Library<T>::~Library()
        {
            for (auto &lib : lib_map)
            {
                dlclose(lib.second);
            }
        }

        template <typename T>
        void *Library<T>::open_lib_file(const std::string &lib_path, Kromblast::Api::LoggerInterface *logger)
        {
            logger->log("LibLoader", "Loading library: " + lib_path);
            void *handle = dlopen(lib_path.c_str(), RTLD_LAZY);
            if (!handle)
            {
                std::cerr << "Cannot open library: " << dlerror() << '\n';
                return nullptr;
            }
            lib_map.insert({lib_path, handle});
            return handle;
        }

        template <typename T>
        void *Library<T>::get_function(void *handle, const std::string &function_name, Kromblast::Api::LoggerInterface *logger)
        {
            logger->log("LibLoader", "Loading symbol: " + function_name);
            void *function = dlsym(handle, function_name.c_str());
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                std::cerr << "Cannot load symbol " << function_name << ": " << dlsym_error << '\n';
                return nullptr;
            }
            return function;
        }
        
        template <typename T>
        T *Library<T>::get_lib(const std::string &lib_path, const std::string &function_name, Kromblast::Api::LoggerInterface *logger)
        {

            void *handle = open_lib_file(lib_path, logger);
            if (handle == nullptr)
            {
                return nullptr;
            }
            T* function = (T*)get_function(handle, "kromblast_lib_get_class", logger);
            if (function == nullptr)
            {
                dlclose(handle);
                return nullptr;
            }
            return function;
        }
    }
}

#endif