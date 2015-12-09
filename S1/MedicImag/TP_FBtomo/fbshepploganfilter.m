% fbSLfilter = FBSHEPPLOGANFILTER(ndetecteurs,halpha,freqcutoff);
% calcul un filtre de Shepp Et Logan de reconstruction pour une géométrie
% Fan Beam de ndetecteurs échantillonnés régulièrement en angle avec un pas angulaire
% de halpha
% le filtre est calculé dans le domaine réel direct (pas le domaine de Fourier)
% Attention la longueur du filtre est de 2*ndetecteurs+1 de telle manière à pouvoir 
% effectuer la convolution 
% - soit dans l'espace directe
% - soit dans l'espace de Fourier avec des techniques de zéro padding
%
% freqcutoff est une coupure en fréquence 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function fbSLfilter = fbshepploganfilter(ndetecteurs,halpha,freqcutoff);
fbSLfilter=zeros(1,2*ndetecteurs+1);
echantalpha=[halpha*(0:ndetecteurs) halpha*(-ndetecteurs:-1)];
sinealpha=sin(echantalpha);
fcoffsin=freqcutoff*sinealpha;
fbSLfilter=freqcutoff^2*(pi/2-fcoffsin.*sin(fcoffsin)) ./ ( (pi/2)^2 - fcoffsin.^2 );
%
%
