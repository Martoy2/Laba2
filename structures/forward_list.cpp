#include "forward_list.h"
#include <stdexcept>
#include <sstream>
#include "serialization.h"

ForwardList::ForwardList() : head(nullptr), tail(nullptr), sz(0) {}

ForwardList::ForwardList(std::initializer_list<std::string> init)
    : head(nullptr), tail(nullptr), sz(0) {
    for (const std::string& value : init) {
        pushBack(value);
    }
}

ForwardList::ForwardList(const ForwardList& other) : head(nullptr), tail(nullptr), sz(0) {
    for (Node* cur = other.head; cur != nullptr; cur = cur->next) {
        pushBack(cur->value);
    }
}

ForwardList& ForwardList::operator=(const ForwardList& other) {
    if (this == &other) return *this;

    clear();
    for (Node* cur = other.head; cur != nullptr; cur = cur->next) {
        pushBack(cur->value);
    }
    return *this;
}

ForwardList::~ForwardList() {
    clear();
}

Node* ForwardList::findNode(const std::string& value) const {  // O(n) - перебор
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->value == value) {
            return cur;
        }
    }
    return nullptr;
}

void ForwardList::pushFront(const std::string& value) {  // O(1)
    head = new Node{value, head};
    if (tail == nullptr) {
        tail = head;
    }
    ++sz;
}

void ForwardList::pushBack(const std::string& value) {  // O(1) - есть tail
    Node* node = new Node{value, nullptr};
    if (tail == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    ++sz;
}

void ForwardList::insertBefore(const std::string& target, const std::string& value) {  // O(n) - поиск предыдущего
    if (head == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    if (head->value == target) {
        pushFront(value);
        return;
    }

    Node* prev = head;
    while (prev->next != nullptr && prev->next->value != target) {
        prev = prev->next;
    }
    if (prev->next == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    prev->next = new Node{value, prev->next};
    ++sz;
}

void ForwardList::insertAfter(const std::string& target, const std::string& value) {  // O(n) - поиск, вставка O(1)
    Node* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    node->next = new Node{value, node->next};
    if (node == tail) {
        tail = node->next;
    }
    ++sz;
}

void ForwardList::popFront() {  // O(1)
    if (head == nullptr) {
        throw std::out_of_range("ForwardList: list is empty");
    }

    Node* del = head;
    head = head->next;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete del;
    --sz;
}

void ForwardList::popBack() {  // O(n) - поиск предпоследнего
    if (head == nullptr) {
        throw std::out_of_range("ForwardList: list is empty");
    }

    if (head == tail) {
        delete head;
        head = tail = nullptr;
        --sz;
        return;
    }

    Node* prev = head;
    while (prev->next != tail) {
        prev = prev->next;
    }
    delete tail;
    tail = prev;
    tail->next = nullptr;
    --sz;
}

void ForwardList::removeBefore(const std::string& target) {  // O(n)
    if (head == nullptr || head->value == target) {
        throw std::invalid_argument("ForwardList: no element before target");
    }

    if (head->next != nullptr && head->next->value == target) {
        popFront();
        return;
    }

    Node* prev = head;
    while (prev->next != nullptr && prev->next->next != nullptr
           && prev->next->next->value != target) {
        prev = prev->next;
    }
    if (prev->next == nullptr || prev->next->next == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    Node* del = prev->next;
    prev->next = del->next;
    delete del;
    --sz;
}

void ForwardList::removeAfter(const std::string& target) {  // O(n) - поиск, удаление O(1)
    Node* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }
    if (node->next == nullptr) {
        throw std::invalid_argument("ForwardList: no element after target");
    }

    Node* del = node->next;
    node->next = del->next;
    if (del == tail) {
        tail = node;
    }
    delete del;
    --sz;
}

void ForwardList::removeValue(const std::string& value) {  // O(n)
    if (head == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    if (head->value == value) {
        popFront();
        return;
    }

    Node* prev = head;
    while (prev->next != nullptr && prev->next->value != value) {
        prev = prev->next;
    }
    if (prev->next == nullptr) {
        throw std::invalid_argument("ForwardList: element not found");
    }

    Node* del = prev->next;
    prev->next = del->next;
    if (del == tail) {
        tail = prev;
    }
    delete del;
    --sz;
}

std::string ForwardList::front() const {  // O(1)
    if (head == nullptr) {
        throw std::out_of_range("ForwardList: list is empty");
    }

    return head->value;
}

std::string ForwardList::back() const {  // O(1)
    if (tail == nullptr) {
        throw std::out_of_range("ForwardList: list is empty");
    }

    return tail->value;
}

bool ForwardList::find(const std::string& value) const {  // O(n)
    return findNode(value) != nullptr;
}

std::size_t ForwardList::size() const {  // O(1)
    return sz;
}

bool ForwardList::empty() const {  // O(1)
    return sz == 0;
}

void ForwardList::clear() {  // O(n)
    while (head != nullptr) {
        Node* del = head;
        head = head->next;
        delete del;
    }
    tail = nullptr;
    sz = 0;
}

char ForwardList::type() const {
    return 'F';
}

std::string ForwardList::serialize() const {  // O(n)
    std::string line;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        line += escapeValue(cur->value);
        if (cur->next != nullptr) {
            line += ' ';
        }
    }

    return line;
}

void ForwardList::deserialize(const std::string& line) {  // O(n)
    clear();

    std::istringstream in(line);
    std::string value;
    while (in >> value) {
        pushBack(unescapeValue(value));
    }
}

void ForwardList::print(std::ostream& os) const {  // O(n)
    os << *this;
}

void ForwardList::printReverse(std::ostream& os) const {  // O(n), рекурсия глубиной n
    os << "[";
    printReverse(os, head);
    os << "]";
}

void ForwardList::printReverse(std::ostream& os, const Node* node) {  // O(n)
    if (node == nullptr) return;

    printReverse(os, node->next);
    if (node->next != nullptr) {
        os << ", ";
    }
    os << node->value;
}

std::ostream& operator<<(std::ostream& os, const ForwardList& list) {  // O(n)
    os << "[";
    for (Node* cur = list.head; cur != nullptr; cur = cur->next) {
        os << cur->value;
        if (cur->next != nullptr) {
            os << ", ";
        }
    }
    os << "]";

    return os;
}
