#pragma once

#include <cassert>

#include "../../1_introduction/code/bit_vector.hpp"
#include "bit_vector_iterator.hpp"

/* Unary */
void write_unary(bit_vector_builder& builder, uint64_t x) {
    assert(x < 64);
    uint64_t u = uint64_t(1) << x;
    builder.append_bits(u, x + 1);
}
uint64_t read_unary(bit_vector_iterator& it) {
    return it.skip_zeros();
}
/***/

/* Binary */
/* write the integer x <= r using b=ceil(log2(r+1)) bits */
void write_binary(bit_vector_builder& builder, uint32_t x, uint32_t r) {
    assert(r > 0);
    assert(x <= r);
    uint32_t b = msb(r) + 1;
    builder.append_bits(x, b);
}
/* read b=ceil(log2(r+1)) bits and interprets them as the integer x */
uint32_t read_binary(bit_vector_iterator& it, uint32_t r) {
    assert(r > 0);
    uint32_t b = msb(r) + 1;
    uint32_t x = it.take(b);
    assert(x <= r);
    return x;
}
/***/

/* Gamma */
void write_gamma(bit_vector_builder& builder, uint64_t x) {
    uint64_t xx = x + 1;
    uint64_t b = msb(xx);
    write_unary(builder, b);
    uint64_t mask = (uint64_t(1) << b) - 1;
    builder.append_bits(xx & mask, b);
}
uint64_t read_gamma(bit_vector_iterator& it) {
    uint64_t b = read_unary(it);
    return (it.take(b) | (uint64_t(1) << b)) - 1;
}
/***/

/* Delta */
void write_delta(bit_vector_builder& builder, uint64_t x) {
    uint64_t xx = x + 1;
    uint64_t b = msb(xx);
    write_gamma(builder, b);
    uint64_t mask = (uint64_t(1) << b) - 1;
    builder.append_bits(xx & mask, b);
}
uint64_t read_delta(bit_vector_iterator& it) {
    uint64_t b = read_gamma(it);
    return (it.take(b) | (uint64_t(1) << b)) - 1;
}
/***/

/* Rice */
void write_rice(bit_vector_builder& builder, uint64_t x, const uint64_t k) {
    assert(k > 0);
    uint64_t q = x >> k;
    uint64_t r = x - (q << k);
    write_gamma(builder, q);
    builder.append_bits(r, k);
}
uint64_t read_rice(bit_vector_iterator& it, const uint64_t k) {
    assert(k > 0);
    uint64_t q = read_gamma(it);
    uint64_t r = it.take(k);
    return r + (q << k);
}
/***/

/* Variable-Byte */
void write_vbyte(bit_vector_builder& builder, uint64_t x) {
    if (x < 128) {
        builder.append_bits(x, 8);
        return;
    }
    uint8_t data_bits = x & 127;
    builder.append_bits(data_bits | 128, 8);
    write_vbyte(builder, x >> 7);
}
uint64_t read_vbyte(bit_vector_iterator& it) {
    uint64_t val = 0;
    for (uint64_t shift = 0;; shift += 7) {
        uint8_t byte = it.take_one_byte();
        val += (byte & 127) << shift;
        if (byte < 128) break;
    }
    return val;
}
/***/