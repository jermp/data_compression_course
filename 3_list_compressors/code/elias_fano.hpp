#pragma once

#include <cmath>  // for std::ceil and std::log2

#include "../../1_introduction/code/bit_vector.hpp"
#include "darray.hpp"  // for select_1 queries on bit_vector

struct elias_fano {
    elias_fano() : m_l(0), m_universe(0) {}

    void encode(uint32_t const* input, uint64_t n) {
        if (n == 0) return;
        uint32_t u = input[n - 1];
        m_universe = u;

        assert(u >= n);
        m_l = std::ceil(std::log2(static_cast<double>(u) / n));
        uint64_t num_clusters = (u >> m_l) + 1;

        bit_vector_builder bvb_high_bits;
        bvb_high_bits.resize(n + num_clusters);

        bit_vector_builder bvb_low_bits;
        bvb_low_bits.resize(n * m_l);

        uint64_t low_mask = (uint64_t(1) << m_l) - 1;
        for (size_t i = 0; i != n; ++i, ++input) {
            uint32_t x = *input;
            bvb_low_bits.set_bits(i * m_l, x & low_mask, m_l);
            bvb_high_bits.set_bits((x >> m_l) + i, 1, 1);
        }

        bvb_high_bits.build(m_high_bits);
        bvb_low_bits.build(m_low_bits);
        m_high_bits_darray.build(m_high_bits);
    }

    /* simple decoding using access
    (faster decode is possible using an iterator
    to cut the number of select queries)*/
    void decode(uint32_t* output) const {
        uint32_t list_size = size();
        for (uint64_t i = 0; i != list_size; ++i) {
            uint64_t x = access(i);
            *output = x;
            ++output;
        }
    }

    uint64_t access(uint64_t i) const {
        assert(i < size());
        uint64_t high = (m_high_bits_darray.select(m_high_bits, i) - i) << m_l;
        uint64_t low = m_low_bits.get_bits(i * m_l, m_l);
        return high | low;
    }

    uint32_t back() const {
        return m_universe;
    }

    uint32_t size() const {
        return m_low_bits.num_bits() / m_l;
    }

    void save(std::ofstream& out) const {
        out.write(reinterpret_cast<char const*>(&m_l), sizeof(m_l));
        out.write(reinterpret_cast<char const*>(&m_universe),
                  sizeof(m_universe));
        m_high_bits.save(out);
        m_low_bits.save(out);
        m_high_bits_darray.save(out);
    }

    void load(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&m_l), sizeof(m_l));
        in.read(reinterpret_cast<char*>(&m_universe), sizeof(m_universe));
        m_high_bits.load(in);
        m_low_bits.load(in);
        m_high_bits_darray.load(in);
    }

private:
    uint32_t m_l;
    uint32_t m_universe;
    bit_vector m_high_bits;
    bit_vector m_low_bits;
    darray m_high_bits_darray;
};
