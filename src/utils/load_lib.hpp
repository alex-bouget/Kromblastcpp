#ifndef KROMBLAST_UTILS_LOAD_LIB_H
#define KROMBLAST_UTILS_LOAD_LIB_H

#include "kb_lib_core.hpp"
#include "kb_lib_kromblast.hpp"
#include "webview.h"
#include <vector>

namespace Kromblast
{
    namespace Utils
    {
        namespace Library
        {
            /**
             * @brief create a link between the javascript and the C++ code
             * @param function Function to link
             * @param w Webview instance
             * @param kromblast Kromblast instance
             */
            void create_js_link(KromblastCore::kromblast_callback function,
                                KromblastCore::WindowInterface *window,
                                KromblastCore::KromblastInterface *kromblast);
            /**
             * @brief Load the libraries
             * @param lib_name List of the libraries name
             * @param kromblast Kromblast interface
             * @param w Webview
             */
            void kromblast_load_library(const std::vector<std::string>& lib_name,
                                        KromblastCore::KromblastInterface *kromblast,
                                        KromblastCore::WindowInterface *window);
        }
    }
}

#endif