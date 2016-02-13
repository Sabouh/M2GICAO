function mcSurface(fittype,inputs,meshfilename)
  
  nargin = argn(2);
  if nargin<1 then fittype = 1;end
  if nargin<2 then inputs = 1;end
  if nargin<3 then meshfilename = "surface.mesh";end
  
  // les données de départ sont données par x et y
  tmpx = [-3 -2 -1 0 1 2 3];
  tmpy = [-3 -2 -1 0 1 2 3];
  tmpz = zeros(length(tmpx),length(tmpy));
  
  for i=1:length(tmpx)
    for j=1:length(tmpy)
      if inputs==2 then 
        tmpz(i,j) = exp((-tmpx(i)^2-tmpy(j)^2)/5)*10;
      elseif inputs==3 then  
        tmpz(i,j) = cos(tmpx(i))*sin(tmpy(j));
      else 
        a = 0.3;
        b = -0.2;
        c = 0.1;
        tmpz(i,j) = a*tmpx(i)+b*tmpy(j)+c+rand()*2-1;;
      end
    end
  end
  
  // reordonnancement des données 
  x = zeros(length(tmpx)*length(tmpy),1);
  y = zeros(x);
  z = zeros(x);
  i=1;
  for i1=1:length(tmpx)
    for i2=1:length(tmpy)
      x(i) = tmpx(i1);
      y(i) = tmpy(i2);
      z(i) = tmpz(i2,i1);
      i=i+1;
    end
  end
  
  
  n = 100;
  [xx,yy]=meshgrid(linspace(min(x),max(x),n),linspace(min(y),max(y),n));
  zz = zeros(n,n);
  

  if fittype==1 then
    c = mcPlan(x,y,z);
  else
    c = mcQuadric(x,y,z);
  end

  for i=1:size(zz,1)
    for j=1:size(zz,2)     
        if fittype==1 then
            zz(i,j) = evalPlan(xx(i,j),yy(i,j),c)
        else
            zz(i,j) = evalQuadric(xx(i,j),yy(i,j),c)
        end
    end
  end
  
  fid = mopen("points.vect", "w");
  write_points_VECT(fid,x,y,z,[1 0 0],3);  
  mclose(fid); 
  
  fid = mopen(meshfilename, "w"); 
  write_MESH(fid,xx,yy,zz,[1.0 0.8 0.6]);
  mclose(fid); 
endfunction 


function c = mcPlan(x,y,z)
  // x,y,z -> données d'entrées
  // c=[a,b,c] -> coef du plan z=ax+by+c
   [d1,d2]= size(x);
   M1 = linspace(1,1,d1);
   A = [x';y';M1];
   A = A';
   B = z;
   M = inv((A')*A)*(A'*B);
  // **** A MODIFIER/COMPLETER ****
  c = M'
  // ******************************
endfunction

function z=evalPlan(x,y,c)
  z = c(1).*x+c(2).*y+c(3);
endfunction

function c = mcQuadric(x,y,z)
  // x,y,z -> données d'entrées
  // c=[a,b,c] -> coef de la quadrique z=ax^2+by^2+c
  
   [d1,d2]= size(x);
   M1 = linspace(1,1,d1);
   A = [(x.*x)';(y.*y)';M1];
   A = A';
   B = z;
   M = inv((A')*A)*(A'*B);
  // **** A MODIFIER/COMPLETER ****
  c = M'
  // ******************************
endfunction

function z=evalQuadric(x,y,c)
  z = c(1).*(x.*x)+c(2).*(y.*y)+c(3);
endfunction








//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points de la polyligne
//          c = couleur RGB
//          l = épaisseur de la ligne
function write_VECT(fid,x,y,z,c,l);
  
  // début de l'objet composite
  mfprintf(fid,"{\n");
  
  // attribut pour l'épaisseur
  mfprintf(fid,"  appearance {linewidth %d}\n",l);
  
  // objet VECT : 1 polyligne avec n sommets et 1 couleur
  n=length(x);
  mfprintf(fid,"{\n");
  
  // l'entete
  mfprintf(fid,"VECT\n 1 %d 1\n %d\n 1\n", n, n);
  // les n sommets
  for i=1:n
    mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
  end
  // la couleur
  mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
  mfprintf(fid,"}\n");
  
  // fin de l'objet composite
  mfprintf(fid,"}\n");
  
endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// objet formé de n vecteurs [P(:,i),P(:,i)+h*V(:,i)]
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          P = les points de la polyligne
//          V = vecteurs associés
//          c = couleur RGB
//          h = homothetie pour le vecteur
//          l = épaisseur de la ligne
function write_VECTS(fid,P,V,c,h,l);
  
  // début de l'objet composite
  mfprintf(fid,"{\n");
  
  // attribut pour l'épaisseur
  mfprintf(fid,"  appearance {linewidth %d}\n",l);
  
  // objet VECT : 1 poyligne avec n sommets et 1 couleur
  n=size(P,2);
  mfprintf(fid,"{\n");
  
  // l'entete
  mfprintf(fid,"VECT\n %d %d %d\n", n, 2*n, n);
  
  // nb de points par polyline
  for i=1:n
    mfprintf(fid," 2");
  end
  mfprintf(fid,"\n");
  
  // nb de couleurs par polyline
  for i=1:n
    mfprintf(fid," 1");
  end
  mfprintf(fid,"\n");
  
  // les n vecteurs
  for i=1:n
    mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
    P(1,i), P(2,i), P(3,i));
    mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
    P(1,i)+h*V(1,i), P(2,i)+h*V(2,i), P(3,i)+h*V(3,i));
  end
  
  // les n couleurs
  for i=1:n
    mfprintf(fid,"%8.5f %8.5f %8.5f 1\n", c(1), c(2), c(3));
  end
  mfprintf(fid,"}\n");
  
  // fin de l'objet composite
  mfprintf(fid,"}\n");
  
endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet de type VECT 
// représentant un muage de points 
// dans le fichier dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points 
//          c = couleur RGB
//          l = épaisseur de la ligne
function write_points_VECT(fid,x,y,z,c,l);
  
  // début de l'objet composite
  mfprintf(fid,"{\n");
  
  // attribut pour l'épaisseur
  mfprintf(fid,"  appearance {linewidth %d}\n",l);
  
  // objet VECT : n poyligne avec chacune 1 sommet et 1 couleur
  n=length(x);
  mfprintf(fid,"{\n");
  
  // l'entete
  mfprintf(fid,"VECT\n %d %d %d\n", n, n, n);
  
  // nombre de points par ligne
  for i=1:n
    mfprintf(fid," 1");
  end
  mfprintf(fid,"\n");
  
  // nombre de couleurs par ligne
  for i=1:n
    mfprintf(fid," 1");
  end
  mfprintf(fid,"\n");
  
  // les n sommets
  for i=1:n
    mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
  end
  
  // les couleur
  for i=1:n
    mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
  end
  mfprintf(fid,"}\n");
  
  // fin de l'objet composite
  mfprintf(fid,"}\n");
  
endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet MESH
function write_MESH(fid,X,Y,Z,c)
  
  n1 = size(X,1);
  n2 = size(X,2);
  
  // début de l'objet composite
  mfprintf(fid,"{\n");
  
  mfprintf(fid, 'CMESH\n%d %d\n', n2, n1);
  for i=1:n1
    for j=1:n2
      mfprintf(fid, '%15.7e %15.7e %15.7e  %8.5f %8.5f %8.5f 1\n', ...
      X(i,j) , Y(i,j) , Z(i,j), c(1), c(2), c(3));
    end
  end
  
  // fin de l'objet composite
  mfprintf(fid,"}\n");
  
endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet SPHERE de centre (x,y,z) et rayon r
function write_SPHERE(fid,x,y,z,r)
  
  // début de l'objet composite
  mfprintf(fid,"{\n");
  
  mfprintf(fid, 'SPHERE\n%15.7e %15.7e %15.7e %15.7e\n', r, x, y, z);
  
  // fin de l'objet composite
  mfprintf(fid,"}\n");
  
endfunction
//----------------------------------------------



