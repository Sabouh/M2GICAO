% sino = LITSINOGRAMME(filename,dimphi,dims);
% lecture dans le tableau sino (dimphi X  dims) du fichier filename
% Le fichier contient un sinogramme standard �chantillonn� sur [0 pi[ x [-1,1]
% organis�e de la mani�re suivante
% pour chaque projection k, pour chaque translation l, sino(k,l)
% sino(k,l) est g(phik,sl)
% avec phik= (k-1)pi/dimphi et sl=-1+(l-1)*2/(dims-1) 
% et g est la transform�e de Radon d'une fonction � identifier f
function sino = litsinogramme(filename,dimphi,dims);
%
sino=zeros(dimphi,dims);
%disp(filename);
sinofile = load(filename);
taille = size(sinofile)
for k=1:dimphi
   for l=1:dims
      sino(k,l)=sinofile(1,(k-1)*dims+l);
   end
end
%

