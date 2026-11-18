#include "query.h"
#include <cctype>
#include <stdexcept>

Array tokenize(const std::string& query) {
    Array tokens;
    std::string token;
    bool inToken = false;
    bool inQuotes = false;

    for (std::size_t i = 0; i < query.size(); ++i) {
        char c = query[i];

        if (inQuotes) {
            if (c == '\\' && i + 1 < query.size()) {
                token += query[++i];
            } else if (c == '"') {
                inQuotes = false;
            } else {
                token += c;
            }
        } else if (c == '"') {
            inQuotes = true;
            inToken = true;
        } else if (std::isspace(static_cast<unsigned char>(c))) {
            if (inToken) {
                tokens.pushBack(token);
                token.clear();
                inToken = false;
            }
        } else {
            token += c;
            inToken = true;
        }
    }

    if (inQuotes) {
        throw std::invalid_argument("query: unclosed quote");
    }
    if (inToken) {
        tokens.pushBack(token);
    }

    return tokens;
}
