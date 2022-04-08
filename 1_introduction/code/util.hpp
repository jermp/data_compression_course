#pragma once

#include <immintrin.h>  // for _mm_popcnt_u64

/* return the number of 64-bit words for num_bits */
static uint64_t num_64bit_words_for(uint64_t num_bits) {
    return (num_bits + 64 - 1) / 64;
}

/*
    Good reference for built-in functions:
    http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
*/

/* position of the most significant bit (msb) */
static uint32_t msb(uint32_t x) {
    assert(x > 0);                 // if x is 0, the result is undefined
    return 31 - __builtin_clz(x);  // count leading zeros (clz)
}

/* position of the least significant bit (lsb) */
static uint32_t lsb(uint32_t x) {
    assert(x > 0);            // if x is 0, the result is undefined
    return __builtin_ctz(x);  // count trailing zeros (ctz)
}
/* long long version */
static uint64_t lsbll(uint64_t x) {
    assert(x > 0);              // if x is 0, the result is undefined
    return __builtin_ctzll(x);  // count trailing zeros (ctz)
}

template <typename T>
void save_vector(std::ofstream& out, std::vector<T> const& vec) {
    uint64_t vec_size = vec.size();
    out.write(reinterpret_cast<char const*>(&vec_size), sizeof(vec_size));
    out.write(reinterpret_cast<char const*>(vec.data()), vec_size * sizeof(T));
}

template <typename T>
void load_vector(std::ifstream& in, std::vector<T>& vec) {
    uint64_t vec_size = 0;
    in.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
    vec.resize(vec_size);
    in.read(reinterpret_cast<char*>(vec.data()), vec_size * sizeof(T));
}

inline uint64_t popcount(uint64_t x) {
    return static_cast<uint64_t>(_mm_popcnt_u64(x));
}

inline uint64_t select_in_word(const uint64_t x, const uint64_t k) {
    assert(k < popcount(x));
    return _tzcnt_u64(_pdep_u64(1ULL << k, x));
}