% Programme de test de la lecture des données
% lecture d'un sinogramme
% et lecture du sinogramme médical
% 
%nphi=384;% nphi=P=nombre de projections
%ns=125;% ns=Q=nombre de translations  
%g=litfbsinogramme('dataFB384_2PIx125_SL',nphi,ns); % lecture des données simulées
nphi=192;% nphi=P=nombre de projections
ns=125;% ns=Q=nombre de translations  
g=litfbsinogramme('dataFB192_PIx125_SL',nphi,ns); % lecture des données simulées
figure(1)
colormap('default')
imagesc(g);
figure(2);
colormap(gray);
imagesc(g);

hs = 1 /(nphi);
%freqcutoff >0
freqcutoff = 98;

%filtre de hilbert en parallele
HFilter = hilbertfilter(ns,hs,freqcutoff);

%figure(3);
%imagesc(HFilter);

%Hsino = zeros(nphi,ns);
%zeropadding 

for k = 1:nphi
zeropaddedproj = [g(k,:) zeros(1,ns-1)]; %zeropadding
paddedHproj = cconv(zeropaddedproj, HFilter, 2*ns-1); %circular convolution
Hsino(k,:) = paddedHproj(1:ns); %unzeropadding
end

figure(4);
%colormap(gray);
imagesc(Hsino);

%derivation
for l = 2:ns-1
        Dsino(:,l) = ( Hsino(:,l+1) - Hsino(:,l-1) )/(2*hs);
end
Dsino = Dsino/(2*pi);
figure(5);
%colormap(gray);
imagesc(Dsino);


%backprojection


N = 200;    % resolution de l'image
hs = 1 / ns;
dPhi = linspace(0,pi-(pi/nphi),nphi);   % discretisation de [0, pi] en 127 intervalle
dS = linspace(-1,1,ns);                  % discretisation de [-1 1] en 80 intervalle
cosphi = cos(dPhi);
sinphi = sin(dPhi);

A = zeros(N);   % creation de l'image a calculer

for x=1:N
    for y=1:N
        
        for j=1:nphi
            xn = ((x / N) * 2) - 1;
            yn = ((y / N) * 2) - 1;
            if (xn^2 + yn^2 < 1)
                S = xn * cosphi(j) + yn * sinphi(j);    % S : abscisse sur O(phi)
                %((S + 1) / 2.0) * 80;
                L = 1 + floor( ((S+1)/2) * ns);
                %L = 1 + floor((S + 1) * 2 / ns);
                if (L == ns)
                   v = Dsino(j,L);
                else
                    a = Dsino(j,L);
                    b = Dsino(j,L+1);
                    t = (S - dS(L)) / hs;
                    v = (1-t) * a + t * b;
                end
                
                A(x,y) = A(x,y) + v;
            end 
        end
        
    end
end
figure(6);
colormap('gray');
imagesc(A); 




