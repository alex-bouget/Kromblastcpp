#include "function_call.h"
#include <iostream>
#include <string.h>

/**
 * @brief count the number of arguments
 * @param args Arguments
 * @return Return the number of arguments
 */
int Kromblast::Utils::Function::count_args(std::string args)
{
    int args_nb = 1;
    for (int i = 0; i < (int)args.length(); i++)
    {
        if (args[i] == ',')
        {
            args_nb++;
        }
    }
    return args_nb;
}

/**
 * @brief call a function in the libraries
 * @param function_called Function called
 * @param kromblast_lib List of the libraries
 * @param kromblast_lib_nb Number of libraries
 * @param kromblast Kromblast instance
 * @return Return the result of the function
 */
char *Kromblast::Utils::Function::call_function(struct KromblastCore::kromblast_callback_called function_called, KromblastCore::kromblast_callback **kromblast_functions, int kromblast_functions_nb, KromblastCore::KromblastInterface *kromblast)
{
    for (int i = 0; i < kromblast_functions_nb; i++)
    {
        if (strcmp(function_called.name, kromblast_functions[i]->name) == 0 && function_called.args_nb == kromblast_functions[i]->args_nb)
        {
            kromblast->log("Callback", ("Calling function: " + std::string(function_called.name)).c_str());
            return kromblast_functions[i]->callback(function_called);
        }
    }
    return (char *)"{\"Error\": \"Function not found\"}";
}