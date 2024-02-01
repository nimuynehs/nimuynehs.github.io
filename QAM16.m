M = 16; % Modulation order (alphabet size or number of points in signal constellation)
k = log2(M); % Number of bits per symbol
n = 30000; % Number of bits to process
sps = 1; % Number of samples per symbol (oversampling factor)
EbNo =10;
Tb=0.0001;
Ac1=1;
Ac2=3;
Ac3=5;
Ac4=7;
rng default;
dataIn = randi([0 1],n,1); % Generate vector of binary data
dataInMatrix = reshape(dataIn,length(dataIn)/k,k);
dataSymbolsIn = bi2de(dataInMatrix);
figure; % Create new figure window.
stem(dataSymbolsIn(1:10));
title('Random Symbols');
xlabel('Symbol Index');
ylabel('Integer Value');
dataMod = qammod(dataSymbolsIn,M); % Binary coding with phase offset of zero
snr = EbNo+10*log10(k)-10*log10(sps);
receivedSignal = awgn(dataMod,snr,'measured');
%receivedSignal = dataMod;
rsr=real(dataMod);
rsi=imag(dataMod);
rsr_bit=[]; 
nb=100; % bbit/bit
for n=1:1:7500   % 
    if rsr(n)==1;  % 
       rsr_bitt=ones(1,nb);
    elseif rsr(n)==3;
        rsr_bitt=ones(1,nb).*3;
    elseif rsr(n)==-1;
        rsr_bitt=ones(1,nb).*-1;
    elseif rsr(n)==-3;
        rsr_bitt=ones(1,nb).*-3;
    end
     rsr_bit=[rsr_bit rsr_bitt];
end
rsi_bit=[]; 
for n=1:1:7500   % 
    if rsi(n)==1;  % 
       rsi_bitt=ones(1,nb);
    elseif rsi(n)==3;
        rsi_bitt=ones(1,nb).*3;
    elseif rsi(n)==-1;
        rsi_bitt=ones(-1,nb).*-1;
    elseif rsi(n)==-3;
        rsi_bitt=ones(-3,nb).*-3;
    end
     rsi_bit=[rsi_bit rsi_bitt];
end
mc=10;  % fc>>fs fc=mc*fs fs=1/Tb 
fc=mc*(1/Tb); % carrier frequency 
t2=Tb/nb:Tb/nb:Tb;                 
t2L=length(t2);
rsr_mod=[];
for (i=1:1:10)
    if (rsr(i)==1)
        rsr_mod0=Ac1*cos(2*pi*fc*t2);%modulation signal with carrier signal 1
    elseif(rsr(i)==3)
        rsr_mod0=Ac2*cos(2*pi*fc*t2);%modulation signal with carrier signal 2
    elseif(rsr(i)==-1)
        rsr_mod0=-Ac1*cos(2*pi*fc*t2);%modulation signal with carrier signal 3
    elseif(rsr(i)==-3)
        rsr_mod0=-Ac2*cos(2*pi*fc*t2);%modulation signal with carrier signal 4
    end
    rsr_mod=[rsr_mod rsr_mod0];
end
rsi_mod=[];
for (i=1:1:10)
    if (rsi(i)==1)
        rsi_mod0=Ac1*sin(2*pi*fc*t2);%modulation signal with carrier signal 1
    elseif(rsi(i)==3)
        rsi_mod0=Ac2*sin(2*pi*fc*t2);%modulation signal with carrier signal 2
    elseif(rsi(i)==-1)
        rsi_mod0=-Ac1*sin(2*pi*fc*t2);%modulation signal with carrier signal 3
    elseif(rsi(i)==-3)
        rsi_mod0=-Ac2*sin(2*pi*fc*t2);%modulation signal with carrier signal 4
    end
    rsi_mod=[rsi_mod rsi_mod0];
end
rs=rsi_mod+rsr_mod;
figure;
plot(rs);
sPlotFig = scatterplot(receivedSignal,1,0,'r.');
hold on
scatterplot(dataMod,1,0,'mx',sPlotFig)
dataSymbolsOut = qamdemod(receivedSignal,M);
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