#include "kromblast.hpp"
#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include "load_lib.hpp"
#include "function_call.hpp"
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
            log("Kromblast::Constructor", "Set fullscreen at" + std::to_string(screen->width) + "x" + std::to_string(screen->height));
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
    kromblast_window->bind(
        "kromblast",
        [&](std::string arg) { // Bind the kromblast function
            return kromblast_callback(arg);
        });
    Utils::Library::kromblast_load_library(config.lib_name, this, *kromblast_window);
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
const std::string Kromblast::Kromblast::kromblast_callback(const std::string req)
{
    struct KromblastCore::kromblast_callback_called function_called;

    // get the function name
    std::string function_name = req.substr(2, req.find_first_of(",") - 3);
    function_called.name = function_name;

    std::string args = req.substr(req.find_first_of(",") + 1, req.length() - req.find_first_of(",") - 2);

    // if there is no arguments
    if (args.length() <= 2)
    {
        function_called.args = {};
        std::string result = Utils::Function::call_function(function_called, handle_callback_function, this);
        return result;
    }

    // get the arguments
    std::vector<std::string> args_data;
    int step = 0;
    int args_nb = Utils::Function::count_args(args);
    for (int i = 0; i < args_nb; i++)
    {
        int start = args.find_first_of("\"", step);
        int end = args.find_first_of("\"", start + 1);
        args_data.push_back(args.substr(start + 1, end - start - 1));
        step = end + 1;
    }
    function_called.args = args_data;

    // call the function
    std::string result = Utils::Function::call_function(function_called, handle_callback_function, this);
    return result;
}

/**
 * @brief Set the html of the window
 * @param html Html
 */
void Kromblast::Kromblast::set_html(const std::string html) { kromblast_window->set_html(html); }

/**
 * @brief Navigate to an url
 * @param url Url
 */
void Kromblast::Kromblast::navigate(const std::string url) { kromblast_window->navigate(url); }

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
 * @param lib Library name
 * @param message Message
 */
void Kromblast::Kromblast::log(const std::string lib, const std::string message)
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

/**
 * @brief Get the list of the callback functions
 * @return Return the list of the callback functions
 */
std::vector<KromblastCore::kromblast_callback> Kromblast::Kromblast::get_functions()
{
    std::vector<KromblastCore::kromblast_callback> functions;
    for (auto it = handle_callback_function.begin(); it != handle_callback_function.end(); ++it)
    {
        functions.push_back(it->second);
    }
    return functions;
}

/**
 * @brief Get the kromblast version
 * @return Return the kromblast version
 */
const std::string Kromblast::Kromblast::get_version() { return KROMBLAST_VERSION; }

/**
 * @brief Claim a function
 * @param callback Callback function
 * @return Return true if the function is claimed
 */
bool Kromblast::Kromblast::claim_callback(struct KromblastCore::kromblast_callback *callback)
{
    if (handle_callback_function.find(callback->name) != handle_callback_function.end())
    {
        return false;
    }
    handle_callback_function[callback->name] = *callback;
    return true;
}