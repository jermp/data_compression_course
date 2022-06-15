# FMCW radar data compression

This folder contains the slide show PDF and the corresponding `.tex` source. Inside the code folder are stored all the `MATLAB` files used to generate the presented results.\
Our project focus on compressing FMCW radar data by exploiting the stationary nature of the analyzed signal. Indeed, the output of such systems is a linear combination of multiple sinusoidal signals and therefore it is by its nature a strongly periodic signal.\
For this reason, we first apply **Linear Predictive Coding** to reduce the dynamic of the signal and then, we use **Huffman coding** to further shrink the output file and improve the overall compression ratio. In order to avoid loss of data we save, along with the LPC FIR coefficient and the Huffman dictionary, also the **residuals** of the LPC compression algorithm. 

 ## Authors 
 Francesco Mancuso ([mandugo](https://github.com/mandugo))\
 Giulio Meucci ([Rustafun](https://github.com/Rustafun))
