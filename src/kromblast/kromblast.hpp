#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include "kb_lib_kromblast.hpp"
#include "window.hpp"
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
        Window *kromblast_window;

        /**
         * @brief Debug mode
         */
        bool debug;

        /**
         * @brief List of the regex of the approved registry
         */
        std::vector<std::regex> approved_registry;

    public:
        /**
         * @brief Constructor
         * @param config Config
         * @return Return a new instance of Kromblast
         */
        Kromblast(const KromblastCore::ConfigKromblast& config);

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

        void run();

        KromblastCore::WindowInterface *get_window() const;

        /**
         * @brief Claim a function
         * @param callback Callback function
         * @return Return true if the function is claimed
         */
        bool claim_callback(struct KromblastCore::kromblast_callback *callback);
    };
}

#endif