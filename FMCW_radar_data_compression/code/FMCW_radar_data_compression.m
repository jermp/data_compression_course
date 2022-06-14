% Exam's project: FMCW radar data compression
% Course: A Crash Course on Data Compression
% Authors: Giulio Meucci, Francesco Mancuso
% 
% This MATLAB script compresses a .wav file using Linear Predictive Coding
% and Huffman Coding and compares the input and output file sizes by 
% evaluating the compression ratio.


clear all
close all
clc

%--------LOAD FILE--------%
[data,~] = audioread('cleanCorsa2_CUT.wav','native');
audioinfo('cleanCorsa2_CUT.wav')

tic

%--------LINEAR PREDICTIVE CODING--------%
FIRLEN = 100;
[FIRcoeffs,residual,~] = FMCW_LPC_analysis(data.',FIRLEN);

%--------SYMBOLS PROBABILITY ESTIMATION--------%
[probabilities, edges] = histcounts(residual,'Normalization','probability','BinMethod','integers');
nonZeroInd = find(probabilities > 0);
p = probabilities(nonZeroInd);
symbols = ceil(edges);
s = symbols(nonZeroInd);

%--------RESIDUAL HUFFMAN ENCODING--------%
dict = huffmandict(s,p);
bitstream = huffmanenco(residual,dict);

toc

%--------MEAN WORD LENGTH EVALUATION--------%
totaL = 0;
meanL = 0;
for ind2 = 1:size(dict,1)
    totaL = totaL + length(dict{ind2,2});
    meanL = meanL + p(ind2)*length(dict{ind2,2});
end
bitstream = dec2bin(bitstream);

%--------COMPARE FILES SIZE--------%
compressed_data_size = length(bitstream) + 64*FIRLEN + 16*size(dict,1) + totaL
original_data_size = 16*length(data)

compression_ratio = original_data_size/compressed_data_size