#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>


void parse() {

}

void expr() {

}

void term() {

}

void factor() {

}

void number() {

}


int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (std::strcmp(argv[1], "-f") == 0 or std::strcmp(argv[1], "--file") == 0) {
            if (argc < 3 or std::strcmp(argv[2], "") == 0) {
                throw std::invalid_argument("file option '-f' or '--file' was specified, but no file path argument followed");
            } else {
                std::cout << "File: " << argv[2] << std::endl;
                std::string line;
                std::ifstream file(argv[2]);
                while (std::getline(file, line)) {
                    std::cout << "Begin parse: " << line << std::endl;
                    // TODO call parse() on line
                }
            }
        } else {
            throw std::invalid_argument("invalid argument specified: '" + std::string(argv[1]) + "'");
        }
    } else {
        std::string expression;  // Make global? use global Token? better to encapsulate . . .
        while (true) {
            std::cout << "Enter expression: ";
            if (!std::getline(std::cin, expression))
                return -1; // I/O error
            if (expression == "q" or expression == "quit" or expression == "exit")
                break;
            if (!expression.empty()) {
                std::cout << "Begin parse: " << expression << std::endl;
                // TODO call parse() on expression
            }
        }
    }
    return 0;
}
