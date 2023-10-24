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
            lib_map.insert(lib_path, handle);
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
        T *Library<T>::get_lib(const std::string &lib_name, const std::string &function_name, Kromblast::Api::LoggerInterface *logger)
        {

            void *handle = open_lib_file(lib_path, logger);
            if (handle == nullptr)
            {
                return;
            }
            T* function = (T*)get_function(handle, "kromblast_lib_get_class", kromblast);
            if (function == nullptr)
            {
                dlclose(handle);
                return;
            }
            return function;
        }
    }
}

/*
/**
 * @brief Create a js link between a function and a function webview
 * @param function Function to link
 * @param w Webview
 * @param kromblast Kromblast interface
 /
void Kromblast::Utils::Library::create_js_link(KromblastCore::kromblast_callback function,
                                               KromblastCore::WindowInterface *window,
                                               KromblastCore::KromblastInterface *kromblast)
{
    std::string function_name = std::string(function.name);
    kromblast->log("LibLinker", "Registering function: " + function_name);
    if (function_name.find(".") != std::string::npos)
    {
        int i = 0;
        while (function_name.find(".", i) != std::string::npos)
        {
            kromblast->log("LibLinker", "Creating namespace: " + function_name.substr(0, function_name.find(".", i)));
            std::string js = "if (window." + function_name.substr(0, function_name.find(".", i)) + " == undefined) { window." + function_name.substr(0, function_name.find(".", i)) + " = {}; }";
            window->inject(js);
            i = function_name.find(".", i) + 1;
        }
    }
    std::string args = "";
    std::string args_array = "";
    for (int k = 0; k < function.args_nb; k++)
    {
        if (k > 0)
        {
            args += ", ";
            args_array += ", ";
        }
        args += "arg" + std::to_string(k);
        args_array += "String(arg" + std::to_string(k) + ")";
    }
    std::string js = "window." + function_name + " = function(" + args + ") { return kromblast(\"" + function_name + "\", [" + args_array + "] ) }";
    window->inject(js);
}
/**
 * @brief Load a library
 * @param lib_name Name of the library
 * @param kromblast Kromblast interface
 /
void load_lib(std::string lib_name, KromblastCore::KromblastInterface *kromblast)
{
    void *handle = open_lib(lib_name, kromblast);
    if (handle == nullptr)
        return;
    KromblastCore::Class::kromblast_lib_get_class_t class_callback = (KromblastCore::Class::kromblast_lib_get_class_t)get_function(handle, "kromblast_lib_get_class", kromblast);
    if (class_callback == nullptr)
    {
        dlclose(handle);
        return;
    }
    KromblastCore::Class::KromLib *kromblast_lib = class_callback();
    kromblast_lib->set_kromblast(kromblast);
    kromblast_lib->load_functions();
}

/**
 * @brief Load the libraries
 * @param lib_name List of the libraries name
 * @param kromblast Kromblast interface
 * @param w Webview
 * @return Return the list of the libraries
 /
void Kromblast::Utils::Library::kromblast_load_library(const std::vector<std::string> &lib_name,
                                                       KromblastCore::KromblastInterface *kromblast,
                                                       KromblastCore::WindowInterface *window)
{
    // create the list of the functions in each library
    for (std::string lib : lib_name)
    {
        load_lib(lib, kromblast);
        // open the library
    }

    for (KromblastCore::kromblast_callback function : kromblast->get_functions())
    {
        kromblast->log("LibLoader", "Creating function: " + std::string(function.name));
        create_js_link(function, window, kromblast);
    }
}
*/