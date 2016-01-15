function Crust(S)
  // **** A MODIFIER/COMPLETER ****
  // R should contain a list of pair of indices connecting the initial points in S
  // for instance: 
  // R = [R ; [1,2]] adds the edge connecting points 1 and 2 in the array R
  R = [];
  
  // ******************************
  
  [T,C,r] = qdelaunay(S);
    
  
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
