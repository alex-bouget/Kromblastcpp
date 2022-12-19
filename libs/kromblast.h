#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include <iostream>

namespace kromblast
{

    std::string kromblast_callback(std::string arg)
    {
        return "\"Return this string to the JS function 'myBoundCallback'\"";
        ;
    }
    class kromblast
    {

    private:
        webview::webview view;

    public:
        kromblast(bool debug)
        {
            view = webview::webview(debug, nullptr);
            view.bind("kromblast", &kromblast_callback);
        }
        kromblast(const std::string &title, bool debug)
        {
            view = webview::webview(debug, nullptr);
            view.set_title(title);
            view.bind("kromblast", &kromblast_callback);
        }
        kromblast(const std::string &title, int width, int height, int resizable, bool debug)
        {
            view = webview::webview(debug, nullptr);
            view.set_title(title);
            view.set_size(width, height, WEBVIEW_HINT_NONE);
            view.bind("kromblast", &kromblast_callback);
        }
        void bind(std::string name, std::function<std::string(std::string)> callback)
        {
            if (name == "kromblast")
            {
                throw std::invalid_argument("kromblast is a reserved name");
            }
            view.bind(name, callback);
        }
        void navigate(const std::string &url)
        {
            view.navigate(url);
            view.bind("kromblast", kromblast_callback);
        }
        void run()
        {
            view.run();
        }
    };

} // namespace kromblast

#endif