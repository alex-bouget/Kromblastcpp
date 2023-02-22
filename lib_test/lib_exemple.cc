#include "kb_lib_core.hpp"
#include "kb_lib_struct.hpp"
#include <stdio.h>
#include <stdlib.h>

char *increment(KromblastCore::kromblast_callback_called parameters)
{
    int nb = atoi(parameters.args[0]);
    char *result = new char[100];
    sprintf(result, "{\"count\": %d}", ++nb);
    return result;
}

int function_number = 1;
struct KromblastCore::kromblast_callback functions[1] = {{
    .name = (char *)"libtest.secondexemple.increment",
    .args_nb = 1,
    .callback = (KromblastCore::kromblast_callback_t)&increment}};

extern "C" KromblastCore::KromLibType kromblast_lib_get_type()
{
    return KromblastCore::KB_LIB_STRUCT;
}

extern "C" struct KromblastCore::kromblast_callback *kromblast_lib_struct_get_function(int *nb_function)
{
    *nb_function = function_number;
    return functions;
}