#include "huffman.h"
#include <stdexcept>
#include "../structures/utf8.h"

Huffman::Huffman(const std::string& text) : root(nullptr), codes(Probing::Double) {
    Array all = splitUtf8(text);
    HashTable<std::size_t> counts;
    for (std::size_t i = 0; i < all.size(); ++i) {
        std::size_t* count = counts.find(all.get(i));
        if (count != nullptr) {
            ++*count;
        } else {
            counts.insert(all.get(i), 1);
            symbols.pushBack(all.get(i));
        }
    }

    for (std::size_t i = 1; i < symbols.size(); ++i) {
        std::string current = symbols.get(i);
        std::size_t weight = *counts.find(current);
        std::size_t j = i;
        while (j > 0 && *counts.find(symbols.get(j - 1)) < weight) {
            symbols.set(j, symbols.get(j - 1));
            --j;
        }
        symbols.set(j, current);
    }

    std::size_t count = symbols.size();
    if (count == 0) {
        return;
    }

    Node** pool = new Node*[count];
    for (std::size_t i = 0; i < count; ++i) {
        std::size_t weight = *counts.find(symbols.get(i));
        pool[i] = new Node{symbols.get(i), weight, nullptr, nullptr};
        frequencies.pushBack(std::to_string(weight));
    }

    while (count > 1) {
        Node* left = takeMin(pool, count);
        Node* right = takeMin(pool, count);
        pool[count++] = new Node{left->symbol + right->symbol, left->weight + right->weight, left, right};
    }
    root = pool[0];
    delete[] pool;

    if (root->left == nullptr) {
        codes.insert(root->symbol, "0");
    } else {
        assignCodes(root, "");
    }
}

Huffman::~Huffman() {
    destroy(root);
}

Huffman::Node* Huffman::takeMin(Node** pool, std::size_t& count) {  // O(k)
    std::size_t best = count - 1;
    for (std::size_t i = count - 1; i-- > 0;) {
        if (pool[i]->weight < pool[best]->weight) {
            best = i;
        }
    }

    Node* node = pool[best];
    for (std::size_t i = best; i + 1 < count; ++i) {
        pool[i] = pool[i + 1];
    }
    --count;
    return node;
}

void Huffman::assignCodes(const Node* node, const std::string& code) {  // O(k)
    if (node->left == nullptr) {
        codes.insert(node->symbol, code);
        return;
    }

    assignCodes(node->left, code + "0");
    assignCodes(node->right, code + "1");
}

void Huffman::destroy(Node* node) {
    if (node == nullptr) return;

    destroy(node->left);
    destroy(node->right);
    delete node;
}

std::string Huffman::encode(const std::string& text) {  // O(n * длина кода)
    Array all = splitUtf8(text);
    std::string bits;
    for (std::size_t i = 0; i < all.size(); ++i) {
        std::string* code = codes.find(all.get(i));
        if (code == nullptr) {
            throw std::invalid_argument("symbol not in table: " + all.get(i));
        }
        bits += *code;
    }
    return bits;
}

std::string Huffman::decode(const std::string& bits) const {  // O(длины кода)
    std::string text;
    if (root == nullptr) {
        return text;
    }

    const Node* node = root;
    for (char bit : bits) {
        if (bit != '0' && bit != '1') {
            throw std::invalid_argument("bad bit in code");
        }
        if (root->left != nullptr) {
            node = bit == '0' ? node->left : node->right;
        }
        if (node->left == nullptr) {
            text += node->symbol;
            node = root;
        }
    }
    return text;
}

void Huffman::printTable(std::ostream& os) {  // O(k)
    for (std::size_t i = 0; i < symbols.size(); ++i) {
        os << "'" << symbols.get(i) << "' " << frequencies.get(i) << " " << *codes.find(symbols.get(i)) << std::endl;
    }
}
