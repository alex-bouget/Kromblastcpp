project(
    'Kromblast',
    'cpp',
    version : '0.1',
    default_options : ['warning_level=3']
)

cc = meson.get_compiler('cpp')

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif


# mini_dep = declare_dependency(
#     include_directories : include_directories('src/libs/mini/src/mini')
# )

nlohmann_json_dep = subproject('nlohmann-json').get_variable('nlohmann_json_dep')

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


# Executable

exe = executable(
    'Kromblast',
    'src/main.cpp',
    install : true,
    dependencies : [
        kromblast_api_dep,
        kromblast_dep,
        webview_dep,
        nlohmann_json_dep,
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