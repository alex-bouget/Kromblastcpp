#ifndef KB_LIB_KROMBLAST_H
#define KB_LIB_KROMBLAST_H

#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include "kb_lib_signal.hpp"
#include <string>
#include <functional>
#include <vector>

/**
 * @brief Namespace of the kromblast library
 * @class KromblastInterface Interface of the kromblast library
 */
namespace Kromblast
{
    namespace Core
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
         * @param lib_name List of the libraries
         * @param approved_registry List of the approved path
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
            std::vector<std::string> approved_registry;
            int mode;
            std::string host;
        };
    }
}
#endif