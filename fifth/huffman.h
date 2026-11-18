#ifndef INC_2C_LABA2_HUFFMAN_H
#define INC_2C_LABA2_HUFFMAN_H
#include <cstddef>
#include <ostream>
#include <string>
#include "../structures/array.h"
#include "../structures/hash_table.h"

class Huffman {
public:
    explicit Huffman(const std::string& text);
    ~Huffman();

    Huffman(const Huffman&) = delete;
    Huffman& operator=(const Huffman&) = delete;

public:
    std::string encode(const std::string& text);
    std::string decode(const std::string& bits) const;
    void printTable(std::ostream& os);

private:
    struct Node {
        std::string symbol;
        std::size_t weight;
        Node* left;
        Node* right;
    };

    static Node* takeMin(Node** pool, std::size_t& count);
    void assignCodes(const Node* node, const std::string& code);
    static void destroy(Node* node);

private:
    Node* root;
    Array symbols;
    Array frequencies;
    HashTable<std::string> codes;
};

#endif
