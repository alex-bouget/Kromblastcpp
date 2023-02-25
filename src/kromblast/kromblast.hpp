#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include "kb_lib_kromblast.hpp"
#include <string>
#include <map>

#define KROMBLAST_VERSION "0.1.0"

/**
 * @brief Namespace of kromblast. The application for transforming a web application into a desktop application
 */
namespace Kromblast
{
    /**
     * @brief structure to store the config
     * @param title Window title
     * @param width Window width
     * @param height Window height
     * @param fullscreen Window fullscreen
     * @param frameless Window frameless
     * @param debug Debug mode
     * @param lib_path Path to the libraries
     * @param lib_count Number of libraries
     * @param lib_name List of the libraries
     * @param mode Mode of the libraries
     * @param host Host of the libraries
     * @note The config is loaded from the ini file
     */
    struct ConfigKromblast
    {
        std::string title;
        int width;
        int height;
        bool fullscreen;
        bool frameless;
        bool debug;
        std::string lib_path;
        std::vector<std::string> lib_name;
        int mode;
        std::string host;
    };

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
    class Kromblast : public KromblastCore::KromblastInterface
    {
    private:
        /**
         * @brief List of the callback functions
         */
        std::map<std::string, KromblastCore::kromblast_callback> handle_callback_function;

        /**
         * @brief Webview
         */
        webview::webview *kromblast_window;

        /**
         * @brief Debug mode
         */
        bool debug;

    public:
        /**
         * @brief Constructor
         * @param config Config
         * @return Return a new instance of Kromblast
         */
        Kromblast(ConfigKromblast config);

        /**
         * @brief Destructor
         */
        ~Kromblast();

        /**
         * @brief Function called for each function
         * @param req Request
         * @return Return the result of the function
         */
        const std::string kromblast_callback(const std::string req);

        /**
         * @brief Set the html of the webview
         * @param html Html
         */
        void set_html(const std::string html);

        /**
         * @brief Navigate to a url
         * @param url Url
         */
        void navigate(const std::string url);

        /**
         * @brief Run the webview
         */
        void run();

        /**
         * @brief Get the debug mode
         */
        bool is_debug();

        /**
         * @brief Log a message
         * @param lib Library name
         * @param message Message
         */
        void log(const std::string lib, const std::string message);

        /**
         * @brief Get the list of the callback functions
         * @return Return the list of the callback functions
         */
        std::vector<KromblastCore::kromblast_callback> get_functions();

        /**
         * @brief Get the kromblast version
         * @return Return the kromblast version
         */
        const std::string get_version();

        /**
         * @brief Claim a function
         * @param callback Callback function
         * @return Return true if the function is claimed
         */
        bool claim_callback(struct KromblastCore::kromblast_callback *callback);
    };
}

#endif