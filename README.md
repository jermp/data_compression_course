A Crash Course on Data Compression
==================================

The need for storing data in compressed form is becoming more and more important
due to the ever-growing rate of data produced on a daily basis.
To keep up with this data deluge, data compression
is a mandatory step to deliver good quality of service in concrete applications.

In this introductory course you will learn about fundamental
data compression algorithms that are all widely adopted by
tools that we use every day, like filesystems, computer networks,
search engines, and databases.
These algorithms have now become indispensable knowledge across many fields in computing, including Information Retriveal, Machine Learning, Natural Language Processing, Applied Physics, and Bioinformatics.


### Syllabus

1. Introduction
	- What is and Why Data Compression?
	- Basic Model and Limit
	- Space vs. Time Trade-offs
	- Fundamental Question(s), Undecidability
	- Some Applications
	- Data, Information, Redundancy, Technological Limitations
	- Warmup: Bit-Packing, Bit-Wise Operations, Read/Write Binary Data, Run-Length Encoding

2. Integer Codes
	- The Static Integer Coding Problem
	- Unique Decodability
	- Unary, Binary, Minimal Binary, Gamma, Delta, Golomb, Rice, Exponential Golomb, Fibonacci, Variable-Byte
	- Effectiveness
	- Information Content, Entropy, Distributions
	- Zero- Minimum-Redundancy Codes, Kraft-McMillan Inequality
	
3. List Compressors
	- The Sorted List Coding Problem
	- Motivations
	- Combinatorial Lower Bound
	- Folklore Strategies
	- Blocking, PForDelta, Simple, Elias-Fano, partitioned Elias-Fano, Rank & Select on Bit-Vectors, Interpolative, Directly-Addressable, Hybrid Approaches
	- Performance

4. Statistical Compressors
	- The Statistical and Minimum-Redundancy Coding Problem
	- Assigning Canonical Prefix-Free Codewords
	- Shannon-Fano
	- Huffman, Canonical Huffman
	- Arithmetic Coding
	- Asymmetric Numeral Systems

5. Dictionary-based Compressors
	- The Dictionary-based Coding Problem
	- LZ77, LZSS, gzip, LZ78, LZW
	- Variants
	- Overview of Compressors

### Books

1. Robert Sedgewick and Kevin Wayne. 2011. *Algorithms*. Four-th edition. Addison-Wesley Professional, ISBN 0-321-57351-X.

2. David Salomon. 2007. *Variable-Length Codes for Data Compression*. Springer Science & Business Media, ISBN 978-1-84628-959-0.

3. Alistair Moffat and Andrew Turpin. 2002. *Compression and Coding Algorithms*. Springer Science & Business Media, ISBN 978-1-4615-0935-6.

4. Gonzalo Navarro. 2016. *Compact Data Structures*. Cambridge University Press, ISBN 978-1-107-15238-0.

### Survey Papers

1. G. E. P. and Rossano Venturini. 2020. *Techniques for Inverted Index Compression*. ACM Computing Surveys. 53, 6, Article 125 (November 2021), 36 pages. https://doi.org/10.1145/3415148

2. Alistair Moffat. 2019. *Huffman Coding*. ACM Computing Surveys. 52, 4, Article 85 (July 2020), 35 pages. https://doi.org/10.1145/3342555

### Creator

Giulio Ermanno Pibiri ([jermp](https://github.com/jermp))

### Contributors

- [tansy](https://github.com/jermp/data_compression_course/commits?author=tansy): adjustments to some scripts.
- [lisifra96](https://github.com/jermp/data_compression_course/commits?author=lisifra96): Kraft-McMillan inequality proof.

<!-- ### QR Codes

	qrencode "myurl" -o myurl.png -->

