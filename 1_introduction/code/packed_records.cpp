#include <iostream>
#include <vector>

#include "bit_vector.hpp"

struct record {
    record(uint8_t w, uint8_t h, uint8_t d, uint8_t m, uint16_t y)
        : weight(w), height(h), day(d), month(m), year(y) {}

    uint8_t weight;
    uint8_t height;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

bool is_good(uint64_t got, uint64_t expected) {
    if (got != expected) {
        std::cout << "got " << got << " but expected " << expected << std::endl;
        return false;
    }
    return true;
}

int main() {
    constexpr uint64_t n = 10000;

    std::vector<record> records;  // declare a vector of records
    records.reserve(n);           // reserve space for n records

    unsigned seed = 3;  // fixed seed to reproduce the results
    std::srand(seed);   // initialize the pseudo-random generator

    // initilize at random the records
    for (uint64_t i = 0; i != n; ++i) {
        record r(std::rand() % 256,  // get a random value in [0,256)
                 std::rand() % 256,  // get a random value in [0,256)
                 std::rand() % 32,   // get a random value in [0,32)
                 std::rand() % 16,   // get a random value in [0,16)
                 std::rand() % 4096  // get a random value in [0,4096)
        );
        records.push_back(r);
    }

    bit_vector_builder builder;  // declare the bit_vector's builder
    builder.reserve(37 * n);     // reserve space for 37*n bits

    for (auto r : records) {
        builder.append_bits(r.weight, 8);
        builder.append_bits(r.height, 8);
        builder.append_bits(r.day, 5);
        builder.append_bits(r.month, 4);
        builder.append_bits(r.year, 12);
    }

    bit_vector packed_records;
    builder.build(packed_records);

    std::cout << "packed_records num_bits = " << packed_records.num_bits()
              << std::endl;
    std::cout << static_cast<double>(packed_records.num_bits()) / n
              << " bits/record" << std::endl;

    std::cout << "packed_records num_64bit_words = "
              << packed_records.num_64bit_words() << std::endl;
    std::cout << (packed_records.num_64bit_words() * 64.0) / n << " bits/record"
              << std::endl;

    /*
        Compare each uncompressed record with the corresponding packed one
        for equality to check if we encode/decode correctly.
    */
    for (uint64_t i = 0, pos = 0; i != n; ++i) {
        record r = records[i];

        uint64_t weight = packed_records.get_bits(pos, 8);
        if (!is_good(weight, r.weight)) return 1;
        pos += 8;

        uint64_t height = packed_records.get_bits(pos, 8);
        if (!is_good(height, r.height)) return 1;
        pos += 8;

        uint64_t day = packed_records.get_bits(pos, 5);
        if (!is_good(day, r.day)) return 1;
        pos += 5;

        uint64_t month = packed_records.get_bits(pos, 4);
        if (!is_good(month, r.month)) return 1;
        pos += 4;

        uint64_t year = packed_records.get_bits(pos, 12);
        if (!is_good(year, r.year)) return 1;
        pos += 12;
    }

    std::cout << "EVERYTHING OK!" << std::endl;

    return 0;
}
