#include "kromblast.h"
#include "dlfcn.h"
#include <iostream>

/**
 * @brief Create a js link between a function and a function webview
 * @param function Function to link
 * @param w Webview
 * @param debug Debug mode
 */
void create_js_link(KromblastLib::kromblast_function function, webview::webview w, bool debug)
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
 * @param lib_name List of the libraries
 * @param lib_nb Number of libraries
 * @param w Webview
 * @param debug Debug mode
 * @return Return the list of the libraries
 */
KromblastLib::KromLib **kromblast_load_library(std::string lib_name[], int lib_nb, webview::webview w, bool debug)
{
    KromblastLib::KromLib **kromblast_lib = new KromblastLib::KromLib *[lib_nb];
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
        KromblastLib::create_lib_t create_lib = (KromblastLib::create_lib_t)dlsym(handle, "create_kromblast_lib");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            std::cerr << "Cannot load symbol 'create_lib': " << dlsym_error << '\n';
            dlclose(handle);
            return nullptr;
        }

        // create the library and add the functions to the webview
        kromblast_lib[i] = create_lib();
        int functions_nb = -1;
        KromblastLib::kromblast_function *functions = kromblast_lib[i]->library_callback(&functions_nb);
        for (int j = 0; j < functions_nb; j++)
        {
            create_js_link(functions[j], w, debug);
        }
    }
    return kromblast_lib;
}

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param title Title of the window
 * @param width Width of the window
 * @param height Height of the window
 * @param debug Debug mode
 * @param lib_name List of the libraries
 * @param lib_nb Number of libraries
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(std::string title, int width, int height, bool debug, std::string lib_name[], int lib_nb)
{
    kromblast_lib_nb = lib_nb;
    kromblast_window = new webview::webview(debug, nullptr);
    this->debug = debug;
    debug ? kromblast_window->set_title(title + " (debug)") : kromblast_window->set_title(title);
    kromblast_window->set_size(width, height, WEBVIEW_HINT_NONE);
    kromblast_window->bind("kromblast", [&](std::string arg) // Bind the kromblast function
                           { return kromblast_callback(arg); });
    kromblast_lib = kromblast_load_library(lib_name, lib_nb, *kromblast_window, debug);
}

/**
 * @brief Destroy the Kromblast::Kromblast object
 */
Kromblast::Kromblast::~Kromblast()
{
    delete kromblast_window;
    for (int i = 0; i < kromblast_lib_nb; i++)
    {
        delete kromblast_lib[i];
    }
    delete[] kromblast_lib;
}

/**
 * @brief count the number of arguments
 * @param args Arguments
 * @return Return the number of arguments
 */
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

/**
 * @brief call a function in the libraries
 * @param function_called Function called
 * @param kromblast_lib List of the libraries
 * @param kromblast_lib_nb Number of libraries
 * @param debug Debug mode
 * @return Return the result of the function
 */
char *call_function(struct KromblastLib::kromblast_function_called function_called, KromblastLib::KromLib **kromblast_lib, int kromblast_lib_nb, bool debug)
{
    for (int i = 0; i < kromblast_lib_nb; i++)
    {
        struct KromblastLib::kromblast_function this_function = {function_called.name, function_called.args_nb};
        if (kromblast_lib[i]->has_function(this_function))
        {
            if (debug)
                std::cout << "Calling function: " << function_called.name << " from lib " << i << std::endl;
            char *result = kromblast_lib[i]->call_function(function_called);
            return result;
        }
    }
    return (char *)"{\"Error\": \"Function not found\"}";
}

/**
 * @brief Callback of the kromblast function
 * @param req Request
 * @return Return the result of the function
 */
std::string Kromblast::Kromblast::kromblast_callback(std::string req)
{
    struct KromblastLib::kromblast_function_called function_called;

    // get the function name
    std::string function_name = req.substr(2, req.find_first_of(",") - 3);
    function_called.name = new char[function_name.length() + 1];
    strcpy(function_called.name, function_name.c_str());

    std::string args = req.substr(req.find_first_of(",") + 1, req.length() - req.find_first_of(",") - 2);

    // if there is no arguments
    if (args.length() <= 2)
    {
        function_called.args_nb = 0;
        function_called.args = nullptr;
        char *result = call_function(function_called, kromblast_lib, kromblast_lib_nb, debug);
        std::string result_str = result;
        return result_str;
    }

    // get the arguments
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

    // call the function
    char *result = call_function(function_called, kromblast_lib, kromblast_lib_nb, debug);
    std::string result_str = result;
    return result_str;
}

/**
 * @brief Set the html of the window
 * @param html Html
 */
void Kromblast::Kromblast::set_html(const std::string &html) { kromblast_window->set_html(html); }

/**
 * @brief Navigate to an url
 * @param url Url
 */
void Kromblast::Kromblast::navigate(const std::string &url) { kromblast_window->navigate(url); }

/**
 * @brief Run the window
 */
void Kromblast::Kromblast::run() { kromblast_window->run(); }

/**
 * @brief Get the debug mode
*/
bool Kromblast::Kromblast::is_debug() { return debug; }