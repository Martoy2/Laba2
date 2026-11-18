#include "storage.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../structures/array.h"
#include "../structures/forward_list.h"
#include "../structures/doubly_list.h"
#include "../structures/stack.h"
#include "../structures/queue.h"
#include "../structures/avl_tree.h"
#include "../structures/hash_set.h"

Storage::Storage() : head(nullptr), tail(nullptr), sz(0) {}

Storage::~Storage() {
    clear();
}

Container* Storage::find(const std::string& name) const {
    for (Entry* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->name == name) {
            return cur->container;
        }
    }
    return nullptr;
}

void Storage::add(const std::string& name, Container* container) {
    if (find(name) != nullptr) {
        delete container;
        throw std::invalid_argument("Storage: name already exists");
    }

    Entry* entry = new Entry{name, container, nullptr};
    if (tail == nullptr) {
        head = tail = entry;
    } else {
        tail->next = entry;
        tail = entry;
    }
    ++sz;
}

void Storage::remove(const std::string& name) {
    Entry* prev = nullptr;
    Entry* cur = head;
    while (cur != nullptr && cur->name != name) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) {
        throw std::invalid_argument("Storage: name not found");
    }

    if (prev == nullptr) {
        head = cur->next;
    } else {
        prev->next = cur->next;
    }
    if (cur == tail) {
        tail = prev;
    }

    delete cur->container;
    delete cur;
    --sz;
}

std::size_t Storage::size() const {
    return sz;
}

void Storage::clear() {
    while (head != nullptr) {
        Entry* del = head;
        head = head->next;
        delete del->container;
        delete del;
    }
    tail = nullptr;
    sz = 0;
}

Container* Storage::create(char type) {
    switch (type) {
        case 'M': return new Array;
        case 'F': return new ForwardList;
        case 'L': return new DoublyList;
        case 'S': return new Stack;
        case 'Q': return new Queue;
        case 'T': return new AVLTree;
        case 'H': return new HashSet;
        default:  return nullptr;
    }
}

void Storage::load(const std::string& path) {
    clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (line.find_first_not_of(" \t\r") == std::string::npos) {
            continue;
        }

        std::istringstream in(line);
        std::string type;
        std::string name;
        if (!(in >> type >> name) || type.size() != 1) {
            throw std::runtime_error("Storage: bad line " + std::to_string(lineNumber));
        }

        Container* container = create(type[0]);
        if (container == nullptr) {
            throw std::runtime_error("Storage: unknown type on line " + std::to_string(lineNumber));
        }
        add(name, container);

        std::string data;
        std::getline(in, data);
        container->deserialize(data);
    }
}

void Storage::save(const std::string& path) const {
    std::string tmpPath = path + ".tmp";
    {
        std::ofstream file(tmpPath);
        if (!file.is_open()) {
            throw std::runtime_error("Storage: cannot open " + tmpPath);
        }

        for (Entry* cur = head; cur != nullptr; cur = cur->next) {
            file << cur->container->type() << ' ' << cur->name;
            std::string data = cur->container->serialize();
            if (!data.empty()) {
                file << ' ' << data;
            }
            file << '\n';
        }

        if (!file) {
            throw std::runtime_error("Storage: cannot write " + tmpPath);
        }
    }

    if (std::rename(tmpPath.c_str(), path.c_str()) != 0) {
        std::remove(tmpPath.c_str());
        throw std::runtime_error("Storage: cannot replace " + path);
    }
}
