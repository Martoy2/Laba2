#ifndef INC_2C_LABA1_DOUBLY_LIST_H
#define INC_2C_LABA1_DOUBLY_LIST_H
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include "node.h"
#include "container.h"

class DoublyList : public Container {
public:
    DoublyList();
    DoublyList(std::initializer_list<std::string> init);

    DoublyList(const DoublyList& other);
    DoublyList& operator=(const DoublyList& other);

    ~DoublyList();

public:
    void pushFront(const std::string& value);
    void pushBack(const std::string& value);
    void insertBefore(const std::string& target, const std::string& value);
    void insertAfter(const std::string& target, const std::string& value);

    void popFront();
    void popBack();
    void removeBefore(const std::string& target);
    void removeAfter(const std::string& target);
    void removeValue(const std::string& value);

    std::string front() const;
    std::string back() const;
    bool find(const std::string& value) const;
    std::size_t size() const;
    bool empty() const;
    void clear();

    void printReverse(std::ostream& os) const;

public:
    char type() const override;
    std::string serialize() const override;
    void deserialize(const std::string& line) override;
    void print(std::ostream& os) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const DoublyList& list);

private:
    DNode* findNode(const std::string& value) const;
    void unlink(DNode* node);

private:
    DNode* head;
    DNode* tail;

    std::size_t sz;
};

#endif
