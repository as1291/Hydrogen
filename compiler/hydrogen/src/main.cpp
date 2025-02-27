#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <cctype>
#include "./tokenization.hpp"
//Optional is a libraray which allows to return instances when 
//no value is present which is nullopt different from nullptr
//as it is not a pointer

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str) {
    // Implementation here
  
    
}

std::string tokens_to_asm(const std::vector<Token> &tokens){
    std::stringstream output;
    output << "global _start\n_start:\n";
    
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60\n";  
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";  // exit status code
                    output << "    syscall\n";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }

    // Read file as a string
    std::string contents;
    { 
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    std::vector<Token> tokens = tokenize(contents);
    std::cout<<tokens_to_asm(tokens)<<std::endl;
    {
        std::fstream file("out.asm", std::ios::out);
        file<<tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return EXIT_SUCCESS;
}
