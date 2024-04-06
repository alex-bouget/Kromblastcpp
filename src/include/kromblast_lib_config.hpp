#ifndef KB_LIB_KROMBLAST_H
#define KB_LIB_KROMBLAST_H

#include <string>
#include <vector>
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

        typedef struct ConfigKromblastWindow ConfigKromblastWindow;

        struct ConfigKromblastWindow
        {
            std::string title;
            int width;
            int height;
            bool fullscreen;
            bool frameless;
            bool debug;
        };

        typedef struct ConfigKromblast ConfigKromblast;
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
            std::vector<std::string> lib_name;
            std::vector<std::string> approved_registry;
            Mode mode;
            std::string host;
        };
    }
}
#endif