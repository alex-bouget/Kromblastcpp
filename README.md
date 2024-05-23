# Kromblastcpp

Use native functions of operating system in a website.

## Dependencies

- [argparse](https://github.com/p-ranav/argparse)
- [nlohmann/json](https://github.com/nlohmann/json)
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [webview](https://github.com/webview/webview)


## Build Kromblastcpp

- Need meson, ninja and a compiler that supports C++20.
- Need to install dependencies of webview, see [here](https://github.com/webview/webview?tab=readme-ov-file#prerequisites)

*WARNING*: Kromblastcpp not work in [Windows](https://github.com/alex-bouget/Kromblastcpp/issues/42) and [MacOS](https://github.com/alex-bouget/Kromblastcpp/issues/43) yet

```bash
meson build
ninja -C build
```

## Run Kromblastcpp

| Command | Description |
| --- | --- |
| `./build/kromblastcpp conf` | Run Kromblastcpp with a configuration from the command |
| `./build/kromblastcpp json` | Run Kromblastcpp with a configuration from a JSON file |

### with command line

| Option | Description | Default | Example |
| --- | --- | --- | --- |
| `-h, --help` | Show help message | | |
| `-v, --version` | Show version | | |
| `-t, --title` | Set the title of the window | | `-t "My title"` |
| `-sw, --width` | Set the width of the window | | `-sw 800` |
| `-sh, --height` | Set the height of the window | | `-sh 600` |
| `-f, --fullscreen` | Set the window in fullscreen | `false` | |
| `-fl, --frameless` | Set the window without frame | `false` | |
| `-d, --debug` | Set Kromblast in debug mode | `false` | |
| `-r, -registry` | Set multiple approved registry (url who can use native functions) | | `-r "https://*" -r "file://*/index.html"` |
| `-m, --mode` | Set the mode of the host (LOCAL, HOSTED, SERVER, DEBUG) | | `-m LOCAL` |
| `-hs, --host` | Set the host of the server or the path to the file | | `-hs "http://localhost:8080"` or `-hs "/path/to/file.html"` |
| `-lp, --lib-path` | Set the path to folder with libraries | | `-lp "/path/to/folder"` |
| `-l, --lib` | Set the library to load | | `-l "mylib.so"` |
| `-lc, --lib-config` | Set the configuration of the library | | `-lc "mylib:key=value" -lc "mylib:key2=value2"` |


## with JSON file

```bash
./build/kromblastcpp json /path/to/config.json
```

### Example of JSON file

```json
{
    "window": {
        "title": "My title",
        "size":  [800, 600],
        "fullscreen": false,
        "frameless": false,
    },
    "debug": false,
    "registry": ["https://*", "file://*/index.html"],
    "mode": "LOCAL",
    "host": "http://localhost:8080",
    "lib_folder": "/path/to/folder",
    "libraries": ["mylib.so", "mylib2.so"],
    "lib_config": {
        "mylib": {
            "key": "value"
        },
    }
}
```

## Create a library

The code for begin a library is:

```cpp
// Kromblast::Class::KromLib
#include "kromblast_lib_plugin.hpp"

// Kromblast::Core::kromblast_callback_called_t
#include "kromblast_lib_plugin_callback.hpp"

// BIND_CALLBACK
#include "kromblast_api_plugin_utils.hpp"

class Plugin : public Kromblast::Class::KromLib
{
public:
    // Version of the library (not used yet)
    std::string get_version() {
        return "0.1.0";
    }

    // at_start is called when the library is loaded
    void at_start() override {
    }

    // load_functions is called after all libraries are loaded
    // and allow to claim your functions for the webview
    void load_functions() override {
        kromblast().get_plugin()->claim_callback(
            "js_class.function", // Name of the function in the webview
            1, // Number of arguments

            // BIND_CALLBACK is a macro for bind the function to the current class
            // see std::bind for more information
            BIND_CALLBACK(Plugin::function),

            // Add regex in the registry only for this function
            // Could be useful if this function can be used in a page that not allowed by the registry
            std::vector<std::regex>({std::regex("https://*"), std::regex("file://*/index.html")})
        );
    }

    // function return dump json
    // parameters is a struct with the arguments of the function
    std::string function(Kromblast::Core::kromblast_callback_called_t *parameters) {
        kromblast().get_logger()->log("Plugin", "function called");
        kromblast().get_logger()->log("Plugin",parameters->args.at(0));
        return R"({"result": "ok"})";
    }
};

// Register the library
extern "C" Kromblast::Class::KromLib *kromblast_lib_get_class()
{
    return new Plugin();
}
```