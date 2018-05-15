// dirpoll.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 dirpoll.cpp -o tmpl && tmpl
#include "CLI11.hpp"
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app{"App description"};

    // Define options
    bool f;
    app.add_flag("-f", f, "flag f");

    int p = 0;
    app.add_option("-p", p, "int parameter");

    std::string s = "s";
    app.add_option("-s,--str", s, "string parameter");

    CLI11_PARSE(app, argc, argv);

    std::cout << "bool flag value: " << f << std::endl;
    std::cout << "int param value: " << p << std::endl;
    std::cout << "str param value: " << s << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Examples of bad command(s):" << std::endl;
    std::cout << argv[0] << " -str=my_str" << std::endl;
    return 0;
}
