#ifndef INC_2C_LABA1_CONTAINER_H
#define INC_2C_LABA1_CONTAINER_H
#include <ostream>
#include <string>

class Container {
public:
    virtual ~Container() = default;

public:
    virtual char type() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& line) = 0;
    virtual void print(std::ostream& os) const = 0;
};

#endif
