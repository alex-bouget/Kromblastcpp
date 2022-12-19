#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "kromblast_library.h"

namespace Kromblast
{
    typedef KromblastLib::KromLib *(*create_lib_t)();

    class Kromblast
    {
        private:
            KromblastLib::KromLib **kromblast_lib;
            int kromblast_lib_nb = -1;
            webview::webview *kromblast_window;
        public:
            Kromblast(std::string title, int width, int height, bool debug, std::string lib_name[], int lib_nb);
            ~Kromblast();
            std::string kromblast_callback(std::string req);
            void set_html(const std::string &html);
            void navigate(const std::string &url);
            void run();
    };
}

#include "kromblast.cc"

#endif