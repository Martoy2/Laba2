#include "first.h"
#include <iostream>
#include <string>
#include "xml.h"

int runFirst() {
    std::string text;
    std::getline(std::cin, text);
    if (!text.empty() && text.back() == '\r') {
        text.pop_back();
    }

    std::string result;
    if (repairXml(text, result)) {
        std::cout << result << std::endl;
    } else {
        std::cout << "Incorrect" << std::endl;
    }

    return 0;
}
