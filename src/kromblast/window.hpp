#ifndef KROMBLAST_WINDOW_HPP
#define KROMBLAST_WINDOW_HPP

#include "webview.h"
#include "kb_os.hpp"

#if KB_OS_NAME == KB_OS_LINUX
    #include "X11/Xlib.h"
#endif

namespace Kromblast
{
    class Window
    {
        private:
            /**
             * @brief Webview
             */
            webview::webview *kromblast_window;

            

        public:

    }
}

#endif