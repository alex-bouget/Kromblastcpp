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
     * @brief Structure for simulating a function
     * @property name Function name
     * @property args_nb Arguments number
    */
    struct kromblast_function
    {
        char* name;
        int   args_nb;
    };

    /**
     * @brief Class of a kromblast library (interface)
     * @method library_callback Return the list of functions
     * @method call_function Call a function
     * @method has_function Check if a function exists in this library
    */
    class KromLib
    {
    public:
        
        /**
         * @brief Return the list of functions
         * @param nb_function Number of functions in the list
         * @return Return the list of functions
        */
        virtual struct kromblast_function* library_callback(int *nb_function) = 0;
        
        /**
         * @brief Call a function
         * @param data Data of the function to call
         * @return Return the result of the function
        */
        virtual char *call_function(const struct kromblast_function_called data) = 0;
        
        /**
         * @brief Check if a function exists in this library
         * @param function_name Name of the function to check
         * @return Return true if the function exists
        */
        virtual bool has_function(char *function_name) = 0;
        
        /**
         * @brief Destructor
        */
        virtual ~KromLib() = default;
    };


    /**
     * function type for creating a library
    */
    typedef KromLib *(*create_lib_t)();
}
#endif