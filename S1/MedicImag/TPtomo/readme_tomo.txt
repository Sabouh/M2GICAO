Pour le TP TOMO distribuer les fichiers
- tptomo.pdf : le sujet. => on ne fait que la premi�re question!!! programation de FBP!
- ExplainationTPtomo.pdf donne quelques explications sur l'algorithme en pratique
- litsinogramme.m : permet de lire le sinogramme data127x80
- testlitsino.m : teste la lecture des donn�es (et appelle litsinogramme pour data127x80 et simplement "load" pour data180x100 (qui sont transpos�es cf. le code ci-dessous))
- data127x80 : un sinogramme de test qui se lit avec litsinogram.m
               (contient 127 projections de 80 donn�es chacune, cad 80 int�grales de droites parall�les)
               tester d'abord votre algo sur ce jeu de donn�es.
- data180x100 : un sinogramme de donn�es m�dicales 
                 qui se lit avec sino=load('d180x100');size(sino) 
             (contient 180 projections de 100 donn�es chacune)

=> pour lire les donn�es en matlab executer le programme testlitsino
ou de mani�re semblable :
% premier jeu de donn�es 
clear all; close all;
nphi=127;
ns=80
sino127x80=litsinogramme('data127x80',nphi,ns);
figure(1);
colormap(gray);
imagesc(sino127x80);
% faire la reconstruction
% ...
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% second jeu de donn�es
clear all; close all;
nphi=180;
ns=100
sino180x100=(load('data180x100'))';
figure(2);
colormap(gray);
imagesc(sino180x100);
% faire la reconstruction
% ...

