# FMCW radar data compression

<a href="https://imgbb.com/"><img src="https://i.ibb.co/w0hXx1q/made-with-matlab.png" alt="made-with-matlab" border="0"></a> 

- `FMCW_radar_data_compression.m`: this script evaluates the compression ratio achievable using **Linear Predictive Coding** and **Huffman Coding**, as explained in the slideshow, onto a `.wav` file. The file used is `cleanCorsa2_CUT.wav`. The length of the LPC FIR filter is 100. After the LPC filtering the histogram of the residuals is evaluated, by specifying `'BinMethod'` as `'integers'`, to use unit-width bins centered on integers, and `'Normalization'` as `'probability'`, to normalize the bin counts so that **sum(N)** is 1 (each bin count represents the probability that an observation falls within that bin). All the probabilities equal to zero are discarded, the Huffman dictionary is built and the symbols are encoded. 

- `mean_word_length_opt.m`

- `FMCW_LPC_analysis.m`

