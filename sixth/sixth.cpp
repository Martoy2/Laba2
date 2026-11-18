#include "sixth.h"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>
#include <string>
#include "../structures/array.h"
#include "../structures/hash_table.h"
#include "../structures/utf8.h"

static const std::size_t TABLE_SIZE = 10007;
static const std::size_t MISS_QUERIES = 10000;

struct Measure {
    double hitProbes;
    double missProbes;
    double nsPerInsert;
};

static Measure measure(Probing probing, double alpha, const Array& keys, const Array& misses) {
    HashTable<std::size_t> table(probing, TABLE_SIZE, 1.0);
    std::size_t n = static_cast<std::size_t>(alpha * TABLE_SIZE);

    auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < n; ++i) {
        table.insert(keys.get(i), i);
    }
    auto end = std::chrono::steady_clock::now();

    table.resetProbes();
    for (std::size_t i = 0; i < n; ++i) {
        table.find(keys.get(i));
    }
    double hit = static_cast<double>(table.probes()) / n;

    table.resetProbes();
    for (std::size_t i = 0; i < misses.size(); ++i) {
        table.find(misses.get(i));
    }
    double miss = static_cast<double>(table.probes()) / misses.size();

    double ns = std::chrono::duration<double, std::nano>(end - start).count() / n;
    return {hit, miss, ns};
}

static int analyze() {
    std::mt19937_64 rng(2026);
    Array keys;
    Array misses;
    for (std::size_t i = 0; i < TABLE_SIZE; ++i) {
        keys.pushBack("k" + std::to_string(rng()));
    }
    for (std::size_t i = 0; i < MISS_QUERIES; ++i) {
        misses.pushBack("m" + std::to_string(rng()));
    }

    std::cout << "Table size " << TABLE_SIZE << ", average probes per search (theory in brackets)" << std::endl;
    std::cout << "alpha |      linear: hit        miss   ns/ins |      double: hit        miss   ns/ins" << std::endl;

    const double alphas[] = {0.1, 0.25, 0.5, 0.7, 0.8, 0.9, 0.95};
    for (double alpha : alphas) {
        Measure linear = measure(Probing::Linear, alpha, keys, misses);
        Measure dbl = measure(Probing::Double, alpha, keys, misses);

        double linearHit = 0.5 * (1 + 1 / (1 - alpha));
        double linearMiss = 0.5 * (1 + 1 / ((1 - alpha) * (1 - alpha)));
        double doubleHit = std::log(1 / (1 - alpha)) / alpha;
        double doubleMiss = 1 / (1 - alpha);

        std::printf("%5.2f | %5.2f (%5.2f) %6.2f (%6.2f) %6.0f | %5.2f (%5.2f) %6.2f (%6.2f) %6.0f\n",
                    alpha, linear.hitProbes, linearHit, linear.missProbes, linearMiss, linear.nsPerInsert,
                    dbl.hitProbes, doubleHit, dbl.missProbes, doubleMiss, dbl.nsPerInsert);
    }
    return 0;
}

static int longestSubstring() {  // O(n)
    std::string text;
    std::getline(std::cin, text);
    if (!text.empty() && text.back() == '\r') {
        text.pop_back();
    }

    Array symbols = splitUtf8(text);
    HashTable<std::size_t> last(Probing::Double);
    std::size_t left = 0;
    std::size_t best = 0;
    std::size_t bestStart = 0;

    for (std::size_t i = 0; i < symbols.size(); ++i) {
        std::size_t* seen = last.find(symbols.get(i));
        if (seen != nullptr && *seen >= left) {
            left = *seen + 1;
        }
        last.insert(symbols.get(i), i);

        if (i - left + 1 > best) {
            best = i - left + 1;
            bestStart = left;
        }
    }

    std::string substring;
    for (std::size_t i = bestStart; i < bestStart + best; ++i) {
        substring += symbols.get(i);
    }
    std::cout << best << " (" << substring << ")" << std::endl;
    return 0;
}

int runSixth(int argc, char* argv[]) {
    std::string mode = argc > 2 ? argv[2] : "";
    if (mode == "1") {
        return analyze();
    }
    if (mode == "2") {
        return longestSubstring();
    }

    std::cerr << "Usage: " << argv[0] << " 6 1   (analysis)  |  " << argv[0] << " 6 2   (longest substring)" << std::endl;
    return 1;
}
