#include "utf8.h"

Array splitUtf8(const std::string& text) {  // O(n)
    Array symbols;
    std::size_t i = 0;
    while (i < text.size()) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        std::size_t length = 1;
        if (c >= 0xF0) {
            length = 4;
        } else if (c >= 0xE0) {
            length = 3;
        } else if (c >= 0xC0) {
            length = 2;
        }
        if (i + length > text.size()) {
            length = text.size() - i;
        }

        symbols.pushBack(text.substr(i, length));
        i += length;
    }
    return symbols;
}
