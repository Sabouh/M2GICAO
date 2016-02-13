close all, clear all
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Calcul et visualisation de surfaces de Bezier
%
% Input: fichoer ascii avec 16xnp points de controle
%
% Les patches sont individuellement evalues en un 
% nombre fixe de parametres (u,v).
%
% Parametres: 
% num_p : nombre de valeurs de parametres = nombre de points de controle
% num_n : nombre de normales calcules (pour le calcul des isophotes)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%BezierSurf = load('surface1');  % read control points
%BezierSurf = load('surface2'); % read control points
%BezierSurf = load('surface3'); % read control points
BezierSurf = load('surface4'); % read control points
%load('teapot'); %loading matrix B
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
num_p=20;                    % nombre de valeurs de parametre en direction u et v
num_n=100;		     % plus num_p est grand plus la surface paraitra lisse
			     % et plus le calcul sera long

%-------------------------------------------------
[nb,~] = size(BezierSurf) % nombre de points de controle dans le fichier
np = floor(nb/16) % nombre de patches composant la surface
                   % Il faudrait verivier que nb est un multiple de 16 %
% Matrice B des points de controle
for k=1:np
  for i=1:4
    for j=1:4
      B(i,j,1,k) = BezierSurf((i-1)*4+j,1);
      B(i,j,2,k) = BezierSurf((i-1)*4+j,2);
      B(i,j,3,k) = BezierSurf((i-1)*4+j,3);
    end
  end
end

% La matrice B stocke tous les points de controle de tous les patchs
% B(:,:,:,k) sont tous les points de controle du patch k
% La dimension de B(:,:,:,k) est 4 x 4 x 3, i.e., 16 points de controle
% a 3 coordonnees (x,y,z)

% B(:,:,1,k): x-coordonnesv des points de controle du patch k comme matrice 4 x 4
% B(:,:,2,k): y-coordonnes des points de controle du patch k comme matrice 4 x 4
% B(:,:,3,k): z-coordonnes des points de controle du patch k comme matrice 4 x 4

% ------------------------------------
% num_p+1 valeurs de parametres uniformes: 0,1,2,...,num_p en u et v
u = linspace(0,1,num_p); 
v = u; 


%  ------------------------------------
% Cubic Bezier patches 
for k=1:np
    S(:,:,:,k)=bezierPatchEval(B(:,:,:,k),u,v); %evaluation du patch k en (num_p+1) x (num_p+1) points
end


% ------------------------------------
% Normal vectors of Cubic Bezier patches 
u=linspace(0,1,num_n); v=u;  %parametrisation uniforme (num_n+1)x (num_n+1) valeurs de parametre

for k=1:np
    N(:,:,:,k)=bezierPatchNormal(B(:,:,:,k),u,v); %vecteurs normal du patch k
end

% ------------------------------------
% Computing Isophotes
%L = [0 0 1];
%  for k=1:np
 %     I1(:,:,:,k) = lignesIsophotes(N,L,u,v);
  %end

%L = [0 1 0];
 % for k=1:np
  %    I2(:,:,:,k) = lignesIsophotes(N,L,u,v);
  %end
  
%L = [0 1 1];
%  for k=1:np
%      I3(:,:,:,k) = lignesIsophotes(N,L,u,v);
%  end
  
%L = [1 0 0];
%  for k=1:np
%      I4(:,:,:,k) = lignesIsophotes(N,L,u,v);
%  end
  
%L = [1 0 1];
  
%  for k=1:np
%      I5(:,:,:,k) = lignesIsophotes(N,L,u,v);
%  end
  
%L = [1 1 0];
%  for k=1:np
%      I6(:,:,:,k) = lignesIsophotes(N,L,u,v);
%  end
  
  
%L = [1 1 1];
%  for k=1:np
%      I7(:,:,:,k) = lignesIsophotes(N,L,u,v);
%  end

% -------------------------------------
%Computing curvature plot
u=linspace(0,1,num_n); v=u;  %parametrisation uniforme (num_n+1)x (num_n+1) valeurs de parametre

for k=1:np
   K(:,:,:,k)=curvPlot(S(:,:,:,k),u,v); %vecteurs normal du patch k
end

% --------------------------------------
% Visualisation d'un patch/surface de Bezier
  plotBezierPatch3D(B(:,:,:,k),S(:,:,:,k)) % plot d'un seul patch k
  plotBezierSurface3D(B,S)		   % plot de tous les np patches
 %% plotBezierPatch3DNormal(B(:,:,:,k),S(:,:,:,k),N(:,:,:,k));
  plotBezierNormale3D(B,N,u,v);
 % c = 1;
 % eps = 0.3;
 % plotLignesIso(B,I1,c,eps,u,v);
 % plotLignesIso(B,I2,c,eps,u,v);
 % plotLignesIso(B,I3,c,eps,u,v);
 % plotLignesIso(B,I4,c,eps,u,v);
 % plotLignesIso(B,I5,c,eps,u,v);
 % plotLignesIso(B,I6,c,eps,u,v);
 % plotLignesIso(B,I7,c,eps,u,v);
 