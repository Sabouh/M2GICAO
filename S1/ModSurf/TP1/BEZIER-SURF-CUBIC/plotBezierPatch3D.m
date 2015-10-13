%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% plot d'un patch de Bezier
%
% Un patch de Bezier bicubique est defini par 16 points de controle de R^3
% stockes dans une matrice 4 x 4 x 3
%
% Input:
%  - Matrice B avec les 16 points de controle d'un patch de Bezier bicubique.
%    Un point de controle a 3 coordonnees (x,y,z)
%    La taille de B est 4 x 4 x 3 
%      B(:,:,k) : matrice 4x4 de points de controle de la keme dimension, k=1,2,3 
%      B(:,:,1): matrice 4x4 de x-coordonnes des points de controle 
%      B(:,:,2): matrice 4x4 de y-coordonnes des points de controle 
%      B(:,:,3): matrice 4x4 de z-coordonnes des points de controle
%      B(i,j,:): 3 coordonnes du point de controle Bij
%  - Matrice S : points calcules sur la surface
%    La structure de S est similaire a celle de B.
%
% Visualisations proposees
%  - reseau de controle
%  - wireframe
%  - smooth shading with color
%  - smooth shading with wires
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function plotBezierPatch3D(B,S)

str1='\bf Control Point';
str2='\bf Control Polygon';
str3='\bf Patch (bi-directional Bezier curve)';

% %-----------------------------------------------
% % lot de Reseau de controle 
figure, hold on
surface(B(:,:,1),B(:,:,2),B(:,:,3),'FaceColor','y')
title('\bf Reseau de controle');
view(3); box;  view(21,19)
% %-----------------------------------------------
% % Plot de Patch de Bezier
figure, hold on
surface(S(:,:,1),S(:,:,2),S(:,:,3),'FaceColor','g')
title('\bf Surface de Bezier');
view(3); box;  view(21,19)

% %----------------------------------------------- 
% % Patch avec interpolated shading
figure, hold on
surface(S(:,:,1),S(:,:,2),S(:,:,3))
shading interp
title('\bf Surface de Bezier avec Interpolated Shading');
view(3); box;  view(21,19)
% %-----------------------------------------------% 
% % Patch avec faceted shading
figure, hold on
surface(S(:,:,1),S(:,:,2),S(:,:,3))
shading faceted
title('\bf Surface de Bezier avec Faceted Shading');
view(3); box;  view(21,19)
% %-----------------------------------------------% 
