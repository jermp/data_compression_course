#!/bin/sh

gunzip -k ../../2_integer_codes/code/lists.txt.gz

# g++ -std=c++11 -DDEBUG compress.cpp -o compress
# g++ -std=c++11 -DDEBUG -mbmi2 -msse4.2 decompress.cpp -o decompress
# g++ -std=c++11 -DDEBUG -mbmi2 -msse4.2 check.cpp -o check

g++ -std=c++11 -DNDEBUG -O3 -march=native compress.cpp -o compress
g++ -std=c++11 -DNDEBUG -O3 -mbmi2 -msse4.2 -march=native decompress.cpp -o decompress
g++ -std=c++11 -DNDEBUG -O3 -mbmi2 -msse4.2 -march=native check.cpp -o check

./compress ef ../../2_integer_codes/code/lists.txt out_ef.bin
./check ef out_ef.bin ../../2_integer_codes/code/lists.txt
./decompress ef out_ef.bin
rm out_ef.bin

./compress bic ../../2_integer_codes/code/lists.txt out_bic.bin
./check bic out_bic.bin ../../2_integer_codes/code/lists.txt
./decompress bic out_bic.bin
rm out_bic.bin

rm ../../2_integer_codes/code/lists.txt
