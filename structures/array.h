#ifndef INC_2C_LABA1_ARRAY_H
#define INC_2C_LABA1_ARRAY_H
#include <string>
#include <ostream>
#include "container.h"

class Array : public Container {
public:
    Array();
    explicit Array(std::size_t n);
    Array(std::size_t n, const std::string& value);
    Array(std::initializer_list<std::string> init);

    Array(const Array& other);
    Array& operator=(const Array& other);

    ~Array();

private:
    void reserve(std::size_t newCap);
    void checkIndex(std::size_t index) const;

public:
    void pushBack(const std::string& value);
    void insert(std::size_t index, const std::string& value);
    std::string get(std::size_t index) const;
    void remove(std::size_t index);
    void set(std::size_t index, const std::string& value);
    std::size_t size() const;

public:
    char type() const override;
    std::string serialize() const override;
    void deserialize(const std::string& line) override;
    void print(std::ostream& os) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const Array& arr);

private:
    std::string* data;

    std::size_t sz;
    std::size_t cap;
};

#endif