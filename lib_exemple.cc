#include "kromblast_library.h"
#include <string.h>
#include <stdio.h>

class lib1 : public KromblastLib::KromLib
{
private:
    int function_number = 1;
    struct KromblastLib::kromblast_function functions[1] = {
        {(char*)"libtest.increment", 1}};

public:
    struct KromblastLib::kromblast_function *library_callback(int *nb_function)
    {
        *nb_function = function_number;
        return functions;
    }

    char *increment(int nb)
    {
        char *result = new char[100];
        sprintf(result, "{\"count\": %d}", ++nb);
        int len = strlen(result);
        char *result2 = new char[len + 1];
        strcpy(result2, result);
        return result2;
    }

    char *call_function(struct KromblastLib::kromblast_function_called function_called)
    {
        if (strcmp(function_called.name, (char *)"libtest.increment") == 0)
        {
            int nb = 0;
            sscanf(function_called.args[0], "%d", &nb);
            return increment(nb);
        }
        return nullptr;
    }

    bool has_function(char *function_name)
    {
        if (strcmp(function_name, (char *)"libtest.increment") == 0)
        {
            return true;
        }
        return false;
    }

    ~lib1()
    {
        for (int i = 0; i < function_number; i++)
        {
            delete[] functions[i].name;
        }
    }
};

extern "C" KromblastLib::KromLib *create_kromblast_lib()
{
    return new lib1();
}
