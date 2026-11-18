#include "hash_set.h"
#include <cstdint>
#include <sstream>
#include "serialization.h"

static const std::size_t INITIAL_BUCKETS = 8;

HashSet::HashSet() : buckets(new ForwardList[INITIAL_BUCKETS]), bucketCount(INITIAL_BUCKETS), sz(0) {}

HashSet::HashSet(std::initializer_list<std::string> init) : HashSet() {
    for (const std::string& value : init) {
        add(value);
    }
}

HashSet::HashSet(const HashSet& other)
    : buckets(new ForwardList[other.bucketCount]), bucketCount(other.bucketCount), sz(other.sz) {
    for (std::size_t i = 0; i < bucketCount; ++i) {
        buckets[i] = other.buckets[i];
    }
}

HashSet& HashSet::operator=(const HashSet& other) {
    if (this == &other) return *this;

    ForwardList* newBuckets = new ForwardList[other.bucketCount];
    for (std::size_t i = 0; i < other.bucketCount; ++i) {
        newBuckets[i] = other.buckets[i];
    }

    delete[] buckets;
    buckets = newBuckets;
    bucketCount = other.bucketCount;
    sz = other.sz;
    return *this;
}

HashSet::~HashSet() {
    delete[] buckets;
}

std::size_t HashSet::hash(const std::string& value) {  // O(длины строки)
    std::uint64_t h = 14695981039346656037u;
    for (unsigned char c : value) {
        h ^= c;
        h *= 1099511628211u;
    }
    return h;
}

std::size_t HashSet::bucketOf(const std::string& value) const {  // O(1)
    return hash(value) % bucketCount;
}

void HashSet::rehash(std::size_t newCount) {  // O(n)
    ForwardList* newBuckets = new ForwardList[newCount];
    for (std::size_t i = 0; i < bucketCount; ++i) {
        while (!buckets[i].empty()) {
            std::string value = buckets[i].front();
            buckets[i].popFront();
            newBuckets[hash(value) % newCount].pushFront(value);
        }
    }

    delete[] buckets;
    buckets = newBuckets;
    bucketCount = newCount;
}

bool HashSet::add(const std::string& value) {  // O(1) в среднем
    ForwardList& bucket = buckets[bucketOf(value)];
    if (bucket.find(value)) {
        return false;
    }

    bucket.pushFront(value);
    ++sz;
    if (sz * 4 > bucketCount * 3) {
        rehash(bucketCount * 2);
    }
    return true;
}

bool HashSet::remove(const std::string& value) {  // O(1) в среднем
    ForwardList& bucket = buckets[bucketOf(value)];
    if (!bucket.find(value)) {
        return false;
    }

    bucket.removeValue(value);
    --sz;
    return true;
}

bool HashSet::contains(const std::string& value) const {  // O(1) в среднем
    return buckets[bucketOf(value)].find(value);
}

std::size_t HashSet::size() const {  // O(1)
    return sz;
}

bool HashSet::empty() const {  // O(1)
    return sz == 0;
}

void HashSet::clear() {  // O(n)
    delete[] buckets;
    buckets = new ForwardList[INITIAL_BUCKETS];
    bucketCount = INITIAL_BUCKETS;
    sz = 0;
}

char HashSet::type() const {
    return 'H';
}

std::string HashSet::serialize() const {  // O(n)
    std::string line;
    for (std::size_t i = 0; i < bucketCount; ++i) {
        ForwardList bucket = buckets[i];
        while (!bucket.empty()) {
            if (!line.empty()) {
                line += ' ';
            }
            line += escapeValue(bucket.front());
            bucket.popFront();
        }
    }

    return line;
}

void HashSet::deserialize(const std::string& line) {  // O(n)
    clear();

    std::istringstream in(line);
    std::string value;
    while (in >> value) {
        add(unescapeValue(value));
    }
}

void HashSet::print(std::ostream& os) const {  // O(n)
    os << *this;
}

std::ostream& operator<<(std::ostream& os, const HashSet& set) {  // O(n)
    bool first = true;
    os << "{";
    for (std::size_t i = 0; i < set.bucketCount; ++i) {
        ForwardList bucket = set.buckets[i];
        while (!bucket.empty()) {
            if (!first) {
                os << ", ";
            }
            os << bucket.front();
            bucket.popFront();
            first = false;
        }
    }
    os << "}";

    return os;
}
