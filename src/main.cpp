#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <cctype>

#include "./arena.hpp"

#include "./generation.hpp"
#include "./parser.hpp"
#include "./tokenization.hpp"
// // Optional is a libraray which allows to return instances when
// // no value is present which is nullopt different from nullptr
// // as it is not a pointer

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }
    //  std::fstream file("out.asm", std::ios::out);
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    std::cout << contents << std::endl;
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<node::NodeProg> prog = parser.parse_prog();
    if (!prog.has_value())
    {
        std::cerr << "invalid program" << std::endl;
        exit(EXIT_FAILURE);
    }
    Generator generator(prog.value());
    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog();
    }
    {
        std::stringstream contents_stream;
        std::fstream input("out.asm", std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    std::cout << contents << std::endl;
    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}