% fbsino = LITFBSINOGRAMME(filename,nprojections,ndetecteurs);
% lecture dans le tableau fbsino (nprojections X ndetecteurs) du fichier filename
% Le fichier contient un sinogramme en géométrie Fan Beam 
% sur une trajectoire circulaire (diamètre R) échantillonné régulièrement 
% en (t,alpha) 
% en géométrie complète (t,alpha)\in [0 2pi[ x [t-arcsin(1/R),t+arcsin(1/R)]
% en géométrie incomplète (t,alpha)\in [0 angle[ x [t-arcsin(1/R),t+arcsin(1/R)]
%  ou angle est l'angle parcourue par la source sur le cercle de la trajectoire
% Dans le cas de projections à détecteur tronquées alpha sera restreint à alpha \in [t-alpha_m,t+alpha_M] avec (alpha_M-alpha_m) le débattement angulaire du détecteur 
% organisée de la manière suivante
% pour chaque position de la source t_k,  c'est à dire pour chaque projection k, 
% pour chaque position angulaire  alpha_l  du détecteur 
% fbsino(k,l) est g(tk,alphal)
% avec tk= (k-1)2pi/nprojections et alphal=tk-arcsin(1/R)+(l-1)*2arcsin(1/R)/(ndetecteurs-1) 
% (plus généralement alphal=tk-arcsin(1/R)+(l-1)*(alpha_M-alpha_m)/(ndetecteurs-1) 
% et g est la transformée en rayon divervent (fan beam) d'une fonction à identifier f
function fbsino = litfbsinogramme(filename,nprojections,ndetecteurs);
%
fbsino=zeros(nprojections,ndetecteurs);
%disp(filename);
fbsinofile = load(filename);
%taille = size(sinofile)
for k=1:nprojections
   for l=1:ndetecteurs
      fbsino(k,l)=fbsinofile(1,(k-1)*ndetecteurs+l);
   end
end
%

