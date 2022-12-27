#include "load_lib.h"
#include "./../core/kb_lib_class.h"
#include "./../core/kb_lib_core.h"
#include "./../core/kb_lib_struct.h"
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
    kromblast->log("LibLinker", ("Registering function: " + function_name).c_str());
    if (function_name.find(".") != std::string::npos)
    {
        int i = 0;
        while (function_name.find(".", i) != std::string::npos)
        {
            kromblast->log("LibLinker", ("Creating namespace: " + function_name.substr(0, function_name.find(".", i))).c_str());
            std::string js = "if (window." + function_name.substr(0, function_name.find(".", i)) + " == undefined) { window." + function_name.substr(0, function_name.find(".", i)) + " = {}; }";
            w.init(js.c_str());
            w.eval(js.c_str());
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
    w.init(js.c_str());
    w.eval(js.c_str());
}

/**
 * @brief open a library
 * @param lib_name Name of the library
 * @param debug Debug mode
 */
void *open_lib(std::string lib_name, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", ("Loading library: " + lib_name).c_str());
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
void *get_function(void *handle, std::string function_name, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", ("Loading symbol: " + function_name).c_str());
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
 * @param nb_functions Number of functions
 * @param handle Handle of the library
 * @param kromblast_interface Kromblast interface
 */
KromblastCore::kromblast_callback *load_class_lib(int *nb_functions, void *handle, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", "Loading symbol: kromblast_lib_get_class");
    KromblastCore::Class::kromblast_lib_get_class_t class_callback = (KromblastCore::Class::kromblast_lib_get_class_t)get_function(handle, "kromblast_lib_get_class", kromblast_interface);
    if (class_callback == nullptr)
    {
        dlclose(handle);
        return nullptr;
    }
    KromblastCore::Class::KromLib *kromblast_lib = class_callback();
    kromblast_lib->set_kromblast(kromblast_interface);
    return kromblast_lib->get_function(nb_functions);
}

/**
 * @brief Load a structured library
 * @param nb_functions Number of functions
 * @param handle Handle of the library
 * @param kromblast_interface Kromblast interface
 */
KromblastCore::kromblast_callback *load_struct_lib(int *nb_functions, void *handle, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", "Loading symbol: kromblast_lib_get_struct");
    KromblastCore::Struct::kromblast_lib_struct_get_function_t struct_callback = (KromblastCore::Struct::kromblast_lib_struct_get_function_t)get_function(handle, "kromblast_lib_struct_get_function", kromblast_interface);
    if (struct_callback == nullptr)
    {
        dlclose(handle);
        return nullptr;
    }
    return struct_callback(nb_functions);
}

/**
 * @brief Load a library
 * @param nb_functions Number of functions
 * @param lib_name Name of the library
 * @param kromblast_interface Kromblast interface
 */
KromblastCore::kromblast_callback *load_lib(int *nb_functions, std::string lib_name, KromblastCore::KromblastInterface *kromblast_interface)
{
    kromblast_interface->log("LibLoader", ("Loading library: " + lib_name).c_str());
    void *handle = open_lib(lib_name, kromblast_interface);
    if (handle == nullptr)
        return nullptr;

    // get the function
    kromblast_interface->log("LibLoader", "Loading symbol: kromblast_lib_get_type");
    KromblastCore::kromblast_lib_get_type_t lib_type = (KromblastCore::kromblast_lib_get_type_t)get_function(handle, "kromblast_lib_get_type", kromblast_interface);
    if (lib_type == nullptr)
    {
        dlclose(handle);
        return nullptr;
    }
    kromblast_interface->log("LibLoader", "Loading functions");
    KromblastCore::KromLibType type = lib_type();
    KromblastCore::kromblast_callback *kromblast_function_lib = nullptr;
    switch (type)
    {
    case KromblastCore::KromLibType::KB_LIB_CLASS:
    {
        kromblast_function_lib = load_class_lib(nb_functions, handle, kromblast_interface);
        break;
    }
    case KromblastCore::KromLibType::KB_LIB_STRUCT:
    {
        kromblast_function_lib = load_struct_lib(nb_functions, handle, kromblast_interface);
        break;
    }
    default:
    {
        std::cerr << "Unknown library type" << '\n';
        dlclose(handle);
        return nullptr;
    }
    }
    return kromblast_function_lib;
}

/**
 * @brief Load the libraries
 * @param nb_functions Number of functions returned
 * @param lib_name List of the libraries name
 * @param lib_nb Number of libraries
 * @param w Webview
 * @param debug Debug mode
 * @return Return the list of the libraries
 */
KromblastCore::kromblast_callback **Kromblast::Utils::Library::kromblast_load_library(int *nb_functions, std::string lib_name[], int lib_nb, KromblastCore::KromblastInterface *kromblast_interface, webview::webview w)
{
    // create the list of the number of functions in each library
    int *kromblast_function_nb = new int[lib_nb];
    // create the list of the functions in each library
    KromblastCore::kromblast_callback **kromblast_function_lib = new KromblastCore::kromblast_callback *[lib_nb];
    int functions_nb = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        kromblast_function_lib[i] = load_lib(&kromblast_function_nb[i], lib_name[i], kromblast_interface);
        functions_nb += kromblast_function_nb[i];
        // open the library
    }
    for (int i = 0; i < lib_nb; i++)
    {
        // get the number of all functions
    }

    // create the list of the functions
    KromblastCore::kromblast_callback **kromblast_function = new KromblastCore::kromblast_callback *[functions_nb];
    int k = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        for (int j = 0; j < kromblast_function_nb[i]; j++)
        {
            kromblast_function[k] = &kromblast_function_lib[i][j];
            kromblast_interface->log("LibLoader", ("Creating function: " + std::string(kromblast_function[k]->name)).c_str());
            create_js_link(*kromblast_function[k], w, kromblast_interface);
            k++;
        }
    }

    // return the list of the functions and the number of functions
    *nb_functions = functions_nb;
    return kromblast_function;
}