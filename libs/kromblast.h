#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "krom_lib.h"
#include <dlfcn.h>
#include <iostream>

KromblastLib::KromLib **kromblast_lib;
int kromblast_lib_nb = -1;
webview::webview *kromblast_window;

std::string kromblast_callback(std::string req)
{
    std::cout << req << std::endl;
    std::cout << req.length() << std::endl;
    struct KromblastLib::kromblast_function_called function_called;
    std::string function_name = req.substr(2, req.find_first_of(",") - 3);
    function_called.name = new char[function_name.length() + 1];
    strcpy(function_called.name, function_name.c_str());
    function_called.name = (char *)function_name.c_str();
    std::string args = req.substr(req.find_first_of(",") + 1, req.length() - req.find_first_of(",") - 2);
    std::cout << "Function called: " << function_called.name << std::endl;
    std::cout << "Args: " << args << std::endl;
    if (args.length() <= 2)
    {
        char *result = kromblast_lib[0]->call_function(function_called.name, nullptr);
        std::string result_str = result;
        return result_str;
    }
    int args_nb = 0;
    std::string *args_data = new std::string[0];
    args_nb = 1;
    for (int i = 0; i < args.length(); i++)
    {
        if (args[i] == ',')
        {
            args_nb++;
        }
    }
    args_data = new std::string[args_nb];
    std::cout << "Args nb: " << args_nb << std::endl;
    int step = 0;
    for (int i = 0; i < args_nb; i++)
    {
        args_data[i] = args.substr(args.find_first_of("\"", step) + 1, args.find_first_of("\"", step + 1) - args.find_first_of("\"", step) - 1);
    }
}

typedef KromblastLib::KromLib *(*create_lib_t)();

KromblastLib::KromLib *get_lib(std::string lib_name)
{
    void *handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!handle)
    {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return nullptr;
    }
    create_lib_t create_lib = (create_lib_t)dlsym(handle, "create_kromblast_lib");
    if (!create_lib)
    {
        std::cerr << "Cannot load symbol 'create_lib': " << dlerror() << '\n';
        return nullptr;
    }
    return create_lib();
}

webview::webview create_kromblast_window(std::string title, int width, int height, bool debug, std::string lib_name[], int lib_nb)
{
    webview::webview w(debug, nullptr);
    kromblast_window = &w;
    w.set_title(title);
    w.set_size(width, height, WEBVIEW_HINT_NONE);
    w.bind("kromblast", &kromblast_callback);
    kromblast_lib = new KromblastLib::KromLib *[lib_nb];
    for (int i = 0; i < lib_nb; i++)
    {
        kromblast_lib[i] = get_lib(lib_name[i]);
        int nb_function = -1;
        KromblastLib::kromblast_function *functions = kromblast_lib[i]->library_callback(&nb_function);
        for (int j = 0; j < nb_function; j++)
        {
            std::string function_name = std::string(functions[j].name);
            std::cout << "Registering function: " << function_name << std::endl;
            if (function_name.find(".") != std::string::npos)
            {
                std::cout << "Creating namespace: " << function_name.substr(0, function_name.find(".")) << std::endl;
                std::string js = "if (window." + function_name.substr(0, function_name.find(".")) + " == undefined) { window." + function_name.substr(0, function_name.find(".")) + " = {}; }";
                w.init(js.c_str());
                w.eval(js.c_str());
            }
            std::string args = "";
            for (int k = 0; k < functions[j].args_nb; k++)
            {
                if (k > 0)
                {
                    args += ", ";
                }
                args += "arg" + std::to_string(k);
            }

            std::string js = "window." + function_name + " = function(" + args + ") { return kromblast(\"" + function_name + "\", [" + args + "] ) }";
            w.init(js.c_str());
            w.eval(js.c_str());
        }
    }
    kromblast_lib_nb = lib_nb;
    return w;
}

#endif