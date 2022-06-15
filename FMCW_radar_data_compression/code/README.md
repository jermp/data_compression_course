# FMCW radar data compression code

<a href="https://imgbb.com/"><img src="https://i.ibb.co/w0hXx1q/made-with-matlab.png" alt="made-with-matlab" border="0"></a> 

- `FMCW_radar_data_compression.m`: this script evaluates the compression ratio achievable using [**Linear Predictive Coding**](https://en.wikipedia.org/wiki/Linear_predictive_coding) and [**Huffman Coding**](https://en.wikipedia.org/wiki/Huffman_coding), as explained in the slideshow, onto a `.wav` file. The file used is `cleanCorsa2_CUT.wav`. The length of the LPC FIR filter is 100. After the LPC filtering the histogram of the residuals is evaluated by specifying: `'BinMethod'` as `'integers'` to set the unit-width bins centered on integers, and `'Normalization'` as `'probability'` to obtain the relative frequencies of the symbols (i.e. each bin count represents the probability that an observation falls within that bin). All the probabilities equal to zero are discarded, the Huffman dictionary is built and the symbols are encoded. The last lines of the script calculate the **average codeword length** and the **compression ratio**;
- `average_codeword_length_opt.m`: this script has been used to analyse the compression performance by varying the length of the LPC FIR filter. The analysis has been carried out using a number of coeffients that goes from 1 to 150. The core of the script is the same as the previous one;
  > :warning: **The execution takes a lot** :warning: 
- `FMCW_LPC_analysis.m`: this function is used in the above scripts. It does the estimation of the LPC FIR filter coefficients, it filters the input signal and gives in output the filter coefficients, the residual signal and the variance of the residual signal.

  > We used the variance of the residual signal as a quality metric during the analysis phase to understand how the LPC was affecting the amplitude of the signal.

