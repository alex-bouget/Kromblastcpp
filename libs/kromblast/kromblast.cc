#include "kromblast.h"
#include "dlfcn.h"
#include <iostream>
#include "X11/Xlib.h"

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
 * @param nb_functions Number of functions returned
 * @param lib_name List of the libraries name
 * @param lib_nb Number of libraries
 * @param w Webview
 * @param debug Debug mode
 * @return Return the list of the libraries
 */
KromblastLib::kromblast_function **kromblast_load_library(int* nb_functions, std::string lib_name[], int lib_nb, webview::webview w, bool debug)
{
    // create the list of the number of functions in each library
    int *kromblast_function_nb = new int[lib_nb];
    // create the list of the functions in each library
    KromblastLib::kromblast_function **kromblast_function_lib = new KromblastLib::kromblast_function *[lib_nb];
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
            std::cout << "Loading symbol: get_kromblast_callback" << std::endl;
        KromblastLib::get_kromblast_callback_t kromblast_callback = (KromblastLib::get_kromblast_callback_t)dlsym(handle, "get_kromblast_callback");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            std::cerr << "Cannot load symbol 'get_kromblast_callback': " << dlsym_error << '\n';
            dlclose(handle);
            return nullptr;
        }

        // create the library and add the functions to the webview
        if (debug)
            std::cout << "Loading functions" << std::endl;
        kromblast_function_nb[i] = -1;
        kromblast_function_lib[i] = (*kromblast_callback)(&kromblast_function_nb[i]); // get the functions
        for (int j = 0; j < kromblast_function_nb[i]; j++)
        {
            create_js_link(kromblast_function_lib[i][j], w, debug);
        }
    }
    int functions_nb = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        // get the number of all functions
        functions_nb += kromblast_function_nb[i];
    }

    // create the list of the functions
    KromblastLib::kromblast_function **kromblast_function = new KromblastLib::kromblast_function*[functions_nb];
    int k = 0;
    for (int i = 0; i < lib_nb; i++)
    {
        for (int j = 0; j < kromblast_function_nb[i]; j++)
        {
            kromblast_function[k] = &kromblast_function_lib[i][j];
            k++;
        }
    }
    
    // return the list of the functions and the number of functions
    *nb_functions = functions_nb;
    return kromblast_function;
}

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param config Configuration of the Kromblast object
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(ConfigKromblast config)
{
    kromblast_window = new webview::webview(debug, nullptr);
    this->debug = debug;
    debug ? kromblast_window->set_title(config.title + " (debug)") : kromblast_window->set_title(config.title);
    if (config.fullscreen) {
        Display *display = XOpenDisplay(NULL);
        Screen *screen = DefaultScreenOfDisplay(display);
        kromblast_window->set_size(screen->width, screen->height, WEBVIEW_HINT_FIXED);
    } else {
        kromblast_window->set_size(config.width, config.height, WEBVIEW_HINT_NONE);
    }
    if (config.fullscreen || config.frameless) {
        GtkWindow *window = (GtkWindow *)kromblast_window->window();
        gtk_window_set_decorated(GTK_WINDOW(window), false);
        gtk_window_move(GTK_WINDOW(window), 0, 0);
        
    }
    kromblast_window->bind("kromblast", [&](std::string arg) // Bind the kromblast function
                           { return kromblast_callback(arg); });
    this->kromblast_function_lib = kromblast_load_library(&kromblast_function_nb, config.lib_name, config.lib_count, *kromblast_window, debug);
}

/**
 * @brief Destroy the Kromblast::Kromblast object
 */
Kromblast::Kromblast::~Kromblast()
{
    delete kromblast_window;
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
char *call_function(struct KromblastLib::kromblast_function_called function_called, KromblastLib::kromblast_function **kromblast_functions, int kromblast_functions_nb, bool debug)
{
    for (int i = 0; i < kromblast_functions_nb; i++)
    {
        if (strcmp(function_called.name, kromblast_functions[i]->name) == 0 && function_called.args_nb == kromblast_functions[i]->args_nb)
        {
            if (debug)
                std::cout << "Calling function: " << function_called.name << std::endl;
            return kromblast_functions[i]->callback(function_called);
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
        char *result = call_function(function_called, kromblast_function_lib, kromblast_function_nb, debug);
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
    char *result = call_function(function_called, kromblast_function_lib, kromblast_function_nb, debug);
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