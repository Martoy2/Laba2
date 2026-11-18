#include "third.h"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include "../structures/array.h"
#include "../structures/hash_set.h"

static bool parseNatural(const std::string& token, std::int64_t& value) {
    if (token.empty() || token.size() > 15) {
        return false;
    }

    value = 0;
    for (char c : token) {
        if (c < '0' || c > '9') {
            return false;
        }
        value = value * 10 + (c - '0');
    }
    return value > 0;
}

static bool place(const std::int64_t* nums, const int* order, int* group, std::int64_t* sums,
                  int n, int k, std::int64_t target, int i) {  // O(k^n) в худшем случае
    if (i == n) {
        return true;
    }

    int current = order[i];
    for (int b = 0; b < k; ++b) {
        if (sums[b] + nums[current] > target) {
            continue;
        }

        sums[b] += nums[current];
        group[current] = b;
        if (place(nums, order, group, sums, n, k, target, i + 1)) {
            return true;
        }
        sums[b] -= nums[current];

        if (sums[b] == 0) {
            break;
        }
    }
    return false;
}

int runThird() {
    std::string line;
    std::getline(std::cin, line);

    HashSet set;
    Array elements;
    std::istringstream in(line);
    std::string token;
    while (in >> token) {
        std::int64_t value = 0;
        if (!parseNatural(token, value)) {
            std::cerr << "error: not a natural number: " << token << std::endl;
            return 1;
        }
        if (set.add(std::to_string(value))) {
            elements.pushBack(std::to_string(value));
        }
    }

    std::int64_t target = 0;
    std::getline(std::cin, line);
    std::istringstream targetIn(line);
    if (!(targetIn >> token) || !parseNatural(token, target)) {
        std::cerr << "error: expected subset sum" << std::endl;
        return 1;
    }

    int n = static_cast<int>(elements.size());
    if (n == 0) {
        std::cerr << "error: empty set" << std::endl;
        return 1;
    }

    std::int64_t* nums = new std::int64_t[n];
    int* order = new int[n];
    int* group = new int[n];
    std::int64_t total = 0;
    std::int64_t maxValue = 0;
    for (int i = 0; i < n; ++i) {
        parseNatural(elements.get(i), nums[i]);
        order[i] = i;
        total += nums[i];
        if (nums[i] > maxValue) {
            maxValue = nums[i];
        }
    }

    for (int i = 1; i < n; ++i) {
        int current = order[i];
        int j = i - 1;
        while (j >= 0 && nums[order[j]] < nums[current]) {
            order[j + 1] = order[j];
            --j;
        }
        order[j + 1] = current;
    }

    bool ok = total % target == 0 && maxValue <= target;
    int k = ok ? static_cast<int>(total / target) : 0;
    std::int64_t* sums = new std::int64_t[k > 0 ? k : 1]();
    if (ok) {
        ok = place(nums, order, group, sums, n, k, target, 0);
    }

    if (!ok) {
        std::cout << "Cannot split into subsets with sum " << target << std::endl;
    } else {
        std::cout << "Sum " << target << ", subsets: " << k << std::endl;
        for (int b = 0; b < k; ++b) {
            bool first = true;
            std::cout << "{";
            for (int i = 0; i < n; ++i) {
                if (group[i] != b) {
                    continue;
                }
                if (!first) {
                    std::cout << ", ";
                }
                std::cout << nums[i];
                first = false;
            }
            std::cout << "}" << std::endl;
        }
    }

    delete[] nums;
    delete[] order;
    delete[] group;
    delete[] sums;
    return 0;
}
