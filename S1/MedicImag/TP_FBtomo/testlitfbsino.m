clear all; close all;
%Exemple1
nproj=384;
ndet=125;
R=3;
fbsino=litfbsinogramme('dataFB384x125_SL',nproj,ndet);
%Exemple2
%nproj=128;
%ndet=41;
%R=3;
%fbsino=litfbsinogramme('dataFBR3_128x41_Disk',nproj,ndet);

figure(1);
colormap(gray);
imagesc(fbsino);
title('data : FB sinogramme ')


% data weighting
% psiM est le demi fan angle
% pas (psiH): rotation à faire pour passer d'un detecteur à un autre

psiM=asin(1./R);
pas = 2*psiM/(ndet-1);
psi = -psiM:pas:psiM;
cosPsi = R*cos(psi);

weight = R*cos(psi);
Wdata = zeros(nproj,ndet);

for t=1:nproj
    Wdata(t,:) = fbsino(t,:).*weight;
end

%  filtering
cutoff=(1+R)*nproj*0.5;
filter = fbshepploganfilter(ndet,pas,cutoff);
ftFilter = fft(filter);
%data zero padding
    zpwdata = zeros(nproj,2*ndet+1);
    zpwdata(:,1:ndet)=Wdata;
    filteredzpwdata = zeros(nproj,2*ndet+1);
    for t=1:nproj
        ftzpwdata = fft(zpwdata(t,:));
        filteredftzpwdata = ftzpwdata .* ftFilter;
        filteredzpwdata(t,:) = real(ifft(filteredftzpwdata));
    end
    filteredwdata = filteredzpwdata(:,1:ndet);
    
figure(2);
%colormap(gray);
imagesc(filteredwdata);

%backprojection

%
pasPhi  = 2*pi/(ndet-1);
phi = 0:pasPhi:2*pi;
cosPhi = cos(phi);
sinPhi = sin(phi);

N=400;
h = 2/ndet;
mu = zeros(N);

for i= 1:N
	x = i/N*2 -1;
	for j= 1:N
		y = j/N*2 -1;
        if(x^2 + y^2 < 1)
            for k =1:nproj
                xteta = (x *cosPhi(k))+ (y*sinPhi(k));
                %changement de base
                yzeta = (x * -sinPhi(k)) +(y*cosPhi(k));

                dist_xy1 = sqrt(((xteta+R) * (xteta+R)) + (yzeta *yzeta));     % distance du pixel par rapport a l'origine (teta,zeta)
                
                
                radian1 = acos((xteta+R) / dist_xy1);   % radian compris entre [0 dfa]
                if (yzeta < 0)               % ajustement du signe en fonction de l'orientation par rapport à zeta
                    radian1 = -radian1;       % radian compris entre [-dfa dfa]
                end
                
                L = 1 + floor( (radian1 + psiM) / pas );
                if (L == ndet)
                    L = ndet-1;
                end
                
                a = filteredwdata(k,L);
                b = filteredwdata(k,L+1);
                
                %r = (radian - psi(L)) / hpsi;  % acces a psi(L) tres long ...
                r = (radian1 - (-psiM + (L-1)*pas)) / pas;
                mu(i,j) = mu(i,j) + ((1-r)*a + r*b);      % accumulation dans l'image finale
            end
        end
	end
end


figure(3);
colormap(gray);
imagesc(mu);
