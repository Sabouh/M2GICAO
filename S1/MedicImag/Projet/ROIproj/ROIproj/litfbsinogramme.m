% fbsino = LITFBSINOGRAMME(filename,nprojections,ndetecteurs);
% lecture dans le tableau fbsino (nprojections X ndetecteurs) du fichier filename
% Le fichier contient un sinogramme en g�om�trie Fan Beam 
% sur une trajectoire circulaire (diam�tre R) �chantillonn� r�guli�rement 
% en (t,alpha) 
% en g�om�trie compl�te (t,alpha)\in [0 2pi[ x [t-arcsin(1/R),t+arcsin(1/R)]
% en g�om�trie incompl�te (t,alpha)\in [0 angle[ x [t-arcsin(1/R),t+arcsin(1/R)]
%  ou angle est l'angle parcourue par la source sur le cercle de la trajectoire
% Dans le cas de projections � d�tecteur tronqu�es alpha sera restreint � alpha \in [t-alpha_m,t+alpha_M] avec (alpha_M-alpha_m) le d�battement angulaire du d�tecteur 
% organis�e de la mani�re suivante
% pour chaque position de la source t_k,  c'est � dire pour chaque projection k, 
% pour chaque position angulaire  alpha_l  du d�tecteur 
% fbsino(k,l) est g(tk,alphal)
% avec tk= (k-1)2pi/nprojections et alphal=tk-arcsin(1/R)+(l-1)*2arcsin(1/R)/(ndetecteurs-1) 
% (plus g�n�ralement alphal=tk-arcsin(1/R)+(l-1)*(alpha_M-alpha_m)/(ndetecteurs-1) 
% et g est la transform�e en rayon divervent (fan beam) d'une fonction � identifier f
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

