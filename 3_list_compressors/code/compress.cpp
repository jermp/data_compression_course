#include <iostream>
#include <fstream>

#include "elias_fano.hpp"
#include "interpolative.hpp"

template <typename CoDec>
void compress(std::string const& input_lists_filename,
              std::string const& output_filename) {
    std::ifstream in(input_lists_filename.c_str());
    if (!in.is_open()) {
        std::cout << "cannot open input file" << std::endl;
        return;
    }
    std::ofstream out(output_filename.c_str(), std::ofstream::binary);
    if (!out.is_open()) {
        std::cout << "cannot open binary file for writing" << std::endl;
        return;
    }

    CoDec codec;
    uint64_t num_ints = 0;
    uint64_t num_lists = 0;
    std::vector<uint32_t> list;

    while (!in.eof()) {
        uint64_t list_size = 0;
        in >> list_size;
        if (list_size > 0) {
            std::cout << "list-" << num_lists << " size = " << list_size
                      << std::endl;

            num_lists += 1;
            list.reserve(list_size);

            for (uint64_t i = 0; i != list_size; ++i) {
                uint32_t x = 0;
                in >> x;
                list.push_back(x);
            }

            codec.encode(list.data(), list.size());
            codec.save(out);

            num_ints += list_size;
            list.clear();
        }
    }
    in.close();

    std::cout << "compressed " << num_lists << " lists" << std::endl;
    std::cout << "(" << num_ints << " integers)" << std::endl;
    std::cout << "written " << out.tellp() * 8 << " bits" << std::endl;
    std::cout << "(" << (out.tellp() * 8.0) / num_ints << " bits/int)"
              << std::endl;

    out.close();
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0]
                  << " [type] [input_lists_filename] [output_filename]"
                  << std::endl;
        return 1;
    }

    std::string type = argv[1];
    std::string input_lists_filename = argv[2];
    std::string output_filename = argv[3];

    if (type == "ef") {
        compress<elias_fano>(input_lists_filename, output_filename);
    } else if (type == "bic") {
        compress<interpolative>(input_lists_filename, output_filename);
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}
