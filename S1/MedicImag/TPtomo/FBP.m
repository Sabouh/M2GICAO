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

figure(3);
colormap('gray');
imagesc(GF);

%backprojection
%phi : nphi
% k de 1 a P
% x.teta(phi) : x1.cos(phik) + x2.sin(phik)
%
N=200;
res = 0;
h = pi/P;
mu = zeros(N,N);
%%On se replace ds la bonne base
for i=1:N
    for j=1:N
        for k =1:P
            xteta = (cos(k*pi/P)*(-1+(i-0.5)*h))+(sin(k*pi/P)*(-1+(j-0.5)*h));
            %%On interpole xteta !!!!!!
            res = res + interpole(GF,k,xteta);
        end
        mu(i,j) = res;
        res=0;
    end
end
figure(4);
imagesc(mu);