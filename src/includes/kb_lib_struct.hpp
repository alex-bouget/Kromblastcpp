#ifndef KB_LIB_STRUCT_H
#define KB_LIB_STRUCT_H

#include "kb_lib_core.hpp"

/**
 * @brief Namespace of the kromblast structure library
 * @typedef kromblast_lib_struct_get_function_t Function on a kromblast structured library for getting a function
 */
namespace KromblastCore
{
    namespace Struct
    {
        /**
         * @brief Function on a kromblast structured library
         */
        typedef struct KromblastCore::kromblast_callback *(*kromblast_lib_struct_get_function_t)(int *nb_function);
    }
}

#endif