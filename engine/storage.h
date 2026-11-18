#ifndef INC_2C_LABA1_STORAGE_H
#define INC_2C_LABA1_STORAGE_H
#include <cstddef>
#include <string>
#include "../structures/container.h"

class Storage {
public:
    Storage();
    ~Storage();

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

public:
    Container* find(const std::string& name) const;
    void add(const std::string& name, Container* container);
    void remove(const std::string& name);

    std::size_t size() const;
    void clear();

    void load(const std::string& path);
    void save(const std::string& path) const;

    static Container* create(char type);

private:
    struct Entry {
        std::string name;
        Container* container;
        Entry* next;
    };

private:
    Entry* head;
    Entry* tail;

    std::size_t sz;
};

#endif
