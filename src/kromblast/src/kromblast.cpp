#include "kromblast.hpp"
#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include "load_lib.hpp"
#include "function_call.hpp"
#include "dlfcn.h"
#include <regex>
#include "X11/Xlib.h"

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param config Configuration of the Kromblast object
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(const KromblastCore::ConfigKromblast &config)
{
    kromblast_window = new Window(config);
    dispatcher = new Dispatcher();
    debug = config.debug;
    webview::webview *webview = kromblast_window->get_webview();
    webview->bind(
        "kromblast",
        [&](std::string arg) { // Bind the kromblast function
            return kromblast_callback(arg);
        });
    for (int i = 0; i < (int)config.approved_registry.size(); i++)
    {
        std::string path = config.approved_registry[i];
        std::string old_path = path;

        path = std::regex_replace(path, std::regex("\\/"), "\\/");
        path = std::regex_replace(path, std::regex("\\."), "\\.");
        path = std::regex_replace(path, std::regex("\\*"), ".*");
        path = "^" + path + "$";

        log("Kromblast::Constructor", "regex path: " + old_path + " -> " + path);
        this->approved_registry.push_back(std::regex(path, std::regex_constants::ECMAScript | std::regex_constants::icase));
        log("Kromblast::Constructor", "approved registry: " + config.approved_registry[i]);
    }
    Utils::Library::kromblast_load_library(config.lib_name, this, kromblast_window);
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
    std::string uri = kromblast_window->get_current_url();
    bool find = false;
    log("Kromblast::kromblast_callback", "URI: " + uri);
    for (std::regex exp : approved_registry)
    {
        if (std::regex_match(uri, exp))
        {
            find = true;
            break;
        }
    }
    if (!find)
    {
        log("Kromblast::kromblast_callback", "Request not approved: " + req);
        return "[\"Request not approved\"]";
    }

    log("Kromblast::kromblast_callback", "Request: " + req);

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

void Kromblast::Kromblast::run()
{
    kromblast_window->run();
}

KromblastCore::WindowInterface *Kromblast::Kromblast::get_window() const
{
    return kromblast_window;
}

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

KromblastCore::Signal::Dispatcher *Kromblast::Kromblast::get_dispatcher() const
{
    return dispatcher;
}