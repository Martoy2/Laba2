#include "stack.h"
#include <stdexcept>

Stack::Stack() {}

Stack::Stack(std::initializer_list<std::string> init) {
    for (const std::string& value : init) {
        push(value);
    }
}

void Stack::push(const std::string& value) {  // O(1)
    list.pushFront(value);
}

std::string Stack::pop() {  // O(1)
    if (list.empty()) {
        throw std::out_of_range("Stack: stack is empty");
    }

    std::string value = list.front();
    list.popFront();
    return value;
}

std::string Stack::top() const {  // O(1)
    if (list.empty()) {
        throw std::out_of_range("Stack: stack is empty");
    }

    return list.front();
}

std::size_t Stack::size() const {  // O(1)
    return list.size();
}

bool Stack::empty() const {  // O(1)
    return list.empty();
}

void Stack::clear() {  // O(n)
    list.clear();
}

char Stack::type() const {
    return 'S';
}

std::string Stack::serialize() const {  // O(n)
    return list.serialize();
}

void Stack::deserialize(const std::string& line) {  // O(n)
    list.deserialize(line);
}

void Stack::print(std::ostream& os) const {  // O(n)
    os << *this;
}

std::ostream& operator<<(std::ostream& os, const Stack& stack) {  // O(n)
    return os << stack.list;
}
