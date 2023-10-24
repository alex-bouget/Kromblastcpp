#ifndef KROMBLAST_WINDOW_HPP
#define KROMBLAST_WINDOW_HPP

#include "kromblast_api_window.hpp"
#include "webview/webview.h"


namespace Kromblast
{
    class Window : public Kromblast::Api::WindowInterface
    {
    private:
        /**
         * @brief Webview
         */
        webview::webview *kromblast_window;

        const Kromblast::Core::ConfigKromblast* config;

        int width;

        int height;

    public:
        Window(const Kromblast::Core::ConfigKromblast &config);

        ~Window();

        void set_fullscreen(bool fullscreen);

        void set_size(int width, int height);

        void set_frameless(bool frameless);

        webview::webview *get_webview();

        void set_html(const std::string html);

        void navigate(const std::string url);

        std::string get_current_url();

        void run();

        void inject(const std::string js);
    };
}

#endif