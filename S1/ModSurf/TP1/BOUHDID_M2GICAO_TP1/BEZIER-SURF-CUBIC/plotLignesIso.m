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
function plotLignesIso(B,I,c,eps,u,v)

str1='\bf Control Point';
str2='\bf Control Polygon';
str3='\bf Patch (bi-directional Bezier curve)';

% %-----------------------------------------------% 
% % Patch avec faceted shading

S(:,:,:) = bezierPatchEval(B(:,:,:),u,v);
figure, hold on
for i=1:length(u)
    for j=1:length(v)
        if (abs(I(i,j,:) -c) < eps)
            plot3(S(i,j,1),S(i,j,2),S(i,j,3))
            
        end
    end
end
%surface(S(:,:,1),S(:,:,2),S(:,:,3))
%quiver3(S(:,:,1),S(:,:,2),S(:,:,3),N(:,:,1),N(:,:,2),N(:,:,3),0.5)
shading faceted
title('\bf Lignes Isophotes ');
view(3); box;  view(21,19)
% %-----------------------------------------------% 