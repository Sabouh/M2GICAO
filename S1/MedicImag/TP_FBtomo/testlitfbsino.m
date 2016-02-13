clear all; close all;
%Exemple1
nproj=384;
ndet=125;
fbsino=litfbsinogramme('dataFB384x125_SL',nproj,ndet);
%Exemple2
%nproj=128;
%ndet=41;
%fbsino=litfbsinogramme('dataFBR3_128x41_Disk',nproj,ndet);

figure(1);
colormap(gray);
imagesc(fbsino);
title('data : FB sinogramme ')


% data weighting
% psiM est le demi fan angle
% pas (psiH): rotation à faire pour passer d'un detecteur à un autre
% psi : liste de tous les angles compris entre -psiM et psiM
R=3;
psiM=asin(1./R);
pas = 2*psiM/(ndet-1);
%on découpe l'espace [-demi fan angle ; demi fan angle] selon le nb de
%detecteurs
psi = -psiM:pas:psiM;
% table de projections
proj = linspace(0,2*pi,(2*pi)/nproj);

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

N=200;
res = 200;
hdet = 2/ndet;
mu = zeros(N,N);

for i= 1:N
	x = i/res*2 -1;
	for j= 1:N
		y = j/res*2 -1;
        %si dans cercle 
        if (x^2 + y^2 < 1)    
            %pour toute projection
            for k =1:nproj
                %compute Psi x_ijt
                %changement de base
                xteta = x*cos(psi(k)) + y*sin(proj(k));
                yzeta =  x*(-sin(proj(k))) + y*cos(psi(k));
                
                %%                
                psi = atan(yzeta/xteta);
                
                % l = 1 + floor(Psi + Ppsim/hPsi)
                l = 1 + floor( psi +psiM /pas);
                
                
                %mu(i,j) = mu(i;j) +
                %linearInterpo(filtererWdata,l,l+1,PSix_ijt)/|x - psiT|²        
                %on interpole 
                interpol = ( filteredwdata(k,l)*(1 - ?)) + (filteredwdata(k,l+1))*?;         
                mu(i,j) = mu(i,j) + interpol ;
            end
		end
	end
end


figure;
colormap(gray);
imagesc(mu);
