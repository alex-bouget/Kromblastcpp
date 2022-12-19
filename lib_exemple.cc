#include "krom_lib.h"
#include <string.h>


class lib1 : public KromblastLib::KromLib {
public:
    char **library_callback(int *nb_function) {
        char **functions = new char*[1];
        functions[0] = (char*)"test";
        *nb_function = 1;
        return functions;
    }

    char* call_function(char* function_name, char* args) {
        if (strcmp(function_name, (char*)"test") == 0) {
            return (char*)"test";
        }
        return (char*)"";
    }
};

extern "C" KromblastLib::KromLib* create_kromblast_lib() {
    return new lib1();
}
