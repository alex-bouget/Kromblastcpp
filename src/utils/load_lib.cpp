#include "load_lib.hpp"
#include <iostream>
#include "dlfcn.h"

namespace Kromblast
{
    namespace Utils
    {
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