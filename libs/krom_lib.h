#ifndef KROM_LIB_H
#define KROM_LIB_H

namespace KromblastLib
{

    struct kromblast_function_called
    {
        char*  name;
        char** args;
        int    args_nb;
    };


    struct kromblast_function
    {
        char* name;
        int   args_nb;
    };

    class KromLib
    {
    public:
        virtual struct kromblast_function* library_callback(int *nb_function) = 0;
        virtual char *call_function(char *function_name, char *args) = 0;
    };
}
#endif