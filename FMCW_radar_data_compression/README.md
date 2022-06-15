# FMCW radar data compression

This folder contains the slide show PDF and the corresponding `.tex` source. Inside the Code folder are stored all the `MATLAB` files used to generate the presented results.\
Our project focus on compressing FMCW radar data by exploiting the stationary nature of the analyzed signal. Indeed, the output of such systems is a linear combination of multiple sinusoidal signals and therefore it is by its nature a strongly periodic signal.__
First, we apply **Linear Predictive Coding** to reduce the dynamic of the signal, then we use **Huffman coding** to further shrink the output file and improve the overall compression ratio.
