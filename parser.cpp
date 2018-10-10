#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <deque>
#include <set>


namespace rdp {

    // TODO make static ?
    std::set<char> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char token;
    std::deque<char> expression;
    std::string outfile = "rdp-output.txt";

    void get_token();
    void parse(std::string arithmetic_expr);
    void expr();
    void factor();
    void term();
    void number();
    void digit();

    void get_token() {
        std::cout << std::endl << "<gettoken> ";
        token = expression.front();
        expression.pop_front();
        // check for '\0' here?
    }

    void parse(std::string arithmetic_expr) {
        expression.clear();
        std::copy(arithmetic_expr.begin(), arithmetic_expr.end(), std::back_inserter(expression));
        expression.push_back('\0');

        std::cout << "Begin parse: " << arithmetic_expr << std::endl;
        std::cout << "<parse> ";
        get_token();
        expr();
    }

    void expr() {
        std::cout << "<expr> ";
        term();
        while (token == '+') {
            get_token();
            term();
        }
    }

    void term() {
        std::cout << "<term> ";
        factor();
        while (token == '*') {
            get_token();
            factor();
        }
    }

    void factor() {
        std::cout << "<factor> ";
        if (token == '(') {
            get_token();
            expr();
            if (token == ')') {
                get_token();
            } else {
                throw std::logic_error("");  // TODO err msg
            }
        } else {
            number();
        }
    }

    void number() {
        std::cout << "<number> ";
        digit();  // only parses single digits
    }

    void digit() {
        std::cout << "<digit> ";
        if (digits.count(token) == 1) {
            get_token();
        } else {
            // TODO check for '\0' char as well . . .
            throw std::logic_error("");  // TODO err msg
        }
    }
}


int main(int argc, char** argv)
{
    std::string expression_line;  // TODO make static ?

    if (argc > 1) {
        if (std::strcmp(argv[1], "-f") == 0 or std::strcmp(argv[1], "--file") == 0) {
            if (argc < 3 or std::strcmp(argv[2], "") == 0) {
                throw std::invalid_argument("file option '-f' or '--file' was specified, but no file path argument followed");
            } else {
                std::cout << "File: " << argv[2] << std::endl;
                std::ifstream file(argv[2]);
                while (std::getline(file, expression_line)) {
                    rdp::parse(expression_line);
                }
            }
        } else {
            throw std::invalid_argument("invalid argument specified: '" + std::string(argv[1]) + "'");
        }
    } else {
        while (true) {
            std::cout << std::endl << "Enter arithmetic expression: ";
            if (!std::getline(std::cin, expression_line))
                return -1; // I/O error
            if (expression_line == "q" or expression_line == "quit" or expression_line == "exit")
                break;
            if (!expression_line.empty()) {
                rdp::parse(expression_line);
            }
        }
    }
    return 0;
}
