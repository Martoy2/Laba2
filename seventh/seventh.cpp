#include "seventh.h"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include "lru_cache.h"

int runSeventh() {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream capIn(line);
    std::int64_t capacity = 0;
    if (!(capIn >> capacity) || capacity <= 0) {
        std::cerr << "error: expected positive capacity" << std::endl;
        return 1;
    }

    LRUCache cache(static_cast<std::size_t>(capacity));
    std::string command;
    bool first = true;
    while (std::cin >> command) {
        if (command == "SET") {
            std::string key;
            std::string value;
            if (!(std::cin >> key >> value)) {
                std::cerr << "error: SET needs key and value" << std::endl;
                return 1;
            }
            cache.set(key, value);
        } else if (command == "GET") {
            std::string key;
            if (!(std::cin >> key)) {
                std::cerr << "error: GET needs key" << std::endl;
                return 1;
            }
            std::string value;
            std::cout << (first ? "" : " ") << (cache.get(key, value) ? value : "-1");
            first = false;
        } else {
            std::cerr << "error: unknown query: " << command << std::endl;
            return 1;
        }
    }
    std::cout << std::endl;

    return 0;
}
