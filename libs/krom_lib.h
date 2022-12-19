#ifndef KROM_LIB_H
#define KROM_LIB_H

namespace KromblastLib
{
    class KromLib
    {
    public:
        virtual char **library_callback(int *nb_function) = 0;
        virtual char* call_function(char* function_name, char* args) = 0;
    };
}
#endif