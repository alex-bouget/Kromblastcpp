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
 * @param debug Debug mode
 */
void Kromblast::Utils::Library::create_js_link(KromblastCore::kromblast_callback function, webview::webview w, bool debug)
{
    std::string function_name = std::string(function.name);
    if (debug)
        std::cout << "Registering function: " << function_name << std::endl;
    if (function_name.find(".") != std::string::npos)
    {
        int i = 0;
        while (function_name.find(".", i) != std::string::npos)
        {
            if (debug)
                std::cout << "Creating namespace: " << function_name.substr(0, function_name.find(".", i)) << std::endl;
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
 * @brief Load the libraries
 * @param nb_functions Number of functions returned
 * @param lib_name List of the libraries name
 * @param lib_nb Number of libraries
 * @param w Webview
 * @param debug Debug mode
 * @return Return the list of the libraries
 */
KromblastCore::kromblast_callback **Kromblast::Utils::Library::kromblast_load_library(int *nb_functions, std::string lib_name[], int lib_nb, KromblastCore::KromblastInterface *kromblast_interface, webview::webview w, bool debug)
{
    // create the list of the number of functions in each library
    int *kromblast_function_nb = new int[lib_nb];
    // create the list of the functions in each library
    KromblastCore::kromblast_callback **kromblast_function_lib = new KromblastCore::kromblast_callback *[lib_nb];
    for (int i = 0; i < lib_nb; i++)
    {
        // open the library
        if (debug)
            std::cout << "Loading library: " << lib_name[i] << std::endl;
        void *handle = dlopen(lib_name[i].c_str(), RTLD_LAZY);
        if (!handle)
        {
            std::cerr << "Cannot open library: " << dlerror() << '\n';
            return nullptr;
        }

        // get the function
        if (debug)
            std::cout << "Loading symbol: kromblast_lib_get_type" << std::endl;
        KromblastCore::kromblast_lib_get_type_t lib_type = (KromblastCore::kromblast_lib_get_type_t)dlsym(handle, "kromblast_lib_get_type");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            std::cerr << "Cannot load symbol 'kromblast_lib_get_type': " << dlsym_error << '\n';
            dlclose(handle);
            return nullptr;
        }
        kromblast_function_nb[i] = -1;
        if (debug)
            std::cout << "Loading functions" << std::endl;
        KromblastCore::KromLibType type = lib_type();
        switch (type)
        {
        case KromblastCore::KromLibType::KB_LIB_CLASS:
        {
            if (debug)
                std::cout << "Loading symbol: kromblast_lib_get_class" << std::endl;
            KromblastCore::Class::kromblast_lib_get_class_t class_callback = (KromblastCore::Class::kromblast_lib_get_class_t)dlsym(handle, "kromblast_lib_get_class");
            dlsym_error = dlerror();
            if (dlsym_error)
            {
                std::cerr << "Cannot load symbol 'kromblast_lib_get_class': " << dlsym_error << '\n';
                dlclose(handle);
                return nullptr;
            }
            KromblastCore::Class::KromLib *kromblast_lib = class_callback();
            kromblast_lib->set_kromblast(kromblast_interface);
            kromblast_function_lib[i] = kromblast_lib->get_function(&kromblast_function_nb[i]); // get the functions
            break;
        }
        case KromblastCore::KromLibType::KB_LIB_STRUCT:
        {
            if (debug)
                std::cout << "Loading symbol: kromblast_lib_get_struct" << std::endl;
            KromblastCore::Struct::kromblast_lib_struct_get_function_t struct_callback = (KromblastCore::Struct::kromblast_lib_struct_get_function_t)dlsym(handle, "kromblast_lib_struct_get_function");
            dlsym_error = dlerror();
            if (dlsym_error)
            {
                std::cerr << "Cannot load symbol 'kromblast_lib_struct_get_function': " << dlsym_error << '\n';
                dlclose(handle);
                return nullptr;
            }
            kromblast_function_lib[i] = (struct_callback)(&kromblast_function_nb[i]); // get the functions
            break;
        }
        default:
        {
            std::cerr << "Unknown library type" << '\n';
            dlclose(handle);
            return nullptr;
        }
        }
    }
    int functions_nb = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        // get the number of all functions
        functions_nb += kromblast_function_nb[i];
    }

    // create the list of the functions
    KromblastCore::kromblast_callback **kromblast_function = new KromblastCore::kromblast_callback *[functions_nb];
    int k = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        for (int j = 0; j < kromblast_function_nb[i]; j++)
        {
            kromblast_function[k] = &kromblast_function_lib[i][j];
            create_js_link(*kromblast_function[k], w, debug);
            k++;
        }
    }

    // return the list of the functions and the number of functions
    *nb_functions = functions_nb;
    return kromblast_function;
}