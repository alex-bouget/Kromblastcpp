#ifndef KB_LIB_KROMBLAST_H
#define KB_LIB_KROMBLAST_H

#include "kb_lib_core.hpp"

/**
 * @brief Namespace of the kromblast library
 * @class KromblastInterface Interface of the kromblast library
 */
namespace KromblastCore
{
    /**
     * @brief Interface of the kromblast library
     * @property kromblast_callback Callback of the library
     * @property set_html Set the html of the webview
     * @property navigate Navigate to a url
     * @property run Run the webview
     * @property is_debug Get the debug mode
     */
    class KromblastInterface
    {
    public:
        /**
         * @brief Function called for each function
         * @param req Request
         * @return Return the result of the function
         */
        virtual const char *kromblast_callback(const char *req) = 0;

        /**
         * @brief Set the html of the webview
         * @param html Html
         */
        virtual void set_html(const char *html) = 0;

        /**
         * @brief Navigate to a url
         * @param url Url
         */
        virtual void navigate(const char *url) = 0;

        /**
         * @brief Run the webview
         */
        virtual void run() = 0;

        /**
         * @brief Get the debug mode
         */
        virtual bool is_debug() = 0;

        /**
         * @brief Log a message
         */
        virtual void log(const char *lib, const char *message) = 0;
    };
}
#endif