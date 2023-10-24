project(
    'Kromblast',
    'cpp',
    version : '0.1',
    default_options : ['warning_level=3']
)

cc = meson.get_compiler('cpp')

# Extra Dependencies

webview_depends = [
    dependency('gtk+-3.0'),
    dependency('webkit2gtk-4.0')
]

webview_lib = library(
    'webview',
    'src/libs/webview/webview.h',
    dependencies : webview_depends,
    install : true
)

webview_dep = declare_dependency(
    include_directories : include_directories('src/libs/webview'),
    dependencies : webview_depends,
    link_with : webview_lib
)

mini_dep = declare_dependency(
    include_directories : include_directories('src/libs/mini')
)

cpp_httplib_dep = subproject('cpp-httplib').get_variable('cpp_httplib_dep')




# Intra Dependencies
kromblast_api_dep = declare_dependency(
    include_directories : include_directories(['src/include/', 'src/include/api']),
)

utils_lib = library(
    'utils',
    'src/utils/load_lib.cpp',
    'src/utils/function_call.cpp',
    include_directories : 'src/utils',
    dependencies : [
        kromblast_api_dep,
        webview_dep,
        cc.find_library('dl')
    ],
    install : true
)

utils_dep = declare_dependency(
    include_directories : include_directories('src/utils'),
    link_with : utils_lib
)

kromblast_lib = library(
    'kromblast',
    'src/kromblast/kromblastlib.cpp',
    include_directories : 'src/kromblast',
    dependencies : [
        kromblast_api_dep,
        utils_dep,
        webview_dep,
        cc.find_library('X11')
    ],
    install : true
)

kromblast_dep = declare_dependency(
    include_directories : include_directories('src/kromblast'),
    link_with : kromblast_lib
)


# Executable

exe = executable(
    'Kromblast',
    'src/main.cpp',
    install : true,
    dependencies : [
        kromblast_api_dep,
        kromblast_dep,
        utils_dep,
        webview_dep,
        mini_dep,
        cc.find_library('stdc++fs'),
        cpp_httplib_dep
    ]
)

lib1 = shared_library(
    'lib_exemple',
    'lib_test/lib_exemple3.cpp',
    dependencies : [
        kromblast_api_dep
    ]
)