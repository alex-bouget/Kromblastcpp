#include "kromblast.h"
#include "dlfcn.h"
#include <iostream>

void create_js_link(KromblastLib::kromblast_function function, webview::webview w)
{

    std::string function_name = std::string(function.name);
    std::cout << "Registering function: " << function_name << std::endl;
    if (function_name.find(".") != std::string::npos)
    {
        std::cout << "Creating namespace: " << function_name.substr(0, function_name.find(".")) << std::endl;
        std::string js = "if (window." + function_name.substr(0, function_name.find(".")) + " == undefined) { window." + function_name.substr(0, function_name.find(".")) + " = {}; }";
        w.init(js.c_str());
        w.eval(js.c_str());
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
KromblastLib::KromLib **kromblast_load_library(std::string lib_name[], int lib_nb, webview::webview w)
{
    KromblastLib::KromLib **kromblast_lib = new KromblastLib::KromLib *[lib_nb];
    for (int i = 0; i < lib_nb; i++)
    {
        void *handle = dlopen(lib_name[i].c_str(), RTLD_LAZY);
        if (!handle)
        {
            std::cerr << "Cannot open library: " << dlerror() << '\n';
            return nullptr;
        }
        Kromblast::create_lib_t create_lib = (Kromblast::create_lib_t)dlsym(handle, "create_kromblast_lib");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            std::cerr << "Cannot load symbol 'create_lib': " << dlsym_error << '\n';
            dlclose(handle);
            return nullptr;
        }
        kromblast_lib[i] = create_lib();
        int functions_nb = -1;
        KromblastLib::kromblast_function *functions = kromblast_lib[i]->library_callback(&functions_nb);
        for (int j = 0; j < functions_nb; j++)
        {
            create_js_link(functions[j], w);
        }
    }
    return kromblast_lib;
}

Kromblast::Kromblast::Kromblast(std::string title, int width, int height, bool debug, std::string lib_name[], int lib_nb)
{
    kromblast_lib_nb = lib_nb;
    kromblast_window = new webview::webview(debug, nullptr);
    kromblast_window->set_title(title);
    kromblast_window->set_size(width, height, WEBVIEW_HINT_NONE);
    kromblast_window->bind("kromblast", [&](std::string arg)
                           { return kromblast_callback(arg); });
    kromblast_lib = kromblast_load_library(lib_name, lib_nb, *kromblast_window);
}

Kromblast::Kromblast::~Kromblast()
{
    delete kromblast_window;
    for (int i = 0; i < kromblast_lib_nb; i++)
    {
        delete kromblast_lib[i];
    }
    delete[] kromblast_lib;
}

int count_args(std::string args)
{
    int args_nb = 1;
    for (int i = 0; i < (int)args.length(); i++)
    {
        if (args[i] == ',')
        {
            args_nb++;
        }
    }
    return args_nb;
}

char *call_function(struct KromblastLib::kromblast_function_called function_called, KromblastLib::KromLib **kromblast_lib, int kromblast_lib_nb)
{
    for (int i = 0; i < kromblast_lib_nb; i++)
    {
        if (kromblast_lib[i]->has_function(function_called.name))
        {
            char *result = kromblast_lib[i]->call_function(function_called);
            return result;
        }
    }
    return (char *)"{\"Error\": \"Function not found\"}";
}

std::string Kromblast::Kromblast::kromblast_callback(std::string req)
{
    struct KromblastLib::kromblast_function_called function_called;

    std::string function_name = req.substr(2, req.find_first_of(",") - 3);
    function_called.name = new char[function_name.length() + 1];
    strcpy(function_called.name, function_name.c_str());

    std::string args = req.substr(req.find_first_of(",") + 1, req.length() - req.find_first_of(",") - 2);

    if (args.length() <= 2)
    {
        function_called.args_nb = 0;
        function_called.args = nullptr;
        char *result = call_function(function_called, kromblast_lib, kromblast_lib_nb);
        std::string result_str = result;
        return result_str;
    }

    int args_nb = count_args(args);
    std::string *args_data = new std::string[args_nb];
    int step = 0;
    for (int i = 0; i < args_nb; i++)
    {
        int start = args.find_first_of("\"", step);
        int end = args.find_first_of("\"", start + 1);
        args_data[i] = args.substr(start + 1, end - start - 1);
        step = end + 1;
    }
    function_called.args_nb = args_nb;
    function_called.args = new char *[args_nb];
    for (int i = 0; i < args_nb; i++)
    {
        function_called.args[i] = new char[args_data[i].length() + 1];
        strcpy(function_called.args[i], args_data[i].c_str());
    }
    char *result = call_function(function_called, kromblast_lib, kromblast_lib_nb);
    std::string result_str = result;
    return result_str;
}

void Kromblast::Kromblast::set_html(const std::string &html)
{
    std::cout << html << std::endl;
    kromblast_window->set_html(html);
}

void Kromblast::Kromblast::navigate(const std::string &url)
{
    kromblast_window->navigate(url);
}

void Kromblast::Kromblast::run()
{
    kromblast_window->run();
}
