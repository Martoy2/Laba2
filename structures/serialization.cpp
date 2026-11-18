#include "serialization.h"

std::string escapeValue(const std::string& value) {
    if (value.empty()) {
        return "\\e";
    }

    std::string token;
    for (char c : value) {
        switch (c) {
            case ' ':  token += "\\s"; break;
            case '\t': token += "\\t"; break;
            case '\n': token += "\\n"; break;
            case '\r': token += "\\r"; break;
            case '\\': token += "\\\\"; break;
            default:   token += c;
        }
    }

    return token;
}

std::string unescapeValue(const std::string& token) {
    if (token == "\\e") {
        return "";
    }

    std::string value;
    for (std::size_t i = 0; i < token.size(); ++i) {
        if (token[i] != '\\' || i + 1 == token.size()) {
            value += token[i];
            continue;
        }

        ++i;
        switch (token[i]) {
            case 's': value += ' '; break;
            case 't': value += '\t'; break;
            case 'n': value += '\n'; break;
            case 'r': value += '\r'; break;
            default:  value += token[i];
        }
    }

    return value;
}
