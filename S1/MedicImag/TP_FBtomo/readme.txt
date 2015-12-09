 
Pour la géométrie fan beam

- tpFBscanner.pdf : le sujet
- litfbsinogramme.m :  permet de lire les données fbsinogramme dataFBR3_128x41_Disk et dataFB384x125_SL
- dataFBR3_128x41_Disk et dataFB384x125_SL sont des fichiers de données en géométrie Fan Beam 
- fbshepploganfilter.m (définit un filtre de shepp et Logan)
% pour lire les données
clear all; close all;
%nproj=384;
%ndet=125;
%fbsino=litfbsinogramme('dataFB384x125_SL',nproj,ndet);
nproj=128;
ndet=41;
fbsino=litfbsinogramme('dataFBR3_128x41_Disk',nproj,ndet);

figure(1);
colormap(gray);
imagesc(fbsino);
title('data : FB sinogramme ')
