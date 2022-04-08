How to compile and run the code
-

From a terminal window, move into this folder
and type the following commands.

To compile in a "debug" environment, define (`-D`) the `DEBUG` flag to enable all asserts:

    g++ -std=c++11 -DDEBUG compress.cpp -o compress
    g++ -std=c++11 -DDEBUG -mbmi2 -msse4.2 decompress.cpp -o decompress
    g++ -std=c++11 -DDEBUG -mbmi2 -msse4.2 check.cpp -o check
    

**NOTE.**
Note the two extra compiler flags `-mbmi2` and `-msse4.2` that are needed, respectively,
for the two hardware instructions `pdep` (parallel bit deposit) and `popcnt` (population count). Both these two special instructions are used in the implementation of the `select` query used by Elias-Fano' access algorithm.

To compile for maximum speed, disable all asserts (`-DNDEBUG`) and also use the optimization flags `-O3` and `-march=native`:

    g++ -std=c++11 -DNDEBUG -O3 -march=native compress.cpp -o compress 
    g++ -std=c++11 -DNDEBUG -O3 -mbmi2 -msse4.2 -march=native decompress.cpp -o decompress
    g++ -std=c++11 -DNDEBUG -O3 -mbmi2 -msse4.2 -march=native check.cpp -o check
    
Now, first unzip the file `lists.txt.gz` in the folder `2_integer_codes/code` which contains 10 sorted integer lists:

    gunzip ../../2_integer_codes/code/lists.txt.gz

Then use the program `./compress` to actually compress the lists.
The program expects the following arguments:

    Usage: ./compress [type] [input_lists_filename] [output_filename]

where `type` is one of the following: `ef` or `bic` for, respectively, Elias-Fano or Binary Interpolative Coding.

The script `run_all.sh` shows all the examples. To run it, use:

    bash run_all.sh


Micro benchmark
-

On a desktop Mac book pro (16-inch, 2019)
with a 2.6 GHz 6-Core Intel Core i7 processor, I got the following results
(compiling with optimization flags `-O3` and `-march=native`).

|**Code**|**bits/int**|**ns/int**|
|:-------|:----------:|:---------|
| ef  | 6.81       | 5.40     |
| bic | 2.61       | 6.60     |
