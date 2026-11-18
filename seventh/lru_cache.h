#ifndef INC_2C_LABA2_LRU_CACHE_H
#define INC_2C_LABA2_LRU_CACHE_H
#include <cstddef>
#include <string>
#include "../structures/node.h"
#include "../structures/hash_table.h"

class LRUCache {
public:
    explicit LRUCache(std::size_t capacity);
    ~LRUCache();

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

public:
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);

    std::size_t size() const;
    std::size_t capacity() const;

private:
    struct Entry {
        std::string value;
        DNode* node = nullptr;
    };

    void pushFront(DNode* node);
    void unlink(DNode* node);

private:
    HashTable<Entry> table;
    DNode* head;
    DNode* tail;

    std::size_t cap;
    std::size_t sz;
};

#endif
