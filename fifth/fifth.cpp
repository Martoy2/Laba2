#include "fifth.h"
#include <iostream>
#include <string>
#include "huffman.h"

int runFifth() {
    std::string text;
    std::getline(std::cin, text);
    if (!text.empty() && text.back() == '\r') {
        text.pop_back();
    }
    if (text.empty()) {
        std::cerr << "error: empty string" << std::endl;
        return 1;
    }

    Huffman huffman(text);
    std::string bits = huffman.encode(text);
    std::string decoded = huffman.decode(bits);

    std::cout << "Code: " << bits << std::endl;
    std::cout << "Table (symbol, frequency, code):" << std::endl;
    huffman.printTable(std::cout);
    std::cout << "Decoded: " << decoded << std::endl;
    std::cout << "Bits: " << bits.size() << " instead of " << text.size() * 8 << std::endl;

    return decoded == text ? 0 : 1;
}
