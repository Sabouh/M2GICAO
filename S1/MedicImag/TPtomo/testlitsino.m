% Programme de test de la lecture des donn�es
% lecture d'un sinogramme
% et lecture du sinogramme m�dical
% 
nphi=127;% nphi=P=nombre de projections
ns=80;% ns=Q=nombre de translations  
g=litsinogramme('data127x80',nphi,ns); % lecture des donn�es simul�es
figure(1)
% contour(g,10); pause(1); surf(g); pause(1); mesh(g); pause(1);
colormap('default')
imagesc(g);
figure(2);
colormap(gray);
%contour(g,10); pause(1); surf(g); pause(1); mesh(g);pause(1);
%pcolor(g);
imagesc(g);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
gmed=(load('data180x100'))'; % lecture des donn�es m�dicales
dimg=size(gmed);
nphi=dimg(2);
ns=dimg(1);
figure(3)
colormap(gray);
%pcolor(gmed);
imagesc(gmed);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nphi=127;% nphi=P=nombre de projections
ns=80;% ns=Q=nombre de translations  
g=litsinogramme('data127x80',nphi,ns); % lecture des donn�es simul�es

f = FBP(g,nphi,ns);
figure(4);
colormap(gray);
imagesc(f);
