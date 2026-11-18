#ifndef INC_2C_LABA1_FORWARD_LIST_H
#define INC_2C_LABA1_FORWARD_LIST_H
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include "node.h"
#include "container.h"

class ForwardList : public Container {
public:
    ForwardList();
    ForwardList(std::initializer_list<std::string> init);

    ForwardList(const ForwardList& other);
    ForwardList& operator=(const ForwardList& other);

    ~ForwardList();

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
    friend std::ostream& operator<<(std::ostream& os, const ForwardList& list);

private:
    Node* findNode(const std::string& value) const;
    static void printReverse(std::ostream& os, const Node* node);

private:
    Node* head;
    Node* tail;

    std::size_t sz;
};

#endif
