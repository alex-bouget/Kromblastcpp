#include "kromblast.hpp"
#include "kromblast_api.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "dispatcher.hpp"
#include "plugin.hpp"
#include "dlfcn.h"
#include <regex>
#include "X11/Xlib.h"

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param config Configuration of the Kromblast object
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(const Core::ConfigKromblast &config)
{
    window = new Window(config);
    dispatcher = new Dispatcher();
    plugin = new Plugin(this);
    logger = new Logger(config.debug);

    webview::webview *webview = window->get_webview();
    webview->bind(
        "kromblast",
        [&](std::string arg) { // Bind the kromblast function
            return kromblast_callback(arg);
        });

    for (std::string path : config.approved_registry)
    {
        const std::string &old_path = path;

        path = std::regex_replace(path, std::regex("\\/"), "\\/");
        path = std::regex_replace(path, std::regex("\\."), "\\.");
        path = std::regex_replace(path, std::regex("\\*"), ".*");
        path = "^" + path + "$";

        log("Kromblast::Constructor", "regex path: " + old_path + " -> " + path);
        this->approved_registry.push_back(std::regex(path, std::regex_constants::ECMAScript | std::regex_constants::icase));
        log("Kromblast::Constructor", "approved registry: " + path);
    }
    plugin->start(config.lib_name);
}

/**
 * @brief Destroy the Kromblast::Kromblast object
 */
Kromblast::Kromblast::~Kromblast()
{
    delete window;
    delete dispatcher;
    delete plugin;
    delete logger;
}

/**
 * @brief Callback of the kromblast function
 * @param req Request
 * @return Return the result of the function
 */
const std::string Kromblast::Kromblast::kromblast_callback(const std::string req)
{
    std::string uri = window->get_current_url();
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

    Core::kromblast_callback_called_t function_called;

    // get the function name
    std::string function_name = req.substr(2, req.find_first_of(",") - 3);
    function_called.name = function_name;

    std::string args = req.substr(req.find_first_of(",") + 1, req.length() - req.find_first_of(",") - 2);

    // if there is no arguments
    if (args.length() <= 2)
    {
        function_called.args = {};
        std::string result = plugin->call_function(&function_called);
        return result;
    }

    // get the arguments
    std::vector<std::string> args_data;
    int step = 0;
    int args_nb = 1;
    for (int i = 0; i < (int)args.length(); i++)
    {
        if (args[i] == ',')
        {
            args_nb++;
        }
    }
    for (int i = 0; i < args_nb; i++)
    {
        int start = args.find_first_of("\"", step);
        int end = args.find_first_of("\"", start + 1);
        args_data.push_back(args.substr(start + 1, end - start - 1));
        step = end + 1;
    }
    function_called.args = args_data;

    // call the function
    std::string result = plugin->call_function(&function_called);
    return result;
}

/**
 * @brief Log a message
 * @param lib Library name
 * @param message Message
 */
void Kromblast::Kromblast::log(const std::string lib, const std::string message)
{
    logger->log(lib, message);
}

/**
 * @brief Get the kromblast version
 * @return Return the kromblast version
 */
const std::string Kromblast::Kromblast::get_version() { return KROMBLAST_VERSION; }

void Kromblast::Kromblast::run()
{
    window->run();
}

Kromblast::Api::LoggerInterface *Kromblast::Kromblast::get_logger() const
{
    return logger;
}

Kromblast::Api::WindowInterface *Kromblast::Kromblast::get_window() const
{
    return window;
}
Kromblast::Api::DispatcherInterface *Kromblast::Kromblast::get_dispatcher() const
{
    return dispatcher;
}
Kromblast::Api::PluginInterface *Kromblast::Kromblast::get_plugin() const
{
    return plugin;
}