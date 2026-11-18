#include "array.h"
#include <utility>
#include <stdexcept>
#include <sstream>
#include "serialization.h"

Array::Array() : data(nullptr), sz(0), cap(0) {}

Array::Array(std::size_t n) : Array(n, "") {}

Array::Array(std::size_t n, const std::string& value)
    : data(n > 0 ? new std::string[n] : nullptr), sz(n), cap(n) {
    for (std::size_t i = 0; i < n; ++i) {
        data[i] = value;
    }
}

Array::Array(std::initializer_list<std::string> init)
    : data(init.size() > 0 ? new std::string[init.size()] : nullptr),
      sz(init.size()), cap(init.size()) {
    std::size_t i = 0;
    for (const std::string& value : init) {
        data[i++] = value;
    }
}

Array::~Array() {
    delete[] data;
}

Array::Array(const Array& other)
    : data(other.cap > 0 ? new std::string[other.cap] : nullptr),
      sz(other.sz), cap(other.cap) {
    for (std::size_t i = 0; i < sz; ++i) {
        data[i] = other.data[i];
    }
}

Array& Array::operator=(const Array& other) {
    if (this == &other) return *this;

    std::string* newData = other.cap > 0 ? new std::string[other.cap] : nullptr;
    for (std::size_t i = 0; i < other.sz; ++i) {
        newData[i] = other.data[i];
    }

    delete[] data;
    data = newData;
    sz = other.sz;
    cap = other.cap;
    return *this;
}

void Array::reserve(std::size_t newCap) {  // O(n) - перенос всех элементов
    if (newCap <= cap) return;

    std::string* newData = new std::string[newCap];
    for (std::size_t i = 0; i < sz; ++i) {
        newData[i] = std::move(data[i]);
    }
    delete[] data;
    data = newData;
    cap = newCap;
}

void Array::checkIndex(std::size_t index) const {  // O(1)
    if (index >= sz) {
        throw std::out_of_range("Array: index out of range");
    }
}

void Array::pushBack(const std::string& value) {  // O(1) амортиз., O(n) при расширении
    if (sz == cap) {
        reserve(cap == 0 ? 4 : cap * 2);
    }
    data[sz] = value;
    ++sz;
}

void Array::insert(std::size_t index, const std::string& value) {  // O(n) - сдвиг вправо
    if (index > sz) {
        throw std::out_of_range("Array: index out of range");
    }

    if (sz == cap) {
        reserve(cap == 0 ? 4 : cap * 2);
    }

    for (std::size_t i = sz; i > index; --i) {
        data[i] = std::move(data[i - 1]);
    }
    data[index] = value;
    ++sz;
}

std::string Array::get(std::size_t index) const {  // O(1)
    checkIndex(index);

    return data[index];
}

void Array::remove(std::size_t index) {  // O(n) - сдвиг влево
    checkIndex(index);

    for (std::size_t i = index; i + 1 < sz; ++i) {
        data[i] = std::move(data[i + 1]);
    }
    --sz;
    data[sz].clear();
}

void Array::set(std::size_t index, const std::string& value) {  // O(1)
    checkIndex(index);

    data[index] = value;
}

std::size_t Array::size() const {  // O(1)
    return sz;
}

char Array::type() const {
    return 'M';
}

std::string Array::serialize() const {  // O(n)
    std::string line;
    for (std::size_t i = 0; i < sz; ++i) {
        line += escapeValue(data[i]);
        if (i + 1 < sz) {
            line += ' ';
        }
    }

    return line;
}

void Array::deserialize(const std::string& line) {  // O(n)
    for (std::size_t i = 0; i < sz; ++i) {
        data[i].clear();
    }
    sz = 0;

    std::istringstream in(line);
    std::string value;
    while (in >> value) {
        pushBack(unescapeValue(value));
    }
}

void Array::print(std::ostream& os) const {  // O(n)
    os << *this;
}


std::ostream& operator<<(std::ostream& os, const Array& arr) {  // O(n)
    os << "[";
    for (std::size_t i = 0; i < arr.sz; ++i) {
        os << arr.data[i];
        if (i + 1 < arr.sz) {
            os << ", ";
        }
    }
    os << "]";

    return os;
}