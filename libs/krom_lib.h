#ifndef KROM_LIB_H
#define KROM_LIB_H

class KromLib {
    public:
        virtual char** library_callback(int* nb_function) = 0;
};

#endif