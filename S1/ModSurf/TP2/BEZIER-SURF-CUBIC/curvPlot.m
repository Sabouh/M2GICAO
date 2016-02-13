%G = [ <Xu,Xu> <Xu,Xv> ; <Xv,Xu> <Xv,Xv>]
%H = [ <Xuu,N> <Xuv,N> ; <Xvu,N> <Xvv,N>]
%L = HG^-1  : valeurs propres Kmin et Kmax
%Kmin et Kmax courbures principales de X en (u,v)
%Notes :
%N(u,v) = (Xu x Xv)/ |Xu x Xv|
%Xu derivée partielle de X selon u 
%Xv derivée partielle de X selon v
%Xuu derivée partielle de Xu selon u
%Xuv derivée partielle de Xu selon v
%Xvu derivée partielle de Xv selon u 
%Xvv derivée partielle de Xv selon v

function K = curvPlot(S,u,v)

    for k=1:3
        Sudiff(:,:,k) = derivee(S(:,:,k)); % dérivée de S en u
        Svdiff(:,:,k) = derivee(S(:,:,k)'); %dérivée de S en v
        Suudiff(:,:,k) = derivee(Sudiff(:,:,k));
        Suvdiff(:,:,k) = derivee(Sudiff(:,:,k)');
        Svudiff(:,:,k) = derivee(Svdiff(:,:,k)');
        Svvdiff(:,:,k) = derivee(Svdiff(:,:,k));
    end

    for i = 1:length(u)-1
        Xu = Sudiff(i,:,:);
        Xv = Svdiff(i,:,:);
        Xuu = Suudiff(i,:,:);
        Xuv = Suvdiff(i,:,:);
        Xvu = Svudiff(i,:,:);
        Xvv = Svvdiff(i,:,:);
        Xu
        size(Xu)
        Xv
        size(Xv)
        N = cross(Xu,Xv)/abs(cross(Xu,Xv'));
        %XuXu resultat du pdt scalaire <Xu, Xu>
        XuXu(i) = dot(Xu,Xu);

        %XuXv resultat du pdt scalaire <Xu, Xv>
        XuXv(i) = dot(Xu,Xv);

        %XvXu resultat du pdt scalaire <Xv, Xu>
        XvXu(i) = dot(Xv,Xu);

        %XvXv resultat du pdt scalaire <Xv, Xv>
        XvXv(i) = dot(Xv,Xv);

        %XuuN resultat du pdt scalaire <Xuu, N>
        XuuN(i) = dot(Xuu,N);

        %XuvN resultat du pdt scalaire <Xuv, N>
        XuvN(i) = dot(Xuv,N);

        %XvuN resultat du pdt scalaire <Xvu, N>
        XvuN(i) = dot(Xvu,N);

        %XvvN resultat du pdt scalaire <Xvv, N>
        XvvN(i) = dot(Xvv,N);


    G = [ XuXu XuXv ; XvXu XvXv ];
    H = [ XuuN XuvN ; XvuN XvvN ];
    L = H*G';
    [Kmin, Kmax] = eig(L);
    K(k) = [Kmin , Kmax];
    end