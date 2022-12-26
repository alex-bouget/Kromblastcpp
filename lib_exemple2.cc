#include "kb_lib_class.h"
#include "kb_lib_core.h"
#include "kb_lib_kromblast.h"
#include <stdio.h>
#include <stdlib.h>

class Exemple : public KromblastCore::Class::KromLib
{
public:
    static char *increment(KromblastCore::kromblast_callback_called parameters)
    {
        int nb = atoi(parameters.args[0]);
        char *result = new char[100];
        sprintf(result, "{\"count\": %d}", ++nb);
        return result;
    }

    int function_number = 1;
    struct KromblastCore::kromblast_callback functions[1] = {
        {(char *)"libtest.secondexemple.increment",
         1,
         (KromblastCore::kromblast_callback_t)&increment}};

    KromblastCore::kromblast_callback *get_function(int *nb_function)
    {
        *nb_function = function_number;
        return functions;
    }

    void set_kromblast(KromblastCore::KromblastInterface *kromblast)
    {
        kromblast->log("Exemple", "Exemple library loaded");
    }
};

extern "C" KromblastCore::KromLibType kromblast_lib_get_type()
{
    return KromblastCore::KromLibType::KB_LIB_CLASS;
}

extern "C" KromblastCore::Class::KromLib *kromblast_lib_get_class()
{
    return (new Exemple());
}