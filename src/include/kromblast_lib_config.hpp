#ifndef KB_LIB_KROMBLAST_H
#define KB_LIB_KROMBLAST_H

#include <string>
#include <vector>
#include <map>
#include "kromblast_compiler_utils.hpp"

/**
 * @brief Namespace of the kromblast library
 * @class KromblastInterface Interface of the kromblast library
 */
namespace Kromblast
{
    namespace Core
    {

        enum Mode
        {
            #ifdef KROMBLAST_DEBUG
            DBUG,
            #endif
            SERVER,
            LOCAL,
            HOSTED,
        };

        using ConfigKromblastWindow = struct ConfigKromblastWindow;

        struct ConfigKromblastWindow
        {
            std::string title;
            int width;
            int height;
            bool fullscreen;
            bool frameless;
            bool debug;
        };

        using ConfigKromblastPlugin = struct ConfigKromblastPlugin;

        struct ConfigKromblastPlugin
        {
            std::string lib_path;
            std::map<std::string, std::string, std::less<>> config;
            int priority; // 0 is the highest priority, -1 is the lowest, 10 is the default
        };

        using ConfigKromblast = struct ConfigKromblast;
        /**
         * @brief structure to store the config
         * @param title Window title
         * @param width Window width
         * @param height Window height
         * @param fullscreen Window fullscreen
         * @param frameless Window frameless
         * @param debug Debug mode
         * @param lib_path Path to the libraries
         * @param lib_name List of the libraries
         * @param approved_registry List of the approved path
         * @param mode Mode of the libraries
         * @param host Host of the libraries
         * @note The config is loaded from the ini file
         */
        struct ConfigKromblast
        {
            ConfigKromblastWindow window;
            bool debug;
            std::vector<ConfigKromblastPlugin> plugins;
            std::vector<std::string> approved_registry;
            Mode mode;
            std::string host;
        };
    }
}
#endif