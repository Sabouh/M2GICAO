/////////////////////////////////////////////////////////////////////////////
// module de calcul de courbe / surface isovaleur
/////////////////////////////////////////////////////////////////////////////

#ifndef _ISO_VALUE_H_
#define _ISO_VALUE_H_

#include<vector>
#include<list>
#include<map>
#include"geom.h"
#include"data_struct_algo.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////
// classe pour calculer une surface isovaleur sur une grille3D
//////////////////////////////////////////////////////////////////////////
class SurfaceIsovaleurGrille
{
	double v; // l'isovaleur
// 	map_Intersection M;
// 	v_TriangleIsovaleur VT;
// 	v_Intersection3D VI;

	////////////////////////////////////////////////////////////////////////////
	// ajouter un triangle-intersection [A,B,C] dans le v_Triangle3D vt
	// avec A sur l'arete [S0,S1], 
	//      B sur l'arete [S0,S2] 
	//      C sur l'arete [S0,S3]
	void ajouter_triangle1(v_Triangle3D &vt, 
	  Point3D S0, double v0, 
	  Point3D S1, double v1, 
	  Point3D S2, double v2, 
	  Point3D S3, double v3);

	////////////////////////////////////////////////////////////////////////////
	// ajouter deux triangles-intersection [A,B,C] et [D,C,B] 
	// dans le v_Triangle3D vt
	// avec A sur l'arete [S1,S2], 
	//      B sur l'arete [S0,S2] 
	//      C sur l'arete [S0,S3]
	//      D sur l'arete [S1,S3]
	void ajouter_triangle2(v_Triangle3D &vt, 
	  Point3D S0, double v0, 
	  Point3D S1, double v1, 
	  Point3D S2, double v2, 
	  Point3D S3, double v3);
  
public :
	//// constructeur ////
	SurfaceIsovaleurGrille() {}
		
	// calcul d'une approximation de la surface isovaleur {f(x,y,z)=v0} 
	// sur la grille G0 uniquement sur les paves dont les sommets
	// sont tous marqués
	// Un sommet (i,j,k) est marqué <==> ind[i+DIM_X*(j+DIM_Y*k)] = true 
	//   avec DIM_X = G.nx()+1 et DIM_Y = G.ny()+1
	// Entrée : vt = vector de Triangle3
	//          G0 = la grille3D
	//          f = fonction de R^3 dans R
	//          v0 = l'iso-valeur 
	//          ind : tableau de booleens (par défaut ind=NULL)
	//            ind=NULL ==> tous les sommets sont pris en compte
	// La routine renvoie la surface isovaleur dans le vector de Triangle3D vt
	void surface_isovaleur (v_Triangle3D &vt, Grille3D G0, 
	  Fct3Var f, double v0, bool *ind = (bool*)0L);
	
	// calcul d'une approximation de la surface isovaleur {f(x,y,z)=v0} 
	// sur la grille G0 uniquement sur les paves dont les sommets
	// sont tous marqués
	// Un sommet (i,j,k) est marqué <==> ind[i+DIM_X*(j+DIM_Y*k)] = true 
	//   avec DIM_X = G.nx()+1 et DIM_Y = G.ny()+1
	// Entrée : vt = vector de Triangle3
	//          G0 = la grille3D
	//          vf = valeur de la fonction aux sommets de la grille
	//            vf[i+DIM_X*(j+DIM_Y*k)] = f(G.x(i),G.y(j),G.z(k))
	//          v0 = l'iso-valeur 
	//          ind : tableau de booleens (par défaut ind=NULL)
	//            ind=NULL ==> tous les sommets sont pris en compte
	// La routine renvoie la surface isovaleur dans le vector de Triangle3D vt
	void surface_isovaleur(v_Triangle3D &vt, Grille3D G, 
	  double *vf, double v0, bool *ind = (bool*)0L);
};

#endif
	
