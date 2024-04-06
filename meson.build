project(
    'Kromblast',
    'cpp',
    version : '0.2',
    default_options : ['warning_level=3', 'cpp_std=c++20']
)

cc = meson.get_compiler('cpp')

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif


# mini_dep = declare_dependency(
#     include_directories : include_directories('src/libs/mini/src/mini')
# )

nlohmann_json_dep = subproject('nlohmann-json').get_variable('nlohmann_json_dep')
argparse_dep = subproject('argparse').get_variable('argparse_dep')

cpp_httplib_dep = subproject('cpp-httplib').get_variable('cpp_httplib_dep')
# Extra Dependencies
webview_dep = subproject('webview-meson').get_variable('libwebview_dep')




# Intra Dependencies
kromblast_api_dep = declare_dependency(
    include_directories : include_directories(['src/include/', 'src/include/api']),
)

kromblast_lib = library(
    'kromblast',
    'src/kromblast/kromblastlib.cpp',
    'src/utils/load_lib.cpp',
    include_directories : ['src/kromblast', 'src/utils'],
    dependencies : [
        kromblast_api_dep,
        webview_dep,
        cc.find_library('X11'),
        cc.find_library('dl'),
    ],
    install : true
)

kromblast_dep = declare_dependency(
    include_directories : include_directories(['src/kromblast', 'src/utils']),
    link_with : kromblast_lib
)

kromblast_config_dep = declare_dependency(
    include_directories : include_directories(['src/config', 'src/mode']),
)

# Executable

exe = executable(
    'Kromblast',
    'src/main.cpp',
    'src/config/config_constructor.cpp',
    'src/config/json_config.cpp',
    'src/config/argsparse_config.cpp',
    'src/config/utils.cpp',
    'src/mode/mode.cpp',
    install : true,
    dependencies : [
        kromblast_api_dep,
        kromblast_dep,
        kromblast_config_dep,
        webview_dep,
        nlohmann_json_dep,
        cc.find_library('stdc++fs'),
        cpp_httplib_dep,
        argparse_dep
    ]
)

lib1 = shared_library(
    'lib_exemple',
    'lib_test/lib_exemple3.cpp',
    dependencies : [
        kromblast_api_dep
    ]
)