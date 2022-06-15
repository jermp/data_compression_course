# FMCW radar data compression

This folder contains the slide show PDF and the corresponding `.tex` source. Inside the code folder are stored all the `MATLAB` files used to generate the presented results.\
Our project focus on compressing FMCW radar data by exploiting the stationary nature of the analyzed signal. Indeed, the output of such systems is a linear combination of multiple sinusoidal signals and therefore it is by its nature a strongly periodic signal.\
For this reason, we first apply **Linear Predictive Coding** to reduce the dynamic of the signal and then, we use **Huffman coding** to further shrink the output file and improve the overall compression ratio. In order to avoid loss of data we save, along with the LPC FIR coefficient and the Huffman dictionary, also the **residuals** of the LPC compression algorithm. 

 ## References

[1] N. Levinson. “The Wiener (Root Mean Square) Error Criterion in Filter Design and
Prediction”. In: Journal of Mathematics and Physics 25.1-4 (1946), pp. 261–278. DOI:
https://doi.org/10.1002/sapm1946251261. \
\
[2] David A. Huffman. “A Method for the Construction of Minimum-Redundancy Codes”. In:
Proceedings of the IRE 40.9 (1952), pp. 1098–1101. DOI: 10.1109/JRPROC.1952.273898.\
\
[3] J. Makhoul. “Linear prediction: A tutorial review”. In: Proceedings of the IEEE 63.4 (1975),
pp. 561–580. DOI: 10.1109/PROC.1975.9792.\
\
[4] S.M. Kay and S.L. Marple. “Spectrum analysis—A modern perspective”. In: Proceedings of
the IEEE 69.11 (1981), pp. 1380–1419. DOI: 10.1109/PROC.1981.12184.\
\
[5] D. O’Shaughnessy. “Linear predictive coding”. In: IEEE Potentials 7.1 (1988), pp. 29–32. DOI:
10.1109/45.1890.

 ## Authors 
 Francesco Mancuso ([mandugo](https://github.com/mandugo))\
 Giulio Meucci ([Rustafun](https://github.com/Rustafun))
