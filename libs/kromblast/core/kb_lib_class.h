#ifndef KB_LIB_CLASS_H
#define KB_LIB_CLASS_H

#include "kb_lib_core.h"
#include "kb_lib_kromblast.h"

/**
 * @brief Namespace of the kromblast library
 * @class KromLib Class of a kromblast library (interface)
 */
namespace KromblastCore
{

    namespace Class
    {
        /**
         * @brief Class of a kromblast library (interface)
         * @property name Name of the library
         * @property version Version of the library
         * @property functions Functions of the library
         * @property functions_nb Number of functions of the library
         */
        class KromLib
        {
        public:
            virtual KromblastCore::kromblast_callback *get_function(int *nb_function) = 0;
            virtual void set_kromblast(KromblastCore::KromblastInterface *kromblast) = 0;
        };

        typedef KromLib* (*kromblast_lib_get_class_t)(void);
    }
}

#endif