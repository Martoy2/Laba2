#ifndef INC_2C_LABA2_HASH_SET_H
#define INC_2C_LABA2_HASH_SET_H
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include "forward_list.h"
#include "container.h"

class HashSet : public Container {
public:
    HashSet();
    HashSet(std::initializer_list<std::string> init);

    HashSet(const HashSet& other);
    HashSet& operator=(const HashSet& other);

    ~HashSet();

public:
    bool add(const std::string& value);
    bool remove(const std::string& value);
    bool contains(const std::string& value) const;

    std::size_t size() const;
    bool empty() const;
    void clear();

public:
    char type() const override;
    std::string serialize() const override;
    void deserialize(const std::string& line) override;
    void print(std::ostream& os) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const HashSet& set);

private:
    static std::size_t hash(const std::string& value);
    std::size_t bucketOf(const std::string& value) const;
    void rehash(std::size_t newCount);

private:
    ForwardList* buckets;

    std::size_t bucketCount;
    std::size_t sz;
};

#endif
