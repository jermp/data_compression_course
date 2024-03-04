% Exam's project: FMCW radar data compression
% Course: A Crash Course on Data Compression
% Authors: Giulio Meucci, Francesco Mancuso
% 
% This MATLAB script compresses a .wav file using Linear Predictive Coding
% and Huffman Coding. It changes the length of the FIR filter iteration by
% iteration in order to find the optimum value that minimize the average
% codeword length.
%
% WARNING: it has a quite long execution time.

clear all
close all
clc

%--------LOAD FILE--------%
[data,~] = audioread('cleanCorsa2_CUT.wav','native');
audioinfo('cleanCorsa2_CUT.wav')

%--------AVERAGE CODEWORD LENGTH EVALUATION--------%
maxLength = 150;
FIRLEN = 1:maxLength;

meanL = zeros(1,maxLength);
totaL = zeros(1,maxLength);

f = waitbar(0, 'Starting');

for ind = 1:maxLength
    [~,residual,~] = FMCW_LPC_analysis(data.',ind);

    [probabilities, edges] = histcounts(residual,'Normalization','probability','BinMethod','integers');
    nonZeroInd = find(probabilities > 0);
    p = probabilities(nonZeroInd);
    symbols = ceil(edges);
    s = symbols(nonZeroInd);

    dict = huffmandict(s,p);
    bitstream = huffmanenco(residual,dict);

    totaL(ind) = 0;
    meanL(ind) = 0;
    for ind2 = 1:size(dict,1)
        totaL(ind) = totaL(ind) + length(dict{ind2,2});
        meanL(ind) = meanL(ind) + p(ind2)*length(dict{ind2,2});
    end

    waitbar(ind/maxLength, f, sprintf('Progress: %d %%', floor(ind/maxLength*100)));
end

figure(1)
plot(FIRLEN,meanL,'LineWidth',2)
ylabel('bits')
xlabel('FIR length')
title('Average Codeword Length')
grid on

