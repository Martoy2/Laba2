#include "queue.h"
#include <stdexcept>

Queue::Queue() {}

Queue::Queue(std::initializer_list<std::string> init) {
    for (const std::string& value : init) {
        push(value);
    }
}

void Queue::push(const std::string& value) {  // O(1)
    list.pushBack(value);
}

std::string Queue::pop() {  // O(1)
    if (list.empty()) {
        throw std::out_of_range("Queue: queue is empty");
    }

    std::string value = list.front();
    list.popFront();
    return value;
}

std::string Queue::front() const {  // O(1)
    if (list.empty()) {
        throw std::out_of_range("Queue: queue is empty");
    }

    return list.front();
}

std::size_t Queue::size() const {  // O(1)
    return list.size();
}

bool Queue::empty() const {  // O(1)
    return list.empty();
}

void Queue::clear() {  // O(n)
    list.clear();
}

char Queue::type() const {
    return 'Q';
}

std::string Queue::serialize() const {  // O(n)
    return list.serialize();
}

void Queue::deserialize(const std::string& line) {  // O(n)
    list.deserialize(line);
}

void Queue::print(std::ostream& os) const {  // O(n)
    os << *this;
}

std::ostream& operator<<(std::ostream& os, const Queue& queue) {  // O(n)
    return os << queue.list;
}
