How to compile and run the code
-

From a terminal window, move into this folder
and type the following commands.

To compile in a "debug" environment, define (`-D`) the `DEBUG` flag to enable all asserts:

	g++ -std=c++11 -DDEBUG compress.cpp -o compress 
	g++ -std=c++11 -DDEBUG decompress.cpp -o decompress
	g++ -std=c++11 -DDEBUG check.cpp -o check
	
To compile for maximum speed, disable all asserts (`-DNDEBUG`) and also use the optimization flags `-O3` and `-march=native`:

	g++ -std=c++11 -DNDEBUG -O3 -march=native compress.cpp -o compress 
	g++ -std=c++11 -DNDEBUG -O3 -march=native decompress.cpp -o decompress
	g++ -std=c++11 -DNDEBUG -O3 -march=native check.cpp -o check
	
Now, first unzip the file `lists.txt.gz` which contains 10 sorted integer lists:

	gunzip lists.txt.gz

Then use the program `./compress` to actually compress the lists.
The program expects the following arguments:

	Usage: ./compress [type] [input_lists_filename] [output_filename]

where `type` is one of the following: `gamma`, `delta`, `vbyte`, `rice_k1`, or `rice_k2`.	

Below, some examples with type `gamma`.

	./compress gamma lists.txt out_gamma.bin
	./decompress gamma out_gamma.bin
	./check gamma out_gamma.bin lists.txt

The script `run_all.sh` shows all the examples. To run it, use:

	bash run_all.sh


Micro benchmark
-

On a desktop Mac book pro (16-inch, 2019)
with a 2.6 GHz 6-Core Intel Core i7 processor, I got the following results
(compiling with optimization flags `-O3` and `-march=native`).

|**Code**|**bits/int**|**ns/int**|
|:-------|:----------:|:---------|
| gamma  | 4.24       | 3.89     |
| delta  | 4.96       | 4.84     |
| rice k=1  | 3.49       | 4.77     |
| rice k=2  | 3.89       | 4.77     |
| vbyte  | 8.11       | 0.95     |