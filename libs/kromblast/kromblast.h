#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "./lib/kromblast_library.h"

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
    class Kromblast
    {
        private:
            /**
             * @brief List of the callback functions
            */
            KromblastLib::kromblast_function** kromblast_function_lib;

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
             * @param title Title of the window
             * @param width Width of the window
             * @param height Height of the window
             * @param debug Debug mode
             * @param lib_name List of the libraries
             * @param lib_nb Number of libraries
             * @return Return a new instance of Kromblast
            */
            Kromblast(std::string title, int width, int height, bool debug, std::string lib_name[], int lib_nb);

            /**
             * @brief Destructor
            */
            ~Kromblast();

            /**
             * @brief Function called for each function
             * @param req Request
             * @return Return the result of the function
            */
            std::string kromblast_callback(std::string req);

            /**
             * @brief Set the html of the webview
             * @param html Html
            */
            void set_html(const std::string &html);

            /**
             * @brief Navigate to a url
             * @param url Url
            */
            void navigate(const std::string &url);

            /**
             * @brief Run the webview
            */
            void run();

            /**
             * @brief Get the debug mode
            */
            bool is_debug();
    };
}

#include "kromblast.cc"

#endif