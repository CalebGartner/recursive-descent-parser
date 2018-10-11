#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <string>
#include <deque>


namespace rdp {

    void get_token();
    void parse(std::string arithmetic_expr);
    void expr();
    void factor();
    void term();
    void number();
    void digit();

    char token;
    static std::string buffer;
    static std::deque<char> expression;

    void get_token() {
        do {
            token = expression.front();
            expression.pop_front();
        } while (std::isspace(token));  // eats up spaces

        if (token != '\0') {
            buffer += std::string("\n<gettoken> ") + token + " ";
        }
    }

    void parse(std::string arithmetic_expr) {
        expression.clear();
        std::copy(arithmetic_expr.begin(), arithmetic_expr.end(), std::back_inserter(expression));
        expression.push_back('\0');


        buffer += "\nArithmetic Expression: " + arithmetic_expr + "\n<parse> ";
        get_token();
        try {
            expr();
            buffer += "\nEnd parse: success!\n";
        } catch (const std::logic_error& err) {
            buffer += "\nEnd parse: failure: " + std::string(err.what()) + " '" + arithmetic_expr + "'\n";
        }
    }

    void expr() {
        buffer += "<expr> ";
        term();
        while (token == '+') {
            get_token();
            term();
        }
    }

    void term() {
        buffer += "<term> ";
        factor();
        while (token == '*') {
            get_token();
            factor();
        }
    }

    void factor() {
        buffer += "<factor> ";
        if (token == '(') {
            get_token();
            expr();
            if (token == ')') {
                get_token();
            } else {
                throw std::logic_error("FACTOR: unbalanced parentheses: invalid arithmetic expression");
            }
        } else {
            number();
        }
    }

    void number() {
        buffer += "<number> ";
        digit();  // only parses single digits
    }

    void digit() {
        buffer += "<digit> ";
        if (std::isdigit(token)) {
            get_token();
        } else {
            throw std::logic_error("DIGIT: missing operand :invalid arithmetic expression");
        }
    }
}


int main(int argc, char** argv)
{
    if (argc > 3) {
        throw std::invalid_argument("too many arguments; please try again");
    }

    std::string expression_line;
    const char * input_file = nullptr;
    const char * output_file = "rdp-output.txt";  // default

    for (int i = 1; i < argc; ++i) {
        if (std::strstr(argv[i], "=")) {
            char* filename_ = std::strstr(argv[i], "=");
            ++filename_;
            if (std::strstr(argv[i], "of=") or std::strstr(argv[i], "outfile=")) {
                output_file = filename_;
            } else if (std::strstr(argv[i], "if=") or std::strstr(argv[i], "infile=")) {
                input_file = filename_;
            } else {
                throw std::invalid_argument("invalid argument specified: '" + std::string(argv[i]) + "'");
            }
        } else {
            throw std::invalid_argument("invalid argument specified: '" + std::string(argv[i]) + "'");
        }
    }

    if (input_file != nullptr) {
        std::ifstream file(input_file);
        while (std::getline(file, expression_line)) {
            rdp::parse(expression_line);
        }
    } else {
        while (true) {
            std::cout << std::endl << "(q, quit, or exit) || Enter arithmetic expression: ";  // add exit options
            if (!std::getline(std::cin, expression_line))
                return -1; // I/O error
            if (expression_line == "q" or expression_line == "quit" or expression_line == "exit")
                break;
            if (!expression_line.empty())
                rdp::parse(expression_line);
        }
    }

    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;

    std::ofstream outfile = std::ofstream(output_file, std::ios::trunc);
    outfile << rdp::buffer;
    std::cout << rdp::buffer;

    return 0;
}
