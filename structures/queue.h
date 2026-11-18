#ifndef INC_2C_LABA1_QUEUE_H
#define INC_2C_LABA1_QUEUE_H
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include "forward_list.h"
#include "container.h"

class Queue : public Container {
public:
    Queue();
    Queue(std::initializer_list<std::string> init);

public:
    void push(const std::string& value);
    std::string pop();
    std::string front() const;

    std::size_t size() const;
    bool empty() const;
    void clear();

public:
    char type() const override;
    std::string serialize() const override;
    void deserialize(const std::string& line) override;
    void print(std::ostream& os) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const Queue& queue);

private:
    ForwardList list;
};

#endif
