#include <iostream>
#include <fstream>
#include <chrono>

#include "elias_fano.hpp"
#include "interpolative.hpp"

template <typename CoDec>
void decompress(std::string const& input_filename) {
    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::microseconds duration_t;

    CoDec codec;

    std::ifstream in(input_filename.c_str(), std::ifstream::binary);
    if (!in.is_open()) {
        std::cout << "cannot open binary file for reading" << std::endl;
        return;
    }

    uint64_t num_lists = 0;
    uint64_t num_ints = 0;

    std::vector<uint32_t> list;  // to be decoded

    auto start = clock_t::now();

    while (true) {
        codec.load(in);
        if (in.eof()) break;
        uint32_t list_size = codec.size();
        list.resize(list_size);
        codec.decode(list.data());
        num_ints += list_size;
        num_lists += 1;
    }

    auto stop = clock_t::now();
    auto elapsed = std::chrono::duration_cast<duration_t>(stop - start);

    std::cout << "decompressed " << num_ints << " integers in "
              << elapsed.count() << " microsecs" << std::endl;
    std::cout << "(" << (elapsed.count() * 1000.0) / num_ints << " ns/int)"
              << std::endl;

    in.close();
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [type] [input_filename]"
                  << std::endl;
        return 1;
    }

    std::string type = argv[1];
    std::string input_filename = argv[2];

    if (type == "ef") {
        decompress<elias_fano>(input_filename);
    } else if (type == "bic") {
        decompress<interpolative>(input_filename);
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}