#ifndef KROMBLAST_WINDOW_HPP
#define KROMBLAST_WINDOW_HPP

#include "load_lib.hpp"
#include "kb_lib_kromblast.hpp"


namespace Kromblast
{
    class Window : public KromblastCore::WindowInterface
    {
    private:
        /**
         * @brief Webview
         */
        webview::webview *kromblast_window;

        const KromblastCore::ConfigKromblast* config;

        int width;

        int height;

    public:
        Window(const KromblastCore::ConfigKromblast &config);

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