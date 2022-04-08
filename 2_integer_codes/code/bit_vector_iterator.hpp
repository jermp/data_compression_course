#pragma once

#include "../../1_introduction/code/bit_vector.hpp"

struct bit_vector_iterator {
    bit_vector_iterator() : m_bv(nullptr) {}

    bit_vector_iterator(bit_vector const& bv, uint64_t pos = 0) : m_bv(&bv) {
        at(pos);
    }

    void at(uint64_t pos) {
        m_pos = pos;
        m_buf = 0;
        m_avail = 0;
    }

    /* return 1 byte assuming position is aligned to a 8-bit boundary  */
    inline uint64_t take_one_byte() {
        assert(m_pos % 8 == 0);
        if (m_avail == 0) fill_buf();
        uint64_t val = m_buf & 255;
        m_buf >>= 8;
        m_avail -= 8;
        m_pos += 8;
        return val;
    }

    /* return the next l bits from the current position and advance by l bits */
    inline uint64_t take(uint64_t l) {
        assert(l <= 64);
        if (m_avail < l) fill_buf();
        uint64_t val;
        if (l != 64) {
            val = m_buf & ((uint64_t(1) << l) - 1);
            m_buf >>= l;
        } else {
            val = m_buf;
        }
        m_avail -= l;
        m_pos += l;
        return val;
    }

    /* skip all zeros from the current position and
    return the number of skipped zeros */
    inline uint64_t skip_zeros() {
        uint64_t zeros = 0;
        while (m_buf == 0) {
            m_pos += m_avail;
            zeros += m_avail;
            fill_buf();
        }

        uint64_t l = lsb(m_buf);
        m_buf >>= l;
        m_buf >>= 1;
        m_avail -= l + 1;
        m_pos += l + 1;
        return zeros + l;
    }

    inline uint64_t position() const {
        return m_pos;
    }

private:
    inline void fill_buf() {
        m_buf = m_bv->get_word64(m_pos);
        m_avail = 64;
    }

    bit_vector const* m_bv;
    uint64_t m_pos;
    uint64_t m_buf;
    uint64_t m_avail;
};
