#ifndef INC_2C_LABA2_HASH_TABLE_H
#define INC_2C_LABA2_HASH_TABLE_H
#include <cstddef>
#include <cstdint>
#include <string>

enum class Probing { Linear, Double };

template <typename V>
class HashTable {
public:
    explicit HashTable(Probing probing = Probing::Double, std::size_t capacity = 11, double maxLoad = 0.7);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

public:
    bool insert(const std::string& key, const V& value);
    V* find(const std::string& key);
    bool remove(const std::string& key);

    std::size_t size() const;
    std::size_t capacity() const;
    double loadFactor() const;

    std::size_t probes() const;
    void resetProbes();

private:
    enum class State : unsigned char { Empty, Busy, Deleted };

    struct Cell {
        std::string key;
        V value{};
        State state = State::Empty;
    };

    static std::size_t hash1(const std::string& key);
    static std::size_t hash2(const std::string& key);
    static std::size_t nextPrime(std::size_t n);

    std::size_t step(const std::string& key) const;
    std::size_t locate(const std::string& key, bool& found) const;
    void resize(std::size_t newCapacity);

private:
    Cell* cells;
    Probing probing;
    double maxLoad;

    std::size_t cap;
    std::size_t sz;
    std::size_t used;
    mutable std::size_t probeCount;
};

template <typename V>
HashTable<V>::HashTable(Probing probing, std::size_t capacity, double maxLoad)
    : cells(nullptr), probing(probing), maxLoad(maxLoad), cap(nextPrime(capacity)), sz(0), used(0), probeCount(0) {
    cells = new Cell[cap];
}

template <typename V>
HashTable<V>::~HashTable() {
    delete[] cells;
}

template <typename V>
std::size_t HashTable<V>::hash1(const std::string& key) {  // FNV-1a, O(длины ключа)
    std::uint64_t h = 14695981039346656037u;
    for (unsigned char c : key) {
        h ^= c;
        h *= 1099511628211u;
    }
    return h;
}

template <typename V>
std::size_t HashTable<V>::hash2(const std::string& key) {  // djb2, O(длины ключа)
    std::uint64_t h = 5381;
    for (unsigned char c : key) {
        h = h * 33 + c;
    }
    return h;
}

template <typename V>
std::size_t HashTable<V>::nextPrime(std::size_t n) {
    if (n < 3) {
        return 3;
    }
    for (;; ++n) {
        bool prime = n % 2 != 0;
        for (std::size_t d = 3; prime && d * d <= n; d += 2) {
            prime = n % d != 0;
        }
        if (prime) {
            return n;
        }
    }
}

template <typename V>
std::size_t HashTable<V>::step(const std::string& key) const {  // O(1)
    if (probing == Probing::Linear) {
        return 1;
    }
    return 1 + hash2(key) % (cap - 1);
}

template <typename V>
std::size_t HashTable<V>::locate(const std::string& key, bool& found) const {  // O(1) в среднем
    std::size_t index = hash1(key) % cap;
    std::size_t delta = step(key);
    std::size_t firstDeleted = cap;
    found = false;

    for (std::size_t i = 0; i < cap; ++i) {
        ++probeCount;
        const Cell& cell = cells[index];
        if (cell.state == State::Empty) {
            return firstDeleted != cap ? firstDeleted : index;
        }
        if (cell.state == State::Busy && cell.key == key) {
            found = true;
            return index;
        }
        if (cell.state == State::Deleted && firstDeleted == cap) {
            firstDeleted = index;
        }
        index = (index + delta) % cap;
    }
    return firstDeleted;
}

template <typename V>
void HashTable<V>::resize(std::size_t newCapacity) {  // O(n)
    Cell* old = cells;
    std::size_t oldCap = cap;

    cap = nextPrime(newCapacity);
    cells = new Cell[cap];
    sz = 0;
    used = 0;

    for (std::size_t i = 0; i < oldCap; ++i) {
        if (old[i].state == State::Busy) {
            insert(old[i].key, old[i].value);
        }
    }
    delete[] old;
}

template <typename V>
bool HashTable<V>::insert(const std::string& key, const V& value) {  // O(1) в среднем
    if (used + 1 > cap * maxLoad) {
        resize(cap * 2);
    }

    bool found = false;
    std::size_t index = locate(key, found);
    Cell& cell = cells[index];
    if (found) {
        cell.value = value;
        return false;
    }

    if (cell.state == State::Empty) {
        ++used;
    }
    cell.key = key;
    cell.value = value;
    cell.state = State::Busy;
    ++sz;
    return true;
}

template <typename V>
V* HashTable<V>::find(const std::string& key) {  // O(1) в среднем
    bool found = false;
    std::size_t index = locate(key, found);
    return found ? &cells[index].value : nullptr;
}

template <typename V>
bool HashTable<V>::remove(const std::string& key) {  // O(1) в среднем
    bool found = false;
    std::size_t index = locate(key, found);
    if (!found) {
        return false;
    }

    cells[index].state = State::Deleted;
    cells[index].value = V{};
    --sz;
    return true;
}

template <typename V>
std::size_t HashTable<V>::size() const {  // O(1)
    return sz;
}

template <typename V>
std::size_t HashTable<V>::capacity() const {  // O(1)
    return cap;
}

template <typename V>
double HashTable<V>::loadFactor() const {  // O(1)
    return static_cast<double>(sz) / cap;
}

template <typename V>
std::size_t HashTable<V>::probes() const {
    return probeCount;
}

template <typename V>
void HashTable<V>::resetProbes() {
    probeCount = 0;
}

#endif
