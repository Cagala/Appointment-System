#include <iostream>
#include <string>

void clearConsole() {
    std::string emptyLine = "";

    for (int i = 0; i < 30; i++) {
        emptyLine = emptyLine + "\n";
    }

    std::cout << emptyLine << std::endl;
}