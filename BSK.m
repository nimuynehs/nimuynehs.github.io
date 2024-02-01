% ********************* BFSK modulation and de-modulation ****************%

clc;
clear all;
close all;


% ********************* Define transmitted signal *************************


x_inp = input('Enter Digital Input Information = ');  % binary signal 0 or 1 % message to be transmitted                               
N = length(x_inp);
Tb=0.0001; % bit period (second)   
snr=100;

% ********************* Represent input signal as digital signal ****
x_bit=[]; 
nb=100; % bbit/bit
for n=1:1:N   % 
    if x_inp(n)==1;  % 
       x_bitt=ones(1,nb);
    else x_inp(n)==0;
        x_bitt=zeros(1,nb);
    end
     x_bit=[x_bit x_bitt];
end

t1=Tb/nb:Tb/nb:nb*N*(Tb/nb); % time of the signal 
f1 = figure(1);
set(f1,'color',[1 1 1]);
subplot(4,3,1);
plot(t1,x_bit,'lineWidth',2);grid on;
axis([ 0 Tb*N -0.5 1.5]);
ylabel('Amplitude(volt)');
xlabel(' Time(sec)');
title('Input signal as digital signal');
subplot(4,3,3);
stem(x_inp,'filled');
ylabel('Binary value');
xlabel('Bit index');
title('Input bits');

% ********************* Define BFSK Modulation ****************************
Ac=5;  % Amplitude of carrier signal
mc1=16;  % fc>>fs fc=mc*fs fs=1/Tb
mc2=4;
fc1=mc1*(1/Tb); % carrier frequency for bit 1
fc2=mc2*(1/Tb); % carrier frequency for bit 0
t2=Tb/nb:Tb/nb:Tb;                 
t2L=length(t2);
xf_mod=[];
for (i=1:1:N)
    if (x_inp(i)==1)
        xf_mod0=Ac*cos(2*pi*fc1*t2);%modulation signal with carrier signal 1
    else
        xf_mod0=Ac*cos(2*pi*fc2*t2);%modulation signal with carrier signal 2
    end
    xf_mod=[xf_mod xf_mod0];
end
t3=Tb/nb:Tb/nb:Tb*N;

% ********************* Transmitted signal x ******************************
xf=xf_mod;

% ********************* Channel model h and w *****************************
h=1; % Fading 
w = awgn(xf_mod,snr,'measured')-xf_mod; % Noise

% ********************* Received signal y *********************************
yf=h.*xf+w;
subplot(4,3,4)
plot(t3,yf);
xlabel('Time(sec)');
ylabel('Amplitude(volt)');
title('Signal of  BFSK modulation ');

% ********************* Define BFSK Demodulation **************************
yf_dem=[];
for n=t2L:t2L:length(yf)
  t=Tb/nb:Tb/nb:Tb;
  c_dem1=cos(2*pi*fc1*t); % carrier signal for information 1
  c_dem2=cos(2*pi*fc2*t); % carrier signal for information 0
  yf_dem1=c_dem1.*yf((n-(t2L-1)):n);
  yf_dem2=c_dem2.*yf((n-(t2L-1)):n);
  t4=Tb/nb:Tb/nb:Tb;
  z1=trapz(t4,yf_dem1);  % integration 
  z2=trapz(t4,yf_dem2);  % integration 
  A_dem1=round(2*z1/Tb);
  A_dem2= round(2*z2/Tb);
  if(A_dem1>Ac/2)      % % logic level = (Ac)/2
    a=1;
  else(A_dem2>Ac/2)
    a=0;
  end
  yf_dem=[yf_dem a];
end

xf_out=yf_dem; % output signal;


% *************** Represent output signal as digital signal ***************
xxf_bit=[];
for n=1:length(xf_out);
    if xf_out(n)==1;
       xxf_bitt=ones(1,nb);
    else xf_out(n)==0;
        xxf_bitt=zeros(1,nb);
    end
     xxf_bit=[xxf_bit xxf_bitt];

end
t4=Tb/nb:Tb/nb:nb*length(xf_out)*(Tb/nb);
subplot(4,3,7)
plot(t4,xxf_bit,'LineWidth',2);grid on;
axis([ 0 Tb*length(xf_out) -0.5 1.5]);
ylabel('Amplitude(volt)');
xlabel(' Time(sec)');
title('Output signal as digital signal');
subplot(4,3,10)
stem(xf_out,'filled')
ylabel('Binary value');
xlabel('Bit index');
title('Output bits');


% ********************* Define BASK Modulation ****************************
Ac1=15; % Amplitude of carrier signal for bit 1
Ac2=10; % Amplitude of carrier signal for bit 0
mc=10;  % fc>>fs fc=mc*fs fs=1/Tb 
fc=mc*(1/Tb); % carrier frequency 
t2=Tb/nb:Tb/nb:Tb;                 
t2L=length(t2);
xa_mod=[];
for (i=1:1:N)
    if (x_inp(i)==1)
        xa_mod0=Ac1*cos(2*pi*fc*t2);%modulation signal with carrier signal 1
    else
        xa_mod0=Ac2*cos(2*pi*fc*t2);%modulation signal with carrier signal 2
    end
    xa_mod=[xa_mod xa_mod0];
end
t3=Tb/nb:Tb/nb:Tb*N;

% ********************* Transmitted signal x ******************************
xa=xa_mod;

% ********************* Channel model h and w *****************************
 h=1;
% EbNo = 100;
% w = awgn(0,EbNo,'measured'); % Noise
% ********************* Received signal y *********************************
ya=h.*xa+w;
subplot(4,3,5)
plot(t3,ya);
xlabel('Time(sec)');
ylabel('Amplitude(volt)');
title('Signal of  BASK modulation ');

% ********************* Define BASK Demodulation **************************

ya_dem=[];
for n=t2L:t2L:length(ya)
  t=Tb/nb:Tb/nb:Tb;
  c=cos(2*pi*fc*t); % carrier signal 
  ya_dem0=c.*ya((n-(t2L-1)):n);
  t4=Tb/nb:Tb/nb:Tb;
  z=trapz(t4,ya_dem0); % integration 
  A_dem=round((2*z/Tb));                                     
  if(A_dem>((Ac1+Ac2)/2)) % logic level = (Ac1+Ac2)/2
    A=1;
  else
    A=0;
  end
  ya_dem=[ya_dem A];
end

xa_out=ya_dem; % output signal;

% *************** Represent output signal as digital signal ***************
xxa_bit=[];
for n=1:length(xa_out);
    if xa_out(n)==1;
       xx_bitt=ones(1,nb);
    else xa_out(n)==0;
        xx_bitt=zeros(1,nb);
    end
     xxa_bit=[xxa_bit xx_bitt];

end
t4=Tb/nb:Tb/nb:nb*length(xa_out)*(Tb/nb);
subplot(4,3,8)
plot(t4,xxa_bit,'LineWidth',2);grid on;
axis([ 0 Tb*length(xa_out) -0.5 1.5]);
ylabel('Amplitude(volt)');
xlabel(' Time(sec)');
title('Output signal as digital signal');
subplot(4,3,11)
stem(xa_out,'filled')
ylabel('Binary value');
xlabel('Bit index');
title('Output bits');

% ********************* Define BPSK Modulation ****************************
Ac=5;  % Amplitude of carrier signal
mc=4;  % fc>>fs fc=mc*fs fs=1/Tb
fc=mc*(1/Tb); % carrier frequency for bit 1
fi1=0; % carrier phase for bit 1
fi2=pi; % carrier phase for bit 0
t2=Tb/nb:Tb/nb:Tb;                 
t2L=length(t2);
xp_mod=[];
for (i=1:1:N)
    if (x_inp(i)==1)
        xp_mod0=Ac*cos(2*pi*fc*t2+fi1);%modulation signal with carrier signal 1
    else
        xp_mod0=Ac*cos(2*pi*fc*t2+fi2);%modulation signal with carrier signal 2
    end
    xp_mod=[xp_mod xp_mod0];
end
t3=Tb/nb:Tb/nb:Tb*N;


% ********************* Transmitted signal x ******************************
xp=xp_mod;
% 
% % ********************* Channel model h and w *****************************
% h=1; % Fading 
% w=0; % Noise
% 
% % ********************* Received signal y *********************************
yp=h.*xp+w;
subplot(4,3,6)
plot(t3,yp);
xlabel('Time(sec)');
ylabel('Amplitude(volt)');
title('Signal of  BPSK modulation ');

% ********************* Define BPSK Demodulation **************************

yp_dem=[];
for n=t2L:t2L:length(yp)
  t=Tb/nb:Tb/nb:Tb;
  c=cos(2*pi*fc*t); % carrier signal 
  yp_dem0=c.*yp((n-(t2L-1)):n);
  t4=Tb/nb:Tb/nb:Tb;
  z=trapz(t4,yp_dem0); % integration 
  A_dem=round((2*z/Tb));                                     
  if(A_dem>Ac/2) % logic level = Ac/2
    A=1;
  else
    A=0;
  end
  yp_dem=[yp_dem A];
end

xp_out=yp_dem; % output signal;

% *************** Represent output signal as digital signal ***************
xxp_bit=[];
for n=1:length(xp_out);
    if xp_out(n)==1;
       xxp_bitt=ones(1,nb);
    else xp_out(n)==0;
        xxp_bitt=zeros(1,nb);
    end
     xxp_bit=[xxp_bit xxp_bitt];

end
t4=Tb/nb:Tb/nb:nb*length(xp_out)*(Tb/nb);
subplot(4,3,9)
plot(t4,xxp_bit,'LineWidth',2);grid on;
axis([ 0 Tb*length(xp_out) -0.5 1.5]);
ylabel('Amplitude(volt)');
xlabel(' Time(sec)');
title('Output signal as digital signal');
subplot(4,3,12)
stem(xp_out,'filled')
ylabel('Binary value');
xlabel('Bit index');
title('Output bits');

% **************************** end of program *****************************
