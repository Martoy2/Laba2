#include "doubly_list.h"
#include <stdexcept>
#include <sstream>
#include "serialization.h"

DoublyList::DoublyList() : head(nullptr), tail(nullptr), sz(0) {}

DoublyList::DoublyList(std::initializer_list<std::string> init)
    : head(nullptr), tail(nullptr), sz(0) {
    for (const std::string& value : init) {
        pushBack(value);
    }
}

DoublyList::DoublyList(const DoublyList& other) : head(nullptr), tail(nullptr), sz(0) {
    for (DNode* cur = other.head; cur != nullptr; cur = cur->next) {
        pushBack(cur->value);
    }
}

DoublyList& DoublyList::operator=(const DoublyList& other) {
    if (this == &other) return *this;

    clear();
    for (DNode* cur = other.head; cur != nullptr; cur = cur->next) {
        pushBack(cur->value);
    }
    return *this;
}

DoublyList::~DoublyList() {
    clear();
}

DNode* DoublyList::findNode(const std::string& value) const {  // O(n) - перебор
    for (DNode* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->value == value) {
            return cur;
        }
    }
    return nullptr;
}

void DoublyList::unlink(DNode* node) {  // O(1)
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }

    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }

    delete node;
    --sz;
}

void DoublyList::pushFront(const std::string& value) {  // O(1)
    DNode* node = new DNode{value, nullptr, head};
    if (head != nullptr) {
        head->prev = node;
    } else {
        tail = node;
    }
    head = node;
    ++sz;
}

void DoublyList::pushBack(const std::string& value) {  // O(1)
    DNode* node = new DNode{value, tail, nullptr};
    if (tail != nullptr) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    ++sz;
}

void DoublyList::insertBefore(const std::string& target, const std::string& value) {  // O(n) - поиск, вставка O(1)
    DNode* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("DoublyList: element not found");
    }

    if (node == head) {
        pushFront(value);
        return;
    }

    DNode* newNode = new DNode{value, node->prev, node};
    node->prev->next = newNode;
    node->prev = newNode;
    ++sz;
}

void DoublyList::insertAfter(const std::string& target, const std::string& value) {  // O(n) - поиск, вставка O(1)
    DNode* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("DoublyList: element not found");
    }

    if (node == tail) {
        pushBack(value);
        return;
    }

    DNode* newNode = new DNode{value, node, node->next};
    node->next->prev = newNode;
    node->next = newNode;
    ++sz;
}

void DoublyList::popFront() {  // O(1)
    if (head == nullptr) {
        throw std::out_of_range("DoublyList: list is empty");
    }

    unlink(head);
}

void DoublyList::popBack() {  // O(1) - есть prev
    if (tail == nullptr) {
        throw std::out_of_range("DoublyList: list is empty");
    }

    unlink(tail);
}

void DoublyList::removeBefore(const std::string& target) {  // O(n) - поиск, удаление O(1)
    DNode* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("DoublyList: element not found");
    }
    if (node->prev == nullptr) {
        throw std::invalid_argument("DoublyList: no element before target");
    }

    unlink(node->prev);
}

void DoublyList::removeAfter(const std::string& target) {  // O(n) - поиск, удаление O(1)
    DNode* node = findNode(target);
    if (node == nullptr) {
        throw std::invalid_argument("DoublyList: element not found");
    }
    if (node->next == nullptr) {
        throw std::invalid_argument("DoublyList: no element after target");
    }

    unlink(node->next);
}

void DoublyList::removeValue(const std::string& value) {  // O(n) - поиск, удаление O(1)
    DNode* node = findNode(value);
    if (node == nullptr) {
        throw std::invalid_argument("DoublyList: element not found");
    }

    unlink(node);
}

std::string DoublyList::front() const {  // O(1)
    if (head == nullptr) {
        throw std::out_of_range("DoublyList: list is empty");
    }

    return head->value;
}

std::string DoublyList::back() const {  // O(1)
    if (tail == nullptr) {
        throw std::out_of_range("DoublyList: list is empty");
    }

    return tail->value;
}

bool DoublyList::find(const std::string& value) const {  // O(n)
    return findNode(value) != nullptr;
}

std::size_t DoublyList::size() const {  // O(1)
    return sz;
}

bool DoublyList::empty() const {  // O(1)
    return sz == 0;
}

void DoublyList::clear() {  // O(n)
    while (head != nullptr) {
        DNode* del = head;
        head = head->next;
        delete del;
    }
    tail = nullptr;
    sz = 0;
}

char DoublyList::type() const {
    return 'L';
}

std::string DoublyList::serialize() const {  // O(n)
    std::string line;
    for (DNode* cur = head; cur != nullptr; cur = cur->next) {
        line += escapeValue(cur->value);
        if (cur->next != nullptr) {
            line += ' ';
        }
    }

    return line;
}

void DoublyList::deserialize(const std::string& line) {  // O(n)
    clear();

    std::istringstream in(line);
    std::string value;
    while (in >> value) {
        pushBack(unescapeValue(value));
    }
}

void DoublyList::print(std::ostream& os) const {  // O(n)
    os << *this;
}

void DoublyList::printReverse(std::ostream& os) const {  // O(n)
    os << "[";
    for (DNode* cur = tail; cur != nullptr; cur = cur->prev) {
        os << cur->value;
        if (cur->prev != nullptr) {
            os << ", ";
        }
    }
    os << "]";
}

std::ostream& operator<<(std::ostream& os, const DoublyList& list) {  // O(n)
    os << "[";
    for (DNode* cur = list.head; cur != nullptr; cur = cur->next) {
        os << cur->value;
        if (cur->next != nullptr) {
            os << ", ";
        }
    }
    os << "]";

    return os;
}
