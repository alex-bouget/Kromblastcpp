#include "function_call.hpp"
#include "kb_lib_kromblast.hpp"
#include "kb_lib_core.hpp"
#include "kb_lib_class.hpp"
#include <iostream>
#include <string.h>
#include <vector>
#include <functional>

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
 * @param kromblast_function Kromblast functions
 * @param kromblast Kromblast interface
 * @return Return the result of the function
 */
std::string Kromblast::Utils::Function::call_function(
    struct KromblastCore::kromblast_callback_called function_called,
    std::map<std::string, KromblastCore::kromblast_callback> kromblast_function,
    KromblastCore::KromblastInterface *kromblast)
{
    kromblast->log("Function", "Function called: " + function_called.name);
    if (kromblast_function.find(function_called.name) != kromblast_function.end())
    {
        struct KromblastCore::kromblast_callback func = kromblast_function[function_called.name];
        kromblast->log("Function", "Function found");
        return func.callback(&function_called);
    }
    return (char *)"{\"Error\": \"Function not found\"}";
}