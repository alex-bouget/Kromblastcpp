#ifndef KB_LIB_KROMBLAST_H
#define KB_LIB_KROMBLAST_H

#include <string>
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
            DEBUG,
            #endif
            SERVER,
            LOCAL,
            HOSTED,
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
            std::string title;
            int width;
            int height;
            bool fullscreen;
            bool frameless;
            bool debug;
            std::string lib_path;
            std::string* lib_name;
            int lib_name_size;
            std::string* approved_registry;
            int approved_registry_size;
            Mode mode;
            std::string host;
        };
    }
}
#endif