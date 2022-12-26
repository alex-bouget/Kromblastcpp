#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "./core/kb_lib_core.h"
#include "./core/kb_lib_kromblast.h"

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
        int lib_count;
        std::string *lib_name;
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
    class Kromblast: public KromblastCore::KromblastInterface
    {
    private:
        /**
         * @brief List of the callback functions
         */
        KromblastCore::kromblast_callback **kromblast_function_lib;

        /**
         * @brief Number of callback functions
         */
        int kromblast_function_nb = -1;

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
        const char *kromblast_callback(const char *req);

        /**
         * @brief Set the html of the webview
         * @param html Html
         */
        void set_html(const char *html);

        /**
         * @brief Navigate to a url
         * @param url Url
         */
        void navigate(const char *url);

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
         * @param lib Library
         * @param message Message
         */
        void log(const char *lib, const char *message);
    };
}

#include "kromblast.cc"

#endif