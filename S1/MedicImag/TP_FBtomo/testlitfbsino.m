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
R=3;
psiM=asin(1./R);
pas = 2*psiM/(ndet-1);
%psiiiiiiii
psi = -psiM:pas:psiM;

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
res = 0;
h = 2/Q;
mu = zeros(N,N);

for i= 1:N
	x = i/100 -1;
	for j= 1:N
		y = j/100 -1;
        for k =1:nproj
			%%%%%%-alphak alphak
            
			
		end
	end
end


figure(3);
colormap(gray);
%imagesc(filteredwdata);
