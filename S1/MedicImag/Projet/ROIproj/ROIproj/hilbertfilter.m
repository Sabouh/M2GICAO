% hilbertfilter = HILBERTFILTER(ndetecteurs,hs,freqcutoff);
% calcule un filtre de Hilbert de reconstruction pour une géométrie
% de ndetecteurs échantillonnés régulièrement en translation avec un pas 
% de hs
% le filtre est calculé dans le domaine réel direct (pas le domaine de Fourier)
% Attention la longueur du filtre est de 2*ndetecteurs-1 de telle manière à pouvoir 
% effectuer la convolution 
% - soit dans l'espace directe
% - soit dans l'espace de Fourier avec des techniques de zéro padding
%
% freqcutoff est une coupure en fréquence 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Hfilter = hilbertfilter(ndetecteurs,hs,freqcutoff);
Hfilter=zeros(1,2*ndetecteurs-1);
echants=hs*[0:(ndetecteurs-1) (-ndetecteurs+1):-1];
% calcul de 2 Sin^2(PI s c)/(Pi s)= 2  PI s c^2 [Sin(PI s c)/(Pi s c)]^2 
% le sinc(x) de matlab est sin(PI x)/(PI x)
Hfilter=sinc(freqcutoff*echants);
Hfilter=Hfilter.^2;
Hfilter= 2*pi*freqcutoff^2*Hfilter.*echants;
%
%
