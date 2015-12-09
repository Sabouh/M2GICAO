function I = lignesIsophotes(N,L,u,v)
for i=1:length(u)
    for j=1:length(v)
        Nk = [N(i,j,1); N(i,j,2); N(i,j,3)];
        I(i,j,:) = dot(Nk,L');
    end
end