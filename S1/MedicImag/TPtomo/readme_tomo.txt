Pour le TP TOMO distribuer les fichiers
- tptomo.pdf : le sujet. => on ne fait que la première question!!! programation de FBP!
- ExplainationTPtomo.pdf donne quelques explications sur l'algorithme en pratique
- litsinogramme.m : permet de lire le sinogramme data127x80
- testlitsino.m : teste la lecture des données (et appelle litsinogramme pour data127x80 et simplement "load" pour data180x100 (qui sont transposées cf. le code ci-dessous))
- data127x80 : un sinogramme de test qui se lit avec litsinogram.m
               (contient 127 projections de 80 données chacune, cad 80 intégrales de droites parallèles)
               tester d'abord votre algo sur ce jeu de données.
- data180x100 : un sinogramme de données médicales 
                 qui se lit avec sino=load('d180x100');size(sino) 
             (contient 180 projections de 100 données chacune)

=> pour lire les données en matlab executer le programme testlitsino
ou de manière semblable :
% premier jeu de données 
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
% second jeu de données
clear all; close all;
nphi=180;
ns=100
sino180x100=(load('data180x100'))';
figure(2);
colormap(gray);
imagesc(sino180x100);
% faire la reconstruction
% ...

