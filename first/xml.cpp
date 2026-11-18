#include "xml.h"
#include "../structures/stack.h"

static bool isNameChar(char c) {
    return c >= 'a' && c <= 'z';
}

bool isValidXml(const std::string& text) {  // O(n)
    Stack tags;
    std::size_t i = 0;

    while (i < text.size()) {
        if (text[i] == ' ') {
            ++i;
            continue;
        }
        if (text[i] != '<') {
            return false;
        }
        ++i;

        bool closing = false;
        if (i < text.size() && text[i] == '/') {
            closing = true;
            ++i;
        }

        std::size_t start = i;
        while (i < text.size() && isNameChar(text[i])) {
            ++i;
        }
        if (i == start || i == text.size() || text[i] != '>') {
            return false;
        }
        std::string name = text.substr(start, i - start);
        ++i;

        if (!closing) {
            tags.push(name);
        } else if (tags.empty() || tags.pop() != name) {
            return false;
        }
    }

    return tags.empty();
}

bool repairXml(const std::string& text, std::string& result) {  // O(29 * n^2)
    if (isValidXml(text)) {
        result = text;
        return true;
    }

    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz</>";
    std::string candidate = text;

    for (std::size_t i = text.size(); i-- > 0;) {
        if (text[i] == ' ') {
            continue;
        }

        for (char c : alphabet) {
            if (c == text[i]) {
                continue;
            }

            candidate[i] = c;
            if (isValidXml(candidate)) {
                result = candidate;
                return true;
            }
        }
        candidate[i] = text[i];
    }

    return false;
}
