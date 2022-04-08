#include <iostream>
#include <vector>
#include <algorithm>  // for std::sort
#include <chrono>     // for std::chrono::high_resolution_clock

struct large_record {
    large_record(uint64_t w, uint64_t h, uint64_t d, uint64_t m, uint64_t y)
        : weight(w), height(h), day(d), month(m), year(y) {}

    uint64_t weight;
    uint64_t height;
    uint64_t day;
    uint64_t month;
    uint64_t year;
};

struct small_record {
    small_record(uint8_t w, uint8_t h, uint8_t d, uint8_t m, uint16_t y)
        : weight(w), height(h), day(d), month(m), year(y) {}

    uint8_t weight;
    uint8_t height;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

int main(int argc, char** argv) {
    uint64_t vector_size = std::stoull(argv[1]);

    constexpr unsigned seed = 13;  // fixed seed to reproduce the results
    std::srand(seed);              // initialize the pseudo-random generator

    std::vector<large_record> large_records;
    std::vector<small_record> small_records;
    large_records.reserve(vector_size);
    small_records.reserve(vector_size);
    for (uint64_t i = 0; i != vector_size; ++i) {
        uint64_t weight = std::rand() % 256;
        uint64_t height = std::rand() % 256;
        uint64_t day = std::rand() % 32;
        uint64_t month = std::rand() % 16;
        uint64_t year = std::rand() % 4096;
        large_records.emplace_back(weight, height, day, month, year);
        small_records.emplace_back(weight, height, day, month, year);
    }

    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::milliseconds duration_t;
    {
        auto start = clock_t::now();
        std::sort(large_records.begin(), large_records.end(),
                  [](large_record const& x, large_record const& y) {
                      return x.day < y.day;
                  });
        auto stop = clock_t::now();
        auto elapsed = std::chrono::duration_cast<duration_t>(stop - start);
        std::cout << "sorting vec took: " << elapsed.count() << " millisecs"
                  << std::endl;
    }
    {
        auto start = clock_t::now();
        std::sort(small_records.begin(), small_records.end(),
                  [](small_record const& x, small_record const& y) {
                      return x.day < y.day;
                  });
        auto stop = clock_t::now();
        auto elapsed = std::chrono::duration_cast<duration_t>(stop - start);
        std::cout << "sorting vec took: " << elapsed.count() << " millisecs"
                  << std::endl;
    }

    return 0;
}