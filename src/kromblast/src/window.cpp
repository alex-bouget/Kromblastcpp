#include "window.hpp"
#include "kb_os.hpp"

#if KB_OS_FAMILY == KB_OS_LINUX
#include "X11/Xlib.h"
#endif

Kromblast::Window::Window(const Core::ConfigKromblast &config)
{
    this->config = &config;

    kromblast_window = new webview::webview(config.debug, nullptr);
    kromblast_window->set_title(config.title + (config.debug ? " (debug)" : ""));
    if (config.fullscreen)
    {
        this->set_fullscreen(true);
    }
    else
    {
        this->set_size(config.width, config.height);
    }
    if (config.frameless)
    {
        this->set_frameless(true);
    }
}

Kromblast::Window::~Window()
{
    delete kromblast_window;
}

void Kromblast::Window::set_fullscreen(bool fullscreen)
{
#if KB_OS_FAMILY == KB_OS_LINUX

    if (!fullscreen)
    {
        set_frameless(false);
        set_size(config->width, config->height);
        return;
    }

    Display *display = XOpenDisplay(NULL);
    Screen *screen = DefaultScreenOfDisplay(display);

    set_size(screen->width, screen->height);

    set_frameless(true);
#endif
}

void Kromblast::Window::set_size(int width, int height)
{
    kromblast_window->set_size(width, height, WEBVIEW_HINT_NONE);
}

void Kromblast::Window::set_frameless(bool frameless)
{
#if KB_OS_FAMILY == KB_OS_LINUX
    if (!frameless)
    {
        GtkWindow *window = (GtkWindow *)kromblast_window->window();
        gtk_window_set_decorated(GTK_WINDOW(window), true);
        return;
    }
    GtkWindow *window = (GtkWindow *)kromblast_window->window();
    gtk_window_set_decorated(GTK_WINDOW(window), false);
    gtk_window_move(GTK_WINDOW(window), 0, 0);
#endif
}

webview::webview *Kromblast::Window::get_webview()
{
    return kromblast_window;
}

/**
 * @brief Set the html of the window
 * @param html Html
 */
void Kromblast::Window::set_html(const std::string html) { kromblast_window->set_html(html); }

/**
 * @brief Navigate to an url
 * @param url Url
 */
void Kromblast::Window::navigate(const std::string url) { kromblast_window->navigate(url); }

std::string Kromblast::Window::get_current_url()
{
    #if KB_OS_FAMILY == KB_OS_LINUX
        return webkit_web_view_get_uri(WEBKIT_WEB_VIEW((GtkWidget *)this->kromblast_window->get_webview()));
    #endif
}

/**
 * @brief Run the window
 */
void Kromblast::Window::run() { kromblast_window->run(); }

void Kromblast::Window::init_inject(const std::string js) { kromblast_window->init(js); }
void Kromblast::Window::inject(const std::string js) { kromblast_window->eval(js); }

