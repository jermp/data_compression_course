#include <iostream>
#include <fstream>
#include <chrono>

#include "elias_fano.hpp"
#include "interpolative.hpp"

template <typename CoDec>
void check(std::string const& compressed_filename,
           std::string const& uncompressed_filename) {
    CoDec codec;

    std::ifstream compressed_in(compressed_filename.c_str(),
                                std::ifstream::binary);
    if (!compressed_in.is_open()) {
        std::cout << "cannot open binary file for reading" << std::endl;
        return;
    }

    std::ifstream uncompressed_in(uncompressed_filename.c_str());
    if (!uncompressed_in.is_open()) {
        std::cout << "cannot open file" << std::endl;
        return;
    }

    uint64_t num_lists = 0;
    uint64_t num_ints = 0;
    std::vector<uint32_t> list;  // to be decoded

    while (true) {
        codec.load(compressed_in);
        if (compressed_in.eof()) break;

        uint32_t list_size = codec.size();
        uint64_t expected = 0;
        uncompressed_in >> expected;
        if (list_size != expected) {
            std::cout << "error: expected list_size " << expected << " but got "
                      << list_size << std::endl;
            compressed_in.close();
            uncompressed_in.close();
            return;
        }

        list.resize(list_size);
        codec.decode(list.data());

        for (uint64_t i = 0; i != list_size; ++i) {
            uncompressed_in >> expected;
            uint32_t x = list[i];
            if (x != expected) {
                std::cout << "error: expected " << expected << " but got " << x
                          << std::endl;
                compressed_in.close();
                uncompressed_in.close();
                return;
            }
        }

        num_ints += list_size;
        std::cout << "list-" << num_lists << " checked (" << num_ints
                  << " ints checked)" << std::endl;
        num_lists += 1;
    }

    std::cout << "checked " << num_ints << " ints" << std::endl;

    compressed_in.close();
    uncompressed_in.close();
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

    if (type == "ef") {
        check<elias_fano>(compressed_filename, uncompressed_filename);
    } else if (type == "bic") {
        check<interpolative>(compressed_filename, uncompressed_filename);
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}