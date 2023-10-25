#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "kromblast_api.hpp"
#include "kromblast_lib_config.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "dispatcher.hpp"
#include "plugin.hpp"
#include <string>
#include <regex>
#include <vector>
#include <map>

#define KROMBLAST_VERSION "0.1.0"

/**
 * @brief Namespace of kromblast. The application for transforming a web application into a desktop application
 */
namespace Kromblast
{

    /**
     * @brief Class of a kromblast application
     * @method kromblast_callback Function called for each function
     * @method set_html Set the html of the webview
     * @method navigate Navigate to a url
     * @method run Run the webview
     * @property kromblast_lib List of the libraries
     * @property kromblast_lib_nb Number of libraries
     * @property kromblast_window Webview
     */
    class Kromblast : public Api::KromblastInterface
    {
    private:
        Logger *logger;
        Window *window;
        Dispatcher *dispatcher;
        Plugin *plugin;
        const std::string kromblast_callback(const std::string req);

        std::vector<std::regex> approved_registry;

    public:
        /**
         * @brief Constructor
         * @param config Config
         * @return Return a new instance of Kromblast
         */
        Kromblast(const Core::ConfigKromblast &config);

        /**
         * @brief Destructor
         */
        ~Kromblast();

        Api::LoggerInterface *get_logger() const;
        Api::WindowInterface *get_window() const;
        Api::DispatcherInterface *get_dispatcher() const;
        Api::PluginInterface *get_plugin() const;

        void log(const std::string lib, const std::string message);

        /**
         * @brief Get the kromblast version
         * @return Return the kromblast version
         */
        const std::string get_version();

        void run();
    };
}

#endif