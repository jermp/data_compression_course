#pragma once

#include "../../1_introduction/code/bit_vector.hpp"
#include "../../1_introduction/code/util.hpp"

struct darray {
    darray() : m_positions(0) {}

    void build(bit_vector const& bv) {
        std::vector<uint64_t> const& data = bv.data();
        std::vector<uint64_t> cur_block_positions;
        std::vector<int64_t> block_inventory;
        std::vector<uint16_t> subblock_inventory;
        std::vector<uint64_t> overflow_positions;

        for (size_t word_idx = 0; word_idx < data.size(); ++word_idx) {
            size_t cur_pos = word_idx * 64;
            uint64_t cur_word = data[word_idx];
            while (true) {
                if (cur_word == 0) break;
                uint64_t l = lsbll(cur_word);
                cur_pos += l;
                cur_word >>= l;
                if (cur_pos >= bv.num_bits()) break;

                cur_block_positions.push_back(cur_pos);

                if (cur_block_positions.size() == block_size) {
                    flush_cur_block(cur_block_positions, block_inventory,
                                    subblock_inventory, overflow_positions);
                }

                // can't do >>= l + 1, can be 64
                cur_word >>= 1;
                cur_pos += 1;
                m_positions += 1;
            }
        }
        if (cur_block_positions.size()) {
            flush_cur_block(cur_block_positions, block_inventory,
                            subblock_inventory, overflow_positions);
        }
        m_block_inventory.swap(block_inventory);
        m_subblock_inventory.swap(subblock_inventory);
        m_overflow_positions.swap(overflow_positions);
    }

    inline uint64_t select(bit_vector const& bv, uint64_t idx) const {
        assert(idx < num_positions());
        uint64_t block = idx / block_size;
        int64_t block_pos = m_block_inventory[block];
        if (block_pos < 0) {  // sparse super-block
            uint64_t overflow_pos = uint64_t(-block_pos - 1);
            return m_overflow_positions[overflow_pos +
                                        (idx & (block_size - 1))];
        }

        size_t subblock = idx / subblock_size;
        size_t start_pos = uint64_t(block_pos) + m_subblock_inventory[subblock];
        size_t reminder = idx & (subblock_size - 1);
        if (!reminder) return start_pos;

        std::vector<uint64_t> const& data = bv.data();
        size_t word_idx = start_pos >> 6;
        size_t word_shift = start_pos & 63;
        uint64_t word = data[word_idx] & (uint64_t(-1) << word_shift);
        while (true) {
            size_t popcnt = popcount(word);
            if (reminder < popcnt) break;
            reminder -= popcnt;
            word = data[++word_idx];
        }
        return (word_idx << 6) + select_in_word(word, reminder);
    }

    inline uint64_t num_positions() const {
        return m_positions;
    }

    void save(std::ofstream& out) const {
        out.write(reinterpret_cast<char const*>(&m_positions),
                  sizeof(m_positions));
        save_vector(out, m_block_inventory);
        save_vector(out, m_subblock_inventory);
        save_vector(out, m_overflow_positions);
    }

    void load(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&m_positions), sizeof(m_positions));
        load_vector(in, m_block_inventory);
        load_vector(in, m_subblock_inventory);
        load_vector(in, m_overflow_positions);
    }

protected:
    static void flush_cur_block(std::vector<uint64_t>& cur_block_positions,
                                std::vector<int64_t>& block_inventory,
                                std::vector<uint16_t>& subblock_inventory,
                                std::vector<uint64_t>& overflow_positions) {
        if (cur_block_positions.back() - cur_block_positions.front() <
            max_in_block_distance) {
            block_inventory.push_back(int64_t(cur_block_positions.front()));
            for (size_t i = 0; i < cur_block_positions.size();
                 i += subblock_size) {
                subblock_inventory.push_back(uint16_t(
                    cur_block_positions[i] - cur_block_positions.front()));
            }
        } else {
            block_inventory.push_back(-int64_t(overflow_positions.size()) - 1);
            for (size_t i = 0; i < cur_block_positions.size(); ++i) {
                overflow_positions.push_back(cur_block_positions[i]);
            }
            for (size_t i = 0; i < cur_block_positions.size();
                 i += subblock_size) {
                subblock_inventory.push_back(uint16_t(-1));
            }
        }
        cur_block_positions.clear();
    }

    static const size_t block_size = 1024;  // 2048
    static const size_t subblock_size = 32;
    static const size_t max_in_block_distance = 1 << 16;

    size_t m_positions;
    std::vector<int64_t> m_block_inventory;
    std::vector<uint16_t> m_subblock_inventory;
    std::vector<uint64_t> m_overflow_positions;
};
