#ifndef KROMBLAST_WINDOW_HPP
#define KROMBLAST_WINDOW_HPP

#include "kromblast_api_window.hpp"
#include "kromblast_lib_config.hpp"
#include "webview.h"

namespace Kromblast
{
    class Window : public Api::WindowInterface
    {
    protected:
        /**
         * @brief Webview
         */
        webview::webview *kromblast_window;

        const Core::ConfigKromblastWindow *config;

        int width;

        int height;

    public:
        Window(const Core::ConfigKromblastWindow &config);

        ~Window();

        void set_fullscreen(bool fullscreen);

        void set_size(int width, int height);

        void set_frameless(bool frameless);

        webview::webview *get_webview();

        void set_html(const std::string html);

        void navigate(const std::string url);

        void run();

        void init_inject(const std::string js);

        void inject(const std::string js);

        std::string get_current_url();

        std::string get_title();
    };
}

#endif