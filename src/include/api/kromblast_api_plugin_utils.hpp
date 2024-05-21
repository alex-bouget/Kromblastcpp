#ifndef KRBLST_API_PLUGIN_UTILS_H
#define KRBLST_API_PLUGIN_UTILS_H

#include "kromblast_compiler_utils.hpp"
#include <functional>

#define BIND_CALLBACK(func) std::bind(&func, this, std::placeholders::_1)


#endif