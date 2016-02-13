% FBHfilter = FBHILBERTFILTER(ndetecteurs,halpha,freqcutoff);
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
function FBHfilter = fbhilbertfilter(ndetecteurs,halpha,freqcutoff);
FBHfilter=zeros(1,2*ndetecteurs-1);
sinalpha=sin(halpha*[0:(ndetecteurs-1) (-ndetecteurs+1):-1]);
% calcul de h_c(alpha) = 2 Sin^2(PI sin(alpha) c)/(Pi sin(alpha))
%                 = 2  PI sin(alpha) c^2 [Sin(PI sin(alpha) c)/(Pi sin(alpha) c)]^2 
% le sinc(x) de matlab est sin(PI x)/(PI x)
FBHfilter=sinc(freqcutoff*sinalpha);
FBHfilter=FBHfilter.^2;
FBHfilter= 2*pi*freqcutoff^2*FBHfilter.*sinalpha;
%
%
