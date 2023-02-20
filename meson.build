project(
    'Kromblast',
    'cpp',
    version : '0.1',
    default_options : ['warning_level=3']
)

# Extra Dependencies

webview_lib = static_library(
    'webview',
    'src/libs/webview/webview.h',
    dependencies : [
        dependency('gtk+-3.0'),
        dependency('webkit2gtk-4.0')
    ]
)

webview_dep = declare_dependency(
    link_with : webview_lib
)

mini_dep = declare_dependency(
    include_directories : include_directories('src/libs/mini')
)




# Intra Dependencies
kromblast_api_dep = declare_dependency(
    include_directories : include_directories('src/include'),
)


utils_lib = static_library(
    'utils',
    'src/utils/load_lib.cpp',
    'src/utils/function_call.cpp',
    include_directories : 'src/utils',
    dependencies : [
        kromblast_api_dep,
        webview_dep
    ]
)

utils_dep = declare_dependency(
    include_directories : include_directories('src/utils'),
    link_with : utils_lib
)


kromblast_lib = static_library(
    'kromblast',
    'src/kromblast/kromblast.cpp',
    include_directories : 'src/kromblast',
    dependencies : [
        kromblast_api_dep,
        utils_dep,
        webview_dep
    ]
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
        mini_dep
    ]
)