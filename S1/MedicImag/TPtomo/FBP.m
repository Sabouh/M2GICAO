%function GF = FBP(g,P,Q);
close all; 
clear all;
nphi=127;% nphi=P=nombre de projections
ns=80;% ns=Q=nombre de translations  
g=litsinogramme('data127x80',nphi,ns); % lecture des données simulées

P = nphi;
Q = ns;
figure(1);
imagesc(g);
filter = (-Q/2):1:(Q/2)-1;
filter = abs(filter);
filter = fftshift(filter);
FG = zeros(Q,P);
%GF = zeros(P,Q);
%for l = 0:(Q-1)
TFG = fft(g');
    for k=1:P
        FG(:,k) = filter' .* TFG(:,k);
       
    end
     GF = (ifft(FG))';
%end
figure(2);
imagesc(GF);