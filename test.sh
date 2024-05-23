#/bin/bash

# {
#     "plugin_folder": "kromlib",
#     "plugin_config": {
#         "liblib_exemple": {
#             "joke": "I'm a joke"
#         }
#     },
#     "debug": true,
#     "mode": "LOCAL",
#     "path": "test",
#     "window": {
#         "size": [
#             800,
#             600
#         ],
#         "title": "Kromblast",
#         "fullscreen": false,
#         "frameless": false
#     },
#     "registry": [
#         "file://*",
#         "*://*",
#         "http://*",
#         "*://*/index.html"
#     ]
# }

./build/Kromblast conf \
    -t "Kromblast" \
    -sw 800 -sh 600 \
    -m LOCAL \
    -hs LOCAL \
    -d \
    -r "file://*" -r "*://*" -r "http://*" -r "*://*/index.html" \
    -l "build/liblib_exemple.so" \
    -lc "liblib_exemple:joke=I'm a joke"