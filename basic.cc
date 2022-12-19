#include "kromblast.h"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Kromblast" << std::endl;
    kromblast::kromblast app("Kromblast", 800, 600, 0, true);
    app.navigate("http://github/Github/Kromblastcpp/test/");
    app.run();
    return 0;
}