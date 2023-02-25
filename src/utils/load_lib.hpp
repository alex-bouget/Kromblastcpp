#ifndef KROMBLAST_UTILS_LOAD_LIB_H
#define KROMBLAST_UTILS_LOAD_LIB_H

#include "kb_lib_core.hpp"
#include "kb_lib_kromblast.hpp"
#include "webview.h"

namespace Kromblast
{
    namespace Utils
    {
        namespace Library
        {
            void create_js_link(KromblastCore::kromblast_callback function, webview::webview w, KromblastCore::KromblastInterface *kromblast);
            KromblastCore::kromblast_callback **kromblast_load_library(int *nb_functions, std::string lib_name[], int lib_nb, KromblastCore::KromblastInterface *kromblast_interface, webview::webview w);
        }
    }
}

#endif