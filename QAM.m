M = 1024; % Modulation order (alphabet size or number of points in signal constellation)
k = log2(M); % Number of bits per symbol
n = 30000; % Number of bits to process
sps = 1; % Number of samples per symbol (oversampling factor)
EbNo =30;
rng default;
dataIn = randi([0 1],n,1); % Generate vector of binary data
dataInMatrix = reshape(dataIn,length(dataIn)/k,k);
dataSymbolsIn = bi2de(dataInMatrix);
figure; % Create new figure window.
stem(dataSymbolsIn(1:100));
title('Random Symbols');
xlabel('Symbol Index');
ylabel('Integer Value');
dataMod = qammod(dataSymbolsIn,M,'bin'); % Binary coding with phase offset of zero
snr = EbNo+10*log10(k)-10*log10(sps);
receivedSignal = awgn(dataMod,snr,'measured');
%receivedSignal = dataMod;
sPlotFig = scatterplot(receivedSignal,1,0,'r.');
hold on
scatterplot(dataMod,1,0,'mx',sPlotFig)
dataSymbolsOut = qamdemod(receivedSignal,M,'bin');
dataOutMatrix = de2bi(dataSymbolsOut,k);
dataOut = dataOutMatrix(:); % Return data in column vector
[numErrors,ber] = biterr(dataIn,dataOut);
fprintf('\nThe binary coding bit error rate is %5.2e, based on %d errors.\n', ...
    ber,numErrors)
figure;
subplot(2,1,1)
stem(dataIn(1:100),'filled');
title('Input Random Bits');
xlabel('Bit Index');
ylabel('Binary Value');
subplot(2,1,2)
stem(dataOut(1:100),'filled');
title('Output Bits');
xlabel('Bit Index');
ylabel('Binary Value');