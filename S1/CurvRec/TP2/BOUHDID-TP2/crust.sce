function Crust(S)
  // **** A MODIFIER/COMPLETER ****
  // R should contain a list of pair of indices connecting the initial points in S
  // for instance: 
  // R = [R ; [1,2]] adds the edge connecting points 1 and 2 in the array R
  R = [];
  
  // ******************************
  
  //Compute the Delaunay triangulation T
  
  //Compute the set C of the circumscribed circle assiociated to triangles T
  [T,C,r] = delaunay(S);
  
  
  //On retrouve sommets correspondant aux indices dans T
  // et on met leur coordonnées ds une matrice ST
  //Matrice STI se souvient de l'indice qui correspond à S pour chaque sommet de ST
  
  ST = [];
  STI =[];
  [lig,col] = size(T);
  for i=1:lig
      for j=1:col
        ST((i-1)*col+j,:) =  S(T(i,j),:);
        STI((i-1)*col+j,:) = T(i,j);
      end
  end
  
  
  //S1 : T U C
  S1 = [ST; C];
  
  //Compute the Delaunay triangulation D of the set of points T U C
  [T1,C1,r] = delaunay(S1);
  
  //On veut retrouver les arêtes des triangles de T1 
  //mais on ne veut pas que les centres de cercles soient pris en compte 

  [lig,col] = size(ST);
  [ligne,colo] = size(T1);
  
  //Pour chaque triangle (chaque ligne de t1)
  //Fonction aretes définie à la fin du fichier : 
  //On parcourt les indices de ce triangle
  // Si l'indice est plus grand que nT de ST (ca veut dire que c est cercle)
  //on ne le prend pas en compte
  //sinon on ajoute le lien entre les 2 autres si aucun ne comprend un cercle

 
 for i=1:ligne
     R = [R ; aretes(STI,T1(i,:),lig)];
 end
 
 //fin
  
  // nouvelle figure 
  scf();
  
  // affichage des points en entrée
  plot(S(:,1),S(:,2),'ro');
  
  // affichage des connections
  for i=1:size(R,1)
    plot([S(R(i,1),1);S(R(i,2),1)],[S(R(i,1),2);S(R(i,2),2)],'b-');
  end
  
  set(gca(),"auto_clear","off")
  set(gca(),"margins",[0.05 , 0.05 , 0.05 , 0.05])
  set(gca(),"box","off")
  set(gca(),"isoview","on")
  set(gca(),"auto_scale","on")
endfunction


function test_crust(num_test)
  exec("qdelaunay.sce",-1);
  
  if argn(2)<1
    num_test=0;
  end
  select num_test
  case 1
    // // test 1
    // les donnees
    S = [0 0; 2 0; 4 0; 0 2; 1 2; 3 2; 2 3];
    Crust(S);
  case 2 then
    // // test 2 − points aleatoires
    S = rand(100,2);
    Crust(S);
  case 3 then
    // // test 3 − points suivant une grille reguliere
    S = zeros(100,2);
    for i =0:9
      for j =1:10
        S(i*10+j,:) = [2*j+i 2*i];
      end
    end
    Crust(S);
  case 4 then
    // // test 4 − points entres à la souris
    S = inputpoints()';
    Crust(S);
    
  end // select
endfunction

//Renvoie les aretes d'un triangle
// tels que les valeurs d'extremites de ces arêtes ne depassent pas N
//

function R=aretes(S,T,N)
    
    R = [];
    a = T(1);
    b = T(2);
    c = T(3);
    if ((a>=N & b>=N) |(a>=N & c>=N) | (b>=N & c>=N)) then
        //Si 2 sur les 3 est supérieur à N on renvoie rien
    else
        //
        if (a>=N) then
            R = [R ; [S(b),S(c)]];
        else
          if (b >=N) then
            R = [R ; [S(a),S(c)]];
          else
            R = [R ; [S(a),S(b)]];
          end
        end
            
    end
    
endfunction
