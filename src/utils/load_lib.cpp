#include "load_lib.hpp"
#include "kb_lib_class.hpp"
#include "kb_lib_core.hpp"
#include <iostream>
#include "dlfcn.h"

/**
 * @brief Create a js link between a function and a function webview
 * @param function Function to link
 * @param w Webview
 * @param kromblast Kromblast interface
 */
void Kromblast::Utils::Library::create_js_link(KromblastCore::kromblast_callback function, webview::webview w, KromblastCore::KromblastInterface *kromblast)
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
            w.init(js);
            w.eval(js);
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
    w.init(js);
    w.eval(js);
}

/**
 * @brief open a library
 * @param lib_name Name of the library
 * @param debug Debug mode
 */
void *open_lib(std::string lib_name, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", "Loading library: " + lib_name);
    void *handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!handle)
    {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return nullptr;
    }
    return handle;
}

/**
 * @brief Get a function from a library
 * @param handle Handle of the library
 * @param function_name Name of the function
 * @param kromblast_interface Kromblast interface
 * @return Function pointer
 */
void *get_function(void *handle, std::string function_name, KromblastCore::KromblastInterface *kromblast)
{
    kromblast->log("LibLoader", "Loading symbol: " + function_name);
    void *function = dlsym(handle, function_name.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        std::cerr << "Cannot load symbol " << function_name << ": " << dlsym_error << '\n';
        return nullptr;
    }
    return function;
}

/**
 * @brief Load a class library
 * @param handle Handle of the library
 * @param kromblast_interface Kromblast interface
 */
void load_class_lib(void *handle, KromblastCore::KromblastInterface *kromblast)
{
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
 * @brief Load a library
 * @param lib_name Name of the library
 * @param kromblast Kromblast interface
 */
void load_lib(std::string lib_name, KromblastCore::KromblastInterface *kromblast)
{
    void *handle = open_lib(lib_name, kromblast);
    if (handle == nullptr)
        return;
    // get the function

    load_class_lib(handle, kromblast);
}

/**
 * @brief Load the libraries
 * @param lib_name List of the libraries name
 * @param kromblast Kromblast interface
 * @param w Webview
 * @return Return the list of the libraries
 */
void Kromblast::Utils::Library::kromblast_load_library(std::vector<std::string> lib_name, KromblastCore::KromblastInterface *kromblast, webview::webview w)
{
    // create the list of the functions in each library
    for (std::string lib: lib_name)
    {
        load_lib(lib, kromblast);
        // open the library
    }

    for (KromblastCore::kromblast_callback function: kromblast->get_functions())
    {
        kromblast->log("LibLoader", "Creating function: " + std::string(function.name));
        create_js_link(function, w, kromblast);
    }
}