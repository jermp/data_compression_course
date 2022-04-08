#include <iostream>
#include <fstream>

#include "integer_codes.hpp"
#include "../../1_introduction/code/bit_vector.hpp"

template <typename ReadFunction>
void check(std::string const& compressed_filename,
           std::string const& uncompressed_filename, ReadFunction read) {
    bit_vector bits;

    {
        std::ifstream in(compressed_filename.c_str(), std::ifstream::binary);
        if (!in.is_open()) {
            std::cout << "cannot open binary file for reading" << std::endl;
            return;
        }
        bits.load(in);
        in.close();
    }

    bit_vector_iterator it(bits);
    uint64_t num_lists = it.take(32);
    uint64_t num_ints = 0;

    std::ifstream in(uncompressed_filename.c_str());
    if (!in.is_open()) {
        std::cout << "cannot open file" << std::endl;
        return;
    }

    std::cout << "checking " << num_lists << " lists..." << std::endl;

    for (uint64_t i = 0; i != num_lists; ++i) {
        uint64_t list_size = it.take(32);

        uint64_t expected = 0;
        in >> expected;
        if (list_size != expected) {
            std::cout << "error: expected list_size " << expected << " but got "
                      << list_size << std::endl;
            in.close();
            return;
        }

        /* decode the gaps */
        uint32_t prev_x = 0;
        uint32_t x = 0;
        for (uint64_t i = 0; i != list_size; ++i) {
            uint32_t x = read(it) + prev_x;
            assert(x >= prev_x);
            prev_x = x;
            in >> expected;

            if (x != expected) {
                std::cout << "error: expected " << expected << " but got " << x
                          << std::endl;
                in.close();
                return;
            }
        }

        num_ints += list_size;
        std::cout << "list-" << i << " checked (" << num_ints
                  << " ints checked)" << std::endl;
    }

    std::cout << "checked " << num_ints << " ints" << std::endl;
    in.close();
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0]
                  << " [type] [compressed_filename] [uncompressed_filename]"
                  << std::endl;
        return 1;
    }

    std::string type = argv[1];
    std::string compressed_filename = argv[2];
    std::string uncompressed_filename = argv[3];

    if (type == "gamma") {
        check(compressed_filename, uncompressed_filename,
              [](bit_vector_iterator& it) { return read_gamma(it); });
    } else if (type == "delta") {
        check(compressed_filename, uncompressed_filename,
              [](bit_vector_iterator& it) { return read_delta(it); });
    } else if (type == "vbyte") {
        check(compressed_filename, uncompressed_filename,
              [](bit_vector_iterator& it) { return read_vbyte(it); });
    } else if (type == "rice_k1") {
        check(compressed_filename, uncompressed_filename,
              [](bit_vector_iterator& it) { return read_rice(it, 1); });
    } else if (type == "rice_k2") {
        check(compressed_filename, uncompressed_filename,
              [](bit_vector_iterator& it) { return read_rice(it, 2); });
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}