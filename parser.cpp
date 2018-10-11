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

    // TODO make static ?
    char token;
    std::deque<char> expression;
    const char * output_filename = "rdp-output.txt";  // TODO use buffer for both streams?
    std::ofstream outfile;

    void get_token() {
        do {
            token = expression.front();
            expression.pop_front();
        } while (std::isspace(token));  // eats up spaces

        if (token != '\0') {
            std::cout << "\n<gettoken> " << token << " ";
            outfile << "\n<gettoken> " << token << " ";
        }
    }

    void parse(std::string arithmetic_expr) {
        expression.clear();
        std::copy(arithmetic_expr.begin(), arithmetic_expr.end(), std::back_inserter(expression));
        expression.push_back('\0');

        outfile = std::ofstream(output_filename, std::ios::app);

        std::cout << "\nArithmetic Expression: " << arithmetic_expr << "\n<parse> ";
        outfile << "\nArithmetic Expression: " << arithmetic_expr << "\n<parse> ";
        get_token();
        try {
            expr();
            std::cout << "\nEnd parse: success!\n";
            outfile << "\nEnd parse: success!\n";
        } catch (const std::logic_error& err){  // TODO more explicit error msgs - give source of error (token)
            std::cout << "\nEnd parse: failure: " << err.what() << " '" << arithmetic_expr << "'\n";
            outfile << "\nEnd parse: failure: " << err.what() << " '" << arithmetic_expr << "'\n";
        }
    }

    void expr() {
        std::cout << "<expr> ";
        outfile << "<expr> ";
        term();
        while (token == '+') {
            get_token();
            term();
        }
    }

    void term() {
        std::cout << "<term> ";
        outfile << "<term> ";
        factor();
        while (token == '*') {
            get_token();
            factor();
        }
    }

    void factor() {
        std::cout << "<factor> ";
        outfile << "<factor> ";
        if (token == '(') {
            get_token();
            expr();
            if (token == ')') {
                get_token();
            } else {
                throw std::logic_error("invalid arithmetic expression");
            }
        } else {
            number();
        }
    }

    void number() {
        std::cout << "<number> ";
        outfile << "<number> ";
        digit();  // only parses single digits
    }

    void digit() {
        outfile << "<digit> ";
        if (std::isdigit(token)) {
            get_token();
        } else {
            throw std::logic_error("invalid arithmetic expression");
        }
    }
}


int main(int argc, char** argv)
{
    if (argc > 3) {
        throw std::invalid_argument("too many arguments; please try again");
    }
    std::string expression_line;
    const char* input_file = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (std::strstr(argv[i], "=")) {
            char* filename_ = std::strstr(argv[i], "=");
            ++filename_;
            if (std::strstr(argv[i], "of=") or std::strstr(argv[i], "outfile=")) {
                rdp::output_filename = filename_;
                std::cout << "Output file: " << filename_ << std::endl;
            } else if (std::strstr(argv[i], "if=") or std::strstr(argv[i], "infile=")) {
                input_file = filename_;
                std::cout << "Input file: " << filename_ << std::endl;
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
    return 0;
}
