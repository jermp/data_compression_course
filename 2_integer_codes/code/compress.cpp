#include <iostream>
#include <fstream>

#include "integer_codes.hpp"
#include "../../1_introduction/code/bit_vector.hpp"

template <typename WriteFunction>
void compress(std::string const& input_lists_filename,
              std::string const& output_filename, WriteFunction write) {
    bit_vector_builder builder;
    builder.append_bits(0, 32);  // reserve the first 32-bit int for num_lists

    uint64_t num_ints = 0;
    uint64_t num_lists = 0;

    std::ifstream in(input_lists_filename.c_str());
    if (!in.is_open()) {
        std::cout << "cannot open input file" << std::endl;
        return;
    }
    while (!in.eof()) {
        uint64_t list_size = 0;
        in >> list_size;
        if (list_size > 0) {
            std::cout << "list-" << num_lists << " size = " << list_size
                      << std::endl;

            num_lists += 1;

            /* write list_size on 32 bits*/
            builder.append_bits(list_size, 32);

            /* encode the gaps */
            uint32_t prev_x = 0;
            uint32_t x = 0;
            for (uint64_t i = 0; i != list_size; ++i) {
                in >> x;
                assert(x >= prev_x);
                write(builder, x - prev_x);
                prev_x = x;
            }

            num_ints += list_size;
        }
    }
    in.close();

    builder.set_bits(0, num_lists, 32);

    std::cout << "compressed " << num_lists << " lists" << std::endl;
    std::cout << "(" << num_ints << " integers)" << std::endl;
    std::cout << "written " << builder.num_bits() << " bits" << std::endl;
    std::cout << "(" << static_cast<double>(builder.num_bits()) / num_ints
              << " bits/int)" << std::endl;

    bit_vector bits;
    builder.build(bits);

    std::ofstream out(output_filename.c_str(), std::ofstream::binary);
    if (!out.is_open()) {
        std::cout << "cannot open binary file for writing" << std::endl;
        return;
    }
    bits.save(out);
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

    if (type == "gamma") {
        compress(input_lists_filename, output_filename,
                 [](bit_vector_builder& builder, uint32_t x) {
                     write_gamma(builder, x);
                 });
    } else if (type == "delta") {
        compress(input_lists_filename, output_filename,
                 [](bit_vector_builder& builder, uint32_t x) {
                     write_delta(builder, x);
                 });
    } else if (type == "vbyte") {
        compress(input_lists_filename, output_filename,
                 [](bit_vector_builder& builder, uint32_t x) {
                     write_vbyte(builder, x);
                 });
    } else if (type == "rice_k1") {
        compress(input_lists_filename, output_filename,
                 [](bit_vector_builder& builder, uint32_t x) {
                     write_rice(builder, x, 1);
                 });
    } else if (type == "rice_k2") {
        compress(input_lists_filename, output_filename,
                 [](bit_vector_builder& builder, uint32_t x) {
                     write_rice(builder, x, 2);
                 });
    } else {
        std::cout << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}