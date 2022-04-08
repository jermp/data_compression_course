#pragma once

#include <cassert>
#include <vector>

#include "util.hpp"

struct bit_vector {
    bit_vector() : m_num_bits(0) {}

    uint64_t num_bits() const {
        return m_num_bits;
    }

    uint64_t num_64bit_words() const {
        return m_bits.size();
    }

    uint64_t get_bits(uint64_t pos, uint64_t len) const {
        assert(pos + len <= num_bits());

        if (!len) return 0;

        uint64_t block = pos / 64;
        uint64_t shift = pos % 64;
        uint64_t mask = -(len == 64) | ((1ULL << len) - 1);

        if (shift + len <= 64) return m_bits[block] >> shift & mask;

        return (m_bits[block] >> shift) |
               (m_bits[block + 1] << (64 - shift) & mask);
    }

    uint64_t get_word64(uint64_t pos) const {
        assert(pos < num_bits());

        uint64_t block = pos / 64;
        uint64_t shift = pos % 64;
        uint64_t word = m_bits[block] >> shift;

        if (shift and block + 1 < m_bits.size()) {
            word |= m_bits[block + 1] << (64 - shift);
        }

        return word;
    }

    void save(std::ofstream& out) const {
        out.write(reinterpret_cast<char const*>(&m_num_bits),
                  sizeof(m_num_bits));
        out.write(reinterpret_cast<char const*>(m_bits.data()),
                  m_bits.size() * sizeof(uint64_t));
    }

    void load(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&m_num_bits), sizeof(m_num_bits));
        m_bits.resize(num_64bit_words_for(m_num_bits));
        in.read(reinterpret_cast<char*>(m_bits.data()),
                m_bits.size() * sizeof(uint64_t));
    }

    std::vector<uint64_t> const& data() const {
        return m_bits;
    }

    friend struct bit_vector_builder;

private:
    uint64_t m_num_bits;
    std::vector<uint64_t> m_bits;
};

struct bit_vector_builder {
    bit_vector_builder() {
        resize(0);
    }

    uint64_t num_bits() const {
        return m_num_bits;
    }

    void resize(uint64_t num_bits) {
        m_num_bits = num_bits;
        m_bits.resize(num_64bit_words_for(num_bits), 0);
    }

    void reserve(uint64_t num_bits) {
        m_bits.reserve(num_64bit_words_for(num_bits));
    }

    void build(bit_vector& bv) {
        std::swap(m_num_bits, bv.m_num_bits);
        m_bits.swap(bv.m_bits);
    }

    void set_bits(uint64_t pos, uint64_t x, uint64_t len) {
        assert(pos + len <= num_bits());
        assert(len == 64 or (x >> len) == 0);
        if (len == 0) return;

        uint64_t mask = (len == 64) ? uint64_t(-1) : ((uint64_t(1) << len) - 1);
        uint64_t word = pos / 64;
        uint64_t pos_in_word = pos % 64;

        m_bits[word] &= ~(mask << pos_in_word);
        m_bits[word] |= x << pos_in_word;

        uint64_t stored = 64 - pos_in_word;
        if (stored < len) {
            m_bits[word + 1] &= ~(mask >> stored);
            m_bits[word + 1] |= x >> stored;
        }
    }

    void append_bits(uint64_t x, uint64_t len) {
        assert(len <= 64);
        if (x >> len) {
            std::cout << "--> x=" << x << " len=" << len
                      << " (x>>len)=" << (x >> len) << std::endl;
        }
        assert(len == 64 or (x >> len) == 0);  // no other bits must be set
        if (len == 0) return;

        uint64_t pos_in_word = m_num_bits % 64;
        m_num_bits += len;
        if (pos_in_word == 0) {
            m_bits.push_back(x);
        } else {
            *m_cur_word |= x << pos_in_word;
            if (len > 64 - pos_in_word) {
                m_bits.push_back(x >> (64 - pos_in_word));
            }
        }

        m_cur_word = &m_bits.back();
    }

private:
    uint64_t m_num_bits;
    std::vector<uint64_t> m_bits;
    uint64_t* m_cur_word;
};
