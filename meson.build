project(
    'Kromblast',
    'cpp',
    version : '0.1',
    default_options : ['warning_level=3']
)

# Extra Dependencies



# Intra Dependencies
Kromblast_dep = declare_dependency(
    include_directories : include_directories('src/kromblast')
    link_with : static_library('kromblast', 'src/kromblast/kromblast.cpp')
)

utils_dep = declare_dependency(
    include_directories : include_directories('src/utils')
    link_with : static_library('utils', 'src/utils/load_lib.cpp', 'src/utils/function_call.cpp')
)


# Executable

exe = executable(
    'Kromblast',
    'src/main.cpp',
    install : true
)