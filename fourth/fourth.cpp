#include "fourth.h"
#include <iostream>
#include <sstream>
#include <string>
#include "../structures/array.h"
#include "../structures/hash_set.h"
#include "../structures/serialization.h"

static const std::size_t MAX_ELEMENTS = 20;

static std::string keyOf(const Array& elements, const int* chosen, int count) {  // O(k^2)
    Array sorted;
    for (int i = 0; i < count; ++i) {
        std::string value = elements.get(chosen[i]);
        std::size_t pos = sorted.size();
        while (pos > 0 && sorted.get(pos - 1) > value) {
            --pos;
        }
        sorted.insert(pos, value);
    }

    std::string key;
    for (std::size_t i = 0; i < sorted.size(); ++i) {
        key += escapeValue(sorted.get(i)) + ' ';
    }
    return key;
}

static void combinations(const Array& elements, int* chosen, int count, int size, int start,
                         HashSet& seen, bool& first) {  // O(n * 2^n) для всех размеров
    if (count == size) {
        if (!seen.add(keyOf(elements, chosen, count))) {
            return;
        }

        std::cout << (first ? "" : ", ") << "{";
        for (int i = 0; i < count; ++i) {
            std::cout << (i ? ", " : "") << elements.get(chosen[i]);
        }
        std::cout << "}";
        first = false;
        return;
    }

    for (int i = start; i < static_cast<int>(elements.size()); ++i) {
        chosen[count] = i;
        combinations(elements, chosen, count + 1, size, i + 1, seen, first);
    }
}

int runFourth() {
    std::string line;
    std::getline(std::cin, line);

    Array elements;
    std::istringstream in(line);
    std::string token;
    while (in >> token) {
        elements.pushBack(token);
    }

    if (elements.size() > MAX_ELEMENTS) {
        std::cerr << "error: too many elements (max " << MAX_ELEMENTS << ")" << std::endl;
        return 1;
    }

    int n = static_cast<int>(elements.size());
    int* chosen = new int[n > 0 ? n : 1];
    HashSet seen;
    bool first = true;

    std::cout << "[";
    for (int size = 0; size <= n; ++size) {
        combinations(elements, chosen, 0, size, 0, seen, first);
    }
    std::cout << "]" << std::endl;

    delete[] chosen;
    return 0;
}
