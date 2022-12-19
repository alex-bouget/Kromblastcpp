#ifndef KROMBLAST_H
#define KROMBLAST_H

#include "webview.h"
#include "krom_lib.h"
#include <dlfcn.h>
#include <iostream>

KromblastLib::KromLib** kromblast_lib;
webview::webview* kromblast_window;


std::string kromblast_callback(std::string req) {
    static int count = 0;
    return "{\"count\": " + std::to_string(++count) + "}";
}

typedef KromblastLib::KromLib* (*create_lib_t)();

KromblastLib::KromLib* get_lib(std::string lib_name) {
    void* handle = dlopen(lib_name.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return nullptr;
    }
    create_lib_t create_lib = (create_lib_t) dlsym(handle, "create_kromblast_lib");
    if (!create_lib) {
        std::cerr << "Cannot load symbol 'create_lib': " << dlerror() << '\n';
        return nullptr;
    }
    return create_lib();
}


webview::webview create_kromblast_window(std::string title, int width, int height, bool debug, int lib_nb) {
    webview::webview w(debug, nullptr);
    kromblast_window = &w;
    w.set_title(title);
    w.set_size(width, height, WEBVIEW_HINT_NONE);
    w.bind("kromblast", &kromblast_callback);
    kromblast_lib = new KromblastLib::KromLib*[lib_nb];
    return w;
}

#endif