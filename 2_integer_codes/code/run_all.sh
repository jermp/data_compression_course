#!/bin/sh

gunzip -k lists.txt.gz

# g++ -std=c++11 -DDEBUG compress.cpp -o compress
# g++ -std=c++11 -DDEBUG decompress.cpp -o decompress
# g++ -std=c++11 -DDEBUG check.cpp -o check

g++ -std=c++11 -DNDEBUG -O3 -march=native compress.cpp -o compress
g++ -std=c++11 -DNDEBUG -O3 -march=native decompress.cpp -o decompress
g++ -std=c++11 -DNDEBUG -O3 -march=native check.cpp -o check

./compress gamma lists.txt out_gamma.bin
./check gamma out_gamma.bin lists.txt
./decompress gamma out_gamma.bin
rm out_gamma.bin

./compress delta lists.txt out_delta.bin
./check delta out_delta.bin lists.txt
./decompress delta out_delta.bin
rm out_delta.bin

./compress rice_k1 lists.txt out_rice_k1.bin
./check rice_k1 out_rice_k1.bin lists.txt
./decompress rice_k1 out_rice_k1.bin
rm out_rice_k1.bin

./compress rice_k2 lists.txt out_rice_k2.bin
./check rice_k2 out_rice_k2.bin lists.txt
./decompress rice_k2 out_rice_k2.bin
rm out_rice_k2.bin

./compress vbyte lists.txt out_vbyte.bin
./check vbyte out_vbyte.bin lists.txt
./decompress vbyte out_vbyte.bin
rm out_vbyte.bin

rm lists.txt
