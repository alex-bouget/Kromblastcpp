#ifndef KROM_LIB_H
#define KROM_LIB_H

/**
 * @brief Namespace of the kromblast library
 * @struct kromblast_function_called Structure used to call a function
 * @struct kromblast_function Structure for simulating a function
 * @class KromLib Class of a kromblast library (interface)
*/
namespace KromblastLib
{

    /**
     * @brief Structure used to call a function
     * @property name Function name
     * @property args Arguments
     * @property args_nb Arguments number
    */
    struct kromblast_function_called
    {
        char*  name;
        char** args;
        int    args_nb;
    };

    /**
     * @brief Function type for a callback
    */
    typedef char* (*kromblast_function_callback_t)(const struct kromblast_function_called);

    /**
     * @brief Structure for simulating a function
     * @property name Function name
     * @property args_nb Arguments number
     * @property callback Callback of the function
    */
    struct kromblast_function
    {
        char* name;
        int   args_nb;
        kromblast_function_callback_t callback;
    };
    
    /**
     * @brief Function type for get all callback of the library
    */
    typedef struct kromblast_function* (*get_kromblast_callback_t)(int *nb_function);
}
#endif