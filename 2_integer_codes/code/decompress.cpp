#include <iostream>
#include <fstream>
#include <chrono>

#include "integer_codes.hpp"
#include "../../1_introduction/code/bit_vector.hpp"

template <typename ReadFunction>
void decompress(std::string const& input_filename, ReadFunction read) {
    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::microseconds duration_t;

    bit_vector bits;

    std::ifstream in(input_filename.c_str(), std::ifstream::binary);
    if (!in.is_open()) {
        std::cout << "cannot open binary file for reading" << std::endl;
        return;
    }
    bits.load(in);
    in.close();

    std::cout << "loaded " << bits.num_bits() << " bits" << std::endl;

    bit_vector_iterator it(bits);
    uint64_t num_lists = it.take(32);
    uint64_t num_ints = 0;

    std::cout << "decompressing " << num_lists << " lists..." << std::endl;
    uint64_t sum = 0;

    auto start = clock_t::now();

    for (uint64_t i = 0; i != num_lists; ++i) {
        uint64_t list_size = it.take(32);
        /* decode the gaps */
        uint32_t prev_x = 0;
        uint32_t x = 0;
        for (uint64_t i = 0; i != list_size; ++i) {
            uint32_t x = read(it) + prev_x;
            assert(x >= prev_x);
            prev_x = x;
            sum += x;
        }

        num_ints += list_size;
    }

    auto stop = clock_t::now();
    auto elapsed = std::chrono::duration_cast<duration_t>(stop - start);

    /* print sum to be sure we do not optimize away the loop */
    std::cout << "(ignore: " << sum << ")" << std::endl;

    std::cout << "decompressed " << num_ints << " integers in "
              << elapsed.count() << " microsecs" << std::endl;
    std::cout << "(" << (elapsed.count() * 1000.0) / num_ints << " ns/int)"
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [type] [input_filename]"
                  << std::endl;
        return 1;
    }

    std::string type = argv[1];
    std::string input_filename = argv[2];

    if (type == "gamma") {
        decompress(input_filename,
                   [](bit_vector_iterator& it) { return read_gamma(it); });
    } else if (type == "delta") {
        decompress(input_filename,
                   [](bit_vector_iterator& it) { return read_delta(it); });
    } else if (type == "vbyte") {
        decompress(input_filename,
                   [](bit_vector_iterator& it) { return read_vbyte(it); });
    } else if (type == "rice_k1") {
        decompress(input_filename,
                   [](bit_vector_iterator& it) { return read_rice(it, 1); });
    } else if (type == "rice_k2") {
        decompress(input_filename,
                   [](bit_vector_iterator& it) { return read_rice(it, 2); });
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}