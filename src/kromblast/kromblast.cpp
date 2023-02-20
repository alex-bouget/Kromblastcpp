#include "kromblast.hpp"
#include "./core/kb_lib_core.h"
#include "./core/kb_lib_struct.h"
#include "./core/kb_lib_class.h"
#include "./utils/load_lib.h"
#include "./utils/function_call.h"
#include "dlfcn.h"
#include <iostream>
#include <ctime>
#include "X11/Xlib.h"

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param config Configuration of the Kromblast object
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(ConfigKromblast config)
{
    if (config.debug)
        log("Kromblast::Constructor", "Starting Kromblast");
    kromblast_window = new webview::webview(config.debug, nullptr);
    this->debug = config.debug;
    debug ? kromblast_window->set_title(config.title + " (debug)") : kromblast_window->set_title(config.title);
    if (config.fullscreen)
    {
        Display *display = XOpenDisplay(NULL);
        Screen *screen = DefaultScreenOfDisplay(display);
        if (debug)
            log("Kromblast::Constructor", ("Set fullscreen at" + std::to_string(screen->width) + "x" + std::to_string(screen->height)).c_str());
        kromblast_window->set_size(screen->width, screen->height, WEBVIEW_HINT_FIXED);
    }
    else
    {
        kromblast_window->set_size(config.width, config.height, WEBVIEW_HINT_NONE);
    }
    if (config.fullscreen || config.frameless)
    {
        if (debug)
            log("Kromblast::Constructor", "Set frameless");
        GtkWindow *window = (GtkWindow *)kromblast_window->window();
        gtk_window_set_decorated(GTK_WINDOW(window), false);
        gtk_window_move(GTK_WINDOW(window), 0, 0);
    }
    kromblast_window->bind("kromblast", [&](std::string arg) // Bind the kromblast function
                           { return (std::string)(kromblast_callback(arg.c_str())); });
    this->kromblast_function_lib = Utils::Library::kromblast_load_library(&kromblast_function_nb, config.lib_name, config.lib_count, this, *kromblast_window);
}

/**
 * @brief Destroy the Kromblast::Kromblast object
 */
Kromblast::Kromblast::~Kromblast()
{
    delete kromblast_window;
}

/**
 * @brief Callback of the kromblast function
 * @param req Request
 * @return Return the result of the function
 */
const char *Kromblast::Kromblast::kromblast_callback(const char *req)
{
    struct KromblastCore::kromblast_callback_called function_called;

    std::string req_str = req;

    // get the function name
    std::string function_name = req_str.substr(2, req_str.find_first_of(",") - 3);
    function_called.name = new char[function_name.length() + 1];
    strcpy(function_called.name, function_name.c_str());

    std::string args = req_str.substr(req_str.find_first_of(",") + 1, req_str.length() - req_str.find_first_of(",") - 2);

    // if there is no arguments
    if (args.length() <= 2)
    {
        function_called.args_nb = 0;
        function_called.args = nullptr;
        char *result = Utils::Function::call_function(function_called, kromblast_function_lib, kromblast_function_nb, this);
        return result;
    }

    // get the arguments
    int args_nb = Utils::Function::count_args(args);
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
    char *result = Utils::Function::call_function(function_called, kromblast_function_lib, kromblast_function_nb, this);
    return result;
}

/**
 * @brief Set the html of the window
 * @param html Html
 */
void Kromblast::Kromblast::set_html(const char *html) { kromblast_window->set_html(html); }

/**
 * @brief Navigate to an url
 * @param url Url
 */
void Kromblast::Kromblast::navigate(const char *url) { kromblast_window->navigate(url); }

/**
 * @brief Run the window
 */
void Kromblast::Kromblast::run() { kromblast_window->run(); }

/**
 * @brief Get the debug mode
 */
bool Kromblast::Kromblast::is_debug() { return debug; }

/**
 * @brief Log a message
 * @param lib Library
 * @param message Message
 */
void Kromblast::Kromblast::log(const char *lib, const char *message)
{
    if (!debug)
        return;
    time_t now = time(nullptr);

    // Convert the time to a tm struct
    tm *timeinfo = localtime(&now);
    // Print the hour, minute, and second
    std::cout << "{" << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << "}";
    std::cout << "[" << lib << "] " << message << std::endl;
}