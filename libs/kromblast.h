#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "krom_lib.h"


KromLib** kromblast_lib;
webview::webview* kromblast_window;


std::string kromblast_callback(std::string req) {
    static int count = 0;
    return "{\"count\": " + std::to_string(++count) + "}";
}


webview::webview create_kromblast_window(std::string title, int width, int height, bool debug, int lib_nb) {
    webview::webview w(debug, nullptr);
    kromblast_window = &w;
    w.set_title(title);
    w.set_size(width, height, WEBVIEW_HINT_NONE);
    w.bind("kromblast", &kromblast_callback);
    kromblast_lib = new KromLib*[lib_nb];
    return w;
}

#endif