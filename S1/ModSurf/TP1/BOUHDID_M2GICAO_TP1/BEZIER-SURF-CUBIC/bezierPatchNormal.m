%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Evaluation d'un patch de Bezier aux parametres u,v
%
% Input:
%  - matrix N de 16 points de controle de dim 3
%     chaque point de controle a 3 coordonnes (x,y,z)
%     taille de B: num_n x num_n x 3
%     B(:,:,k) keme coordonnes des16  points de controle, k=1,2,3
%     B(i,j,:) les 3 coordonnes du point de controle b_ij
%  - u Vecteur de |u|=num_n valeurs de parametre en u
%  - v Vecteur de |v|=num_n valeurs de parametre en v

% Output:
%  - matrix N avec la grille de |u|x|v| points 3D sur la surface
%    La structure de S est similaire a celle de B.
%    Taille de S: |u|x|v|x3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function N = bezierPatchNormal(B,u,v)

 N = zeros(length(u),length(v),3);
 S(:,:,:) = bezierPatchEval(B(:,:,:),u,v);
for i = 1:length(u)-1
   	for j = 1:length(v)-1
        Xu = S(i+1,j,:) - S(i,j,:);
        Xuk = [Xu(1); Xu(2); Xu(3)];
    	Xv = S(i,j+1,:) - S(i,j,:);
        Xvk = [Xv(1); Xv(2); Xv(3)];
    	N(i,j,:) = (cross(Xuk,Xvk) / norm(cross(Xuk,Xvk)));
    end
end
