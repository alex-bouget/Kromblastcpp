#ifndef KB_LIB_CORE_H
#define KB_LIB_CORE_H

/**
 * @brief Namespace of the kromblast library
 * @struct kromblast_callback_called Structure used to call a function
 * @typedef kromblast_callback_t Function type for a callback
 * @struct kromblast_callback Structure for simulating a function
 */
namespace KromblastCore
{
    /**
     * @brief Structure used to call a function
     * @property name Function name
     * @property args Arguments
     * @property args_nb Arguments number
     */
    struct kromblast_callback_called
    {
        char *name;
        char **args;
        int args_nb;
    };

    /**
     * @brief Function type for a callback
     */
    typedef char *(*kromblast_callback_t)(const struct kromblast_callback_called);

    /**
     * @brief Structure for simulating a function
     * @property name Function name
     * @property args_nb Arguments number
     * @property callback Callback of the function
     */
    struct kromblast_callback
    {
        char *name;
        int args_nb;
        kromblast_callback_t callback;
    };

    /**
     * @brief Type of the library
     * @property KB_LIB_CLASS Class library
     * @property KB_LIB_STRUCT Struct library
     */
    enum KromLibType
    {
        KB_LIB_CLASS,
        KB_LIB_STRUCT
    };

    /**
     * @brief get the type of the library on a library
     */
    typedef KromLibType (*kromblast_lib_get_type_t)();
}
#endif