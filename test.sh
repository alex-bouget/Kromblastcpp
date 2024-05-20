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
    -w 800 -h 600 \
    -m LOCAL \
    --host "test" \
    -d \
    -a "file://*" -a "*://*" -a "http://*" -a "*://*/index.html" \
    -p "kromlib" \
    -c "liblib_exemple:joke=I'm a joke"