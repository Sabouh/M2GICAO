
close all; 
clear all;
%%%%%%%%%%%%%%%%%%%%%%%%%Decommenter le test que l'on veut faire !
%%%TEST1
%nphi=127;% nphi=P=nombre de projections
%ns=80;% ns=Q=nombre de translations  
%g=litsinogramme('data127x80',nphi,ns); % lecture des données simulées

%%%%%TEST2
gmed=(load('data180x100'))'; % lecture des données médicales
dimg=size(gmed);
nphi=dimg(1);
ns=dimg(2);
g = gmed;
%%%%%

P = nphi;
Q = ns;
figure(1);
imagesc(g);
filter = (-Q/2):1:(Q/2)-1;
filter = abs(filter);
filter = fftshift(filter);
FG = zeros(Q,P);

TFG = fft(g');
    for k=1:P
        FG(:,k) = filter' .* TFG(:,k);
    end
     GF = (ifft(FG))';
     

figure(2);
imagesc(GF);

figure(3);
colormap('gray');
imagesc(GF);

%backprojection
N=200;
res = 0;
h = 2/Q;
mu = zeros(N,N);
%%On se replace ds la bonne base
%%On replace les coordonnées dans -1 et 1

for i=1:N
    x = i/100 -1; %coordonnée x entre -1 et 1 (car l'image finale sera de taille 200x200 -> [0:200] à [-1: 1])
    for j=1:N
        y = j/100 -1; %coordonnée y entre -1 et 1
        for k =1:P
            %%On verifie que (x,y) appartient au cercle
            if (x*cos(k*pi/P) + y*sin(k*pi/P) <= 1 && x*cos(k*pi/P) + y*sin(k*pi/P) >=-1)
                xteta = x*cos(k*pi/P)+y*sin(k*pi/P);
                %%On interpole xteta 
                sl =  floor((1+xteta)/h);
                %%On commence à 1 dans GF.
                if(sl >0 && sl <Q)
                    interpol = GF(k,sl)*(xteta*2/(Q-1)-sl) + GF(k,sl+1)*(2/(Q-1)-(xteta*2/(Q-1)-sl));
                    res = res + interpol;
                end
            else
               %% "pas dans le cercle";
            end
            
        end
        %On place la valeur dans l'image
        mu(i,j) = res;
        res=0;
    end
end
figure(4);
colormap('gray');
imagesc(mu); 