#include "lru_cache.h"
#include <stdexcept>

LRUCache::LRUCache(std::size_t capacity)
    : table(Probing::Double), head(nullptr), tail(nullptr), cap(capacity), sz(0) {
    if (capacity == 0) {
        throw std::invalid_argument("LRUCache: capacity must be positive");
    }
}

LRUCache::~LRUCache() {
    while (head != nullptr) {
        DNode* del = head;
        head = head->next;
        delete del;
    }
}

void LRUCache::pushFront(DNode* node) {  // O(1)
    node->prev = nullptr;
    node->next = head;
    if (head != nullptr) {
        head->prev = node;
    } else {
        tail = node;
    }
    head = node;
}

void LRUCache::unlink(DNode* node) {  // O(1)
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
}

void LRUCache::set(const std::string& key, const std::string& value) {  // O(1) в среднем
    Entry* entry = table.find(key);
    if (entry != nullptr) {
        entry->value = value;
        unlink(entry->node);
        pushFront(entry->node);
        return;
    }

    if (sz == cap) {
        DNode* oldest = tail;
        unlink(oldest);
        table.remove(oldest->value);
        delete oldest;
        --sz;
    }

    DNode* node = new DNode{key, nullptr, nullptr};
    pushFront(node);
    table.insert(key, Entry{value, node});
    ++sz;
}

bool LRUCache::get(const std::string& key, std::string& value) {  // O(1) в среднем
    Entry* entry = table.find(key);
    if (entry == nullptr) {
        return false;
    }

    unlink(entry->node);
    pushFront(entry->node);
    value = entry->value;
    return true;
}

std::size_t LRUCache::size() const {  // O(1)
    return sz;
}

std::size_t LRUCache::capacity() const {  // O(1)
    return cap;
}
