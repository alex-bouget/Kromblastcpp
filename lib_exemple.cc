#include "krom_lib.h"
#include <string.h>
#include <stdio.h>

class lib1 : public KromblastLib::KromLib
{
private:
    struct KromblastLib::kromblast_function functions[1] = {
        {(char*)"libtest.increment", 0}};

public:
    struct KromblastLib::kromblast_function *library_callback(int *nb_function)
    {
        *nb_function = 1;
        return functions;
    }

    char *increment()
    {
        static int count = 0;
        char *result = new char[100];
        sprintf(result, "{\"count\": %d}", ++count);
        int len = strlen(result);
        char *result2 = new char[len + 1];
        strcpy(result2, result);
        return result2;
    }

    char *call_function(char *function_name, char *args)
    {
        if (strcmp(function_name, (char *)"libtest.increment") == 0)
        {
            return increment();
        }
        return nullptr;
    }
};

extern "C" KromblastLib::KromLib *create_kromblast_lib()
{
    return new lib1();
}
