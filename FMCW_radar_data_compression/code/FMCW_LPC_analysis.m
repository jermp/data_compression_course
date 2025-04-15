function [FIRcoeffs,residual,residual_variance] = FMCW_LPC_analysis(data,FIRLEN)
    %
    % Linear Predictive Coding analysis for FMCW radar signal
    %
    % USAGE: [FIRcoeffs,residual,residual_variance] = FMCW_LPC_analysis(data,FIRLEN)
    %
    % Exam's project: FMCW radar data compression
    % Course: A Crash Course on Data Compression
    % Authors: Giulio Meucci, Francesco Mancuso

    signal = double(data);
    [FIRcoeffs,residual_variance] = lpc(signal,FIRLEN);
    est_s = int16(filter([0 -FIRcoeffs(2:end)],1,signal));
    residual = data - est_s;

end