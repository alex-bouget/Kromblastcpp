#include "kromblast_library.h"
#include <stdio.h>
#include <stdlib.h>


char *increment(KromblastLib::kromblast_function_called parameters)
{
    int nb = atoi(parameters.args[0]);
    char *result = new char[100];
    sprintf(result, "{\"count\": %d}", ++nb);
    return result;
}

int function_number = 1;
struct KromblastLib::kromblast_function functions[1] = {
    {.name = (char *)"libtest.secondexemple.increment",
     .args_nb = 1,
     .callback = (KromblastLib::kromblast_function_callback_t)&increment}};

extern "C" struct KromblastLib::kromblast_function *get_kromblast_callback(int *nb_function)
{
    *nb_function = function_number;
    return functions;
}