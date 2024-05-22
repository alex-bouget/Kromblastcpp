#include "kromblast.hpp"
#include "kromblast_api.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "dispatcher.hpp"
#include "plugin.hpp"
#include "dlfcn.h"
#include <regex>
#include "X11/Xlib.h"
#include <algorithm>

/**
 * @brief Construct a new Kromblast::Kromblast object
 * @param config Configuration of the Kromblast object
 * @return Return the Kromblast object
 */
Kromblast::Kromblast::Kromblast(const Core::ConfigKromblast &config)
{
    this->config = config;
    window = new Window(config.window);
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
    plugin->start(config.plugins);
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


bool Kromblast::Kromblast::url_is_approved(const std::string &url)
{
    return std::ranges::any_of(approved_registry, [&url](const std::regex &exp) {
        return std::regex_match(url, exp);
    });
}

/**
 * @brief Callback of the kromblast function
 * @param req Request
 * @return Return the result of the function
 */
const std::string Kromblast::Kromblast::kromblast_callback(const std::string req)
{
    std::string uri = window->get_current_url();
    log("Kromblast::kromblast_callback", "URI: " + uri);
    log("Kromblast::kromblast_callback", "Request: " + req);

    // req = "function_name",["arg1","arg2",...]
    std::regex request_regex(R"_(\["(.*)",\[(.*)\]\])_");
    std::smatch match;

    if (!std::regex_search(req, match, request_regex))
    {
        log("Kromblast::kromblast_callback", "Request not valid: " + req);
        return "[\"Request not valid\"]";
    }

    Core::kromblast_callback_called_t function_called;
    function_called.name = match[1];

    if (!(url_is_approved(uri) || plugin->is_approved_registry(function_called.name, uri)))
    {
        log("Kromblast::kromblast_callback", "Request not approved: " + req);
        return "[\"Request not approved\"]";
    }

    std::string args = match[2];

    // get the arguments
    std::vector<std::string> args_data;
    while (args.length() > 0)
    {
        int start = args.find_first_of("\"", 0);
        int end = start;
        do {
            end = args.find_first_of("\"", end + 1);
        } while (args.at(end - 1) == '\\');
        args_data.push_back(args.substr(start + 1, end - start - 1));
        args = args.substr(end + 1);
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