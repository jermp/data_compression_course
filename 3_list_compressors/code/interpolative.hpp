#include <vector>
#include <cassert>

#include "../../1_introduction/code/bit_vector.hpp"
#include "../../2_integer_codes/code/integer_codes.hpp"

struct interpolative {
    interpolative() : m_size(0), m_universe(0) {}

    void encode(uint32_t const* input, uint64_t n) {
        if (n == 0) return;
        bit_vector_builder builder;
        m_size = n;
        m_universe = input[n - 1];
        encode(builder, input, m_size - 1, 0, m_universe);
        builder.build(m_bits);
    }

    void decode(uint32_t* output) const {
        bit_vector_iterator it(m_bits);
        output[m_size - 1] = m_universe;
        decode(it, output, m_size - 1, 0, m_universe);
    }

    uint32_t back() const {
        return m_universe;
    }

    uint32_t size() const {
        return m_size;
    }

    void save(std::ofstream& out) const {
        out.write(reinterpret_cast<char const*>(&m_size), sizeof(m_size));
        out.write(reinterpret_cast<char const*>(&m_universe),
                  sizeof(m_universe));
        m_bits.save(out);
    }

    void load(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&m_size), sizeof(m_size));
        in.read(reinterpret_cast<char*>(&m_universe), sizeof(m_universe));
        m_bits.load(in);
    }

private:
    uint32_t m_size;
    uint32_t m_universe;
    bit_vector m_bits;

    void encode(bit_vector_builder& builder, uint32_t const* input, uint32_t n,
                uint32_t lo, uint32_t hi) {
        if (n == 0) return;

        assert(lo <= hi);
        assert(hi - lo >= n - 1);

        if (hi - lo + 1 == n) return;  // run: stop recursion

        uint32_t m = n / 2;
        uint32_t x = input[m];
        write_binary(builder, x - lo - m, hi - lo - n + 1);

        encode(builder, input, m, lo, x - 1);
        encode(builder, input + m + 1, n - m - 1, x + 1, hi);
    }

    void decode(bit_vector_iterator& it, uint32_t* output, uint32_t n,
                uint32_t lo, uint32_t hi) const {
        if (n == 0) return;

        assert(lo <= hi);
        assert(hi - lo >= n - 1);

        if (hi - lo + 1 == n) {  // run: decode implicitly and stop recursion
            for (uint32_t i = 0; i != n; ++i) output[i] = lo++;
            return;
        }

        uint32_t m = n / 2;
        uint32_t x = read_binary(it, hi - lo - n + 1) + lo + m;
        output[m] = x;

        decode(it, output, m, lo, x - 1);
        decode(it, output + m + 1, n - m - 1, x + 1, hi);
    }
};
