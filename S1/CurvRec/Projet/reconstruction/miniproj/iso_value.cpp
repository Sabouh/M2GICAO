/////////////////////////////////////////////////////////////////////////////
// module de calcul de courbe / surface isovaleur
/////////////////////////////////////////////////////////////////////////////

#include<cstdio>
#include<vector>
#include<map>
#include"geom.h"
#include"data_struct_algo.h"
#include"iso_value.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////
// classe SurfaceIsovaleurGrille
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// pour deux points (P1,v1) et (P2,v2) de R^3 x R avec v1<v<=v2 ou v2<v<=v1
// calcule le point I de R^3 tel que (I,0) sur le segment [(P1,v1),(P2,v2)]
Point3D intersection(Point3D P1, double v1,  Point3D P2, double v2, double v)
{
	double l = (v-v1)/(v2-v1);
	return (1-l)*P1+l*P2;
}



////////////////////////////////////////////////////////////////////////////
// ajouter un triangle-intersection [A,B,C] dans le v_Triangle3D vt
// avec A sur l'arete [S0,S1], 
//      B sur l'arete [S0,S2] 
//      C sur l'arete [S0,S3]
void SurfaceIsovaleurGrille::ajouter_triangle1(v_Triangle3D &vt, 
  Point3D S0, double v0, 
  Point3D S1, double v1, 
  Point3D S2, double v2, 
  Point3D S3, double v3)
{				
	Triangle3D T;
	
	T.S0 = intersection(S0,v0,S1,v1,v);
	T.S1 = intersection(S0,v0,S2,v2,v);
	T.S2 = intersection(S0,v0,S3,v3,v);
	vt.push_back(T);
}	

////////////////////////////////////////////////////////////////////////////
// ajouter deux triangles-intersection [A,B,C] et [D,C,B] 
// dans le v_Triangle3D vt
// avec A sur l'arete [S1,S2], 
//      B sur l'arete [S0,S2] 
//      C sur l'arete [S1,S3]
//      D sur l'arete [S0,S3]
void SurfaceIsovaleurGrille::ajouter_triangle2(v_Triangle3D &vt, 
  Point3D S0, double v0, 
  Point3D S1, double v1, 
  Point3D S2, double v2, 
  Point3D S3, double v3)
{
	Point3D A,B,C,D;				
	Triangle3D T;
	
	A = intersection(S1,v1,S2,v2,v);
	B = intersection(S0,v0,S2,v2,v);
	C = intersection(S1,v1,S3,v3,v);
	D = intersection(S0,v0,S3,v3,v);
	T.S0 = A; T.S1 = B; T.S2 = C; 
	vt.push_back(T);
	T.S0 = D; T.S1 = C; T.S2 = B; 
	vt.push_back(T);
}	


// les décompositions en cinq tetraedres 
const unsigned int PAVE_5TETRA_1[5][4] = 
  {{0,2,3,6},{3,5,6,7},{0,3,5,6},{0,4,5,6},{0,1,3,5}};
const unsigned int PAVE_5TETRA_2[5][4] = 
  {{1,4,5,7},{0,1,2,4},{1,2,4,7},{1,2,3,7},{2,4,6,7}};

// les décompositions en six tetraedres 
const unsigned int PAVE_6TETRA[6][4] = 
  {{2,3,4,6},{3,5,6,7},{3,4,5,6},{1,2,3,4},{1,3,4,5},{0,1,2,4}};
// const unsigned int PAVE_6TETRA[6][4] = 
//   {{0,1,3,5},{0,3,5,4},{7,3,4,5},{2,3,4,7},{6,2,4,7},{0,3,2,4}};

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
// La routine renvoie la surface isovaleur 
// sous forme d'un vector de Triangle3D
void SurfaceIsovaleurGrille::surface_isovaleur
  (v_Triangle3D &vt, Grille3D G, double *vf, double v0, bool *ind)
{
	v = v0;
	
	// dimension de la grille
	unsigned int nx = G.nx();
	unsigned int ny = G.ny();
	unsigned int nz = G.nz();
	
	// traitement de tous les pavés de la grille g
	for (unsigned int k=0; k<nz; k++)
	for (unsigned int j=0; j<ny; j++)
	for (unsigned int i=0; i<nx; i++)
	{
		// indice1D des 8 sommets
		unsigned int indice1D[8];
		indice1D[0] = i+(nx+1)*(j+(ny+1)*k);
		indice1D[1] = indice1D[0]+1;
		indice1D[2] = indice1D[0]+nx+1;
		indice1D[3] = indice1D[2]+1;
		indice1D[4] = indice1D[0]+(nx+1)*(ny+1);
		indice1D[5] = indice1D[4]+1;
		indice1D[6] = indice1D[4]+nx+1;
		indice1D[7] = indice1D[6]+1;
		
		// déterminer si les 8 sommets sont actifs
		if (ind)
		{
			if (
			  !ind[indice1D[0]] ||
			  !ind[indice1D[1]] ||
			  !ind[indice1D[2]] ||
			  !ind[indice1D[3]] ||
			  !ind[indice1D[4]] ||
			  !ind[indice1D[5]] ||
			  !ind[indice1D[6]] ||
			  !ind[indice1D[7]])
			  	// si non passer au pavé suivant
				continue;
		}
		
		// traitement du pavé (i,j,k) = [x1,x2] x [y1,y2] x [z1,z2]
		double x1 = G.x(i);
		double x2 = G.x(i+1);
		double y1 = G.y(j);
		double y2 = G.y(j+1);
		double z1 = G.z(k);
		double z2 = G.z(k+1);

		// coordonnees des 8 sommets
		Point3D s[8] = {Point3D(x1,y1,z1),Point3D(x2,y1,z1),
		                Point3D(x1,y2,z1),Point3D(x2,y2,z1),
						Point3D(x1,y1,z2),Point3D(x2,y1,z2),
		                Point3D(x1,y2,z2),Point3D(x2,y2,z2)};

		// valeurs et marques aux huit sommets
		double vs[8];
		bool marques[8];
		for (int m=0; m<8; m++) 
		{
			vs[m] = vf[indice1D[m]];
			marques[m] = vs[m]<v;
		}
		
// 		// la décomposition en cinq tetraedres suivant la parité de i+j+k 
// 		unsigned int t[5][4];
// 		bool ijkpair = (i+j)%2==0;
// 		for (int i1=0; i1<5; i1++)
// 		for (int i2=0; i2<4; i2++)
// 			t[i1][i2] = ijkpair ? 
//			            PAVE_5TETRA_1[i1][i2] : PAVE_5TETRA_2[i1][i2];
// 		
// 		for (unsigned char l=0; l<5; l++)
		// la décomposition en six tetraedres 
		unsigned int t[6][4];
		for (int i1=0; i1<6; i1++)
		for (int i2=0; i2<4; i2++)
			t[i1][i2] = PAVE_6TETRA[i1][i2];
		
		for (unsigned char l=0; l<6; l++)
		{
			// traitement du tetraedre numero l
			unsigned int i0 = t[l][0];
			unsigned int i1 = t[l][1];
			unsigned int i2 = t[l][2];	
			unsigned int i3 = t[l][3];	
			// valeurs des quatre sommets de la grille	
			double v0 = vs[i0];
			double v1 = vs[i1];
			double v2 = vs[i2];
			double v3 = vs[i3];
			// marques des quatre sommets de la grille	
			bool m0 = marques[i0];
			bool m1 = marques[i1];
			bool m2 = marques[i2];
			bool m3 = marques[i3];
			// coordonnées des quatre sommets de la grille	
			Point3D S0 = s[i0];
			Point3D S1 = s[i1];
			Point3D S2 = s[i2];
			Point3D S3 = s[i3];
			
			
			// configuration avec 3 intersections -> 1 triangle
			if (( m0 && !m1 && !m2 && !m3) ||
			    (!m0 &&  m1 &&  m2 &&  m3))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S0,S1]
				// B sur l'arete [S0,S2]
				// C sur l'arete [S0,S3]
				ajouter_triangle1(vt, S0,v0 , S1,v1 , S2,v2 , S3,v3);
			}
			
			if (( m1 && !m2 && !m3 && !m0) ||
			    (!m1 &&  m2 &&  m3 &&  m0))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S1,S2]
				// B sur l'arete [S1,S3]
				// C sur l'arete [S1,S0]
				ajouter_triangle1(vt, S1,v1 , S2,v2 , S3,v3 , S0,v0);
			}
			
			if (( m2 && !m3 && !m0 && !m1) ||
			    (!m2 &&  m3 &&  m0 &&  m1))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S2,S3]
				// B sur l'arete [S2,S0]
				// C sur l'arete [S2,S1]
				ajouter_triangle1(vt, S2,v2 , S3,v3 , S0,v0 , S1,v1);
			}
			
			if (( m3 && !m0 && !m1 && !m2) ||
			    (!m3 &&  m0 &&  m1 &&  m2))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S3,S0]
				// B sur l'arete [S3,S1]
				// C sur l'arete [S3,S2]
				ajouter_triangle1(vt, S3,v3 , S0,v0 , S1,v1 , S2,v2);
			}
			
			// configuration avec 4 intersections -> 2 triangles
			if (( m0 &&  m1 && !m2 && !m3) ||
			    (!m0 && !m1 &&  m2 &&  m3))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S1,S2]
				// B sur l'arete [S0,S2]
				// C sur l'arete [S0,S3]
				// D sur l'arete [S1,S3]
				ajouter_triangle2(vt, S0,v0 , S1,v1 , S2,v2 , S3,v3);
			}

			if (( m0 &&  m2 && !m3 && !m1) ||
			    (!m0 && !m2 &&  m3 &&  m1))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S2,S3]
				// B sur l'arete [S0,S3]
				// C sur l'arete [S0,S1]
				// D sur l'arete [S2,S1]
				ajouter_triangle2(vt, S0,v0 , S2,v2 , S3,v3 , S1,v1);
			}

			if (( m0 &&  m3 && !m1 && !m2) ||
			    (!m0 && !m3 &&  m1 &&  m2))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S3,S1]
				// B sur l'arete [S0,S1]
				// C sur l'arete [S0,S2]
				// D sur l'arete [S3,S2]
				ajouter_triangle2(vt, S0,v0 , S3,v3 , S1,v1 , S2,v2);
			}

		}
	}
}

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
// La routine renvoie la surface isovaleur 
// sous forme d'un vector de Triangle3D
void SurfaceIsovaleurGrille::surface_isovaleur
  (v_Triangle3D &vt, Grille3D G, Fct3Var f, double v0, bool *ind)
{
	v = v0;
	
	// dimension de la grille
	unsigned int nx = G.nx();
	unsigned int ny = G.ny();
	unsigned int nz = G.nz();
	
	// traitement de tous les pavés de la grille g
	for (unsigned int k=0; k<nz; k++)
	for (unsigned int j=0; j<ny; j++)
	for (unsigned int i=0; i<nx; i++)
	{
		// indice1D des 8 sommets
		unsigned int indice1D[8];
		indice1D[0] = i+(nx+1)*(j+(ny+1)*k);
		indice1D[1] = indice1D[0]+1;
		indice1D[2] = indice1D[0]+nx+1;
		indice1D[3] = indice1D[2]+1;
		indice1D[4] = indice1D[0]+(nx+1)*(ny+1);
		indice1D[5] = indice1D[4]+1;
		indice1D[6] = indice1D[4]+nx+1;
		indice1D[7] = indice1D[6]+1;
		
		// déterminer si les 8 sommets sont actifs
		if (ind)
		{
			if (
			  !ind[indice1D[0]] ||
			  !ind[indice1D[1]] ||
			  !ind[indice1D[2]] ||
			  !ind[indice1D[3]] ||
			  !ind[indice1D[4]] ||
			  !ind[indice1D[5]] ||
			  !ind[indice1D[6]] ||
			  !ind[indice1D[7]])
			  	// si non passer au pavé suivant
				continue;
		}
		
		// traitement du pavé (i,j,k) = [x1,x2] x [y1,y2] x [z1,z2]
		double x1 = G.x(i);
		double x2 = G.x(i+1);
		double y1 = G.y(j);
		double y2 = G.y(j+1);
		double z1 = G.z(k);
		double z2 = G.z(k+1);

		// coordonnees des 8 sommets
		Point3D s[8] = {Point3D(x1,y1,z1),Point3D(x2,y1,z1),
		                Point3D(x1,y2,z1),Point3D(x2,y2,z1),
						Point3D(x1,y1,z2),Point3D(x2,y1,z2),
		                Point3D(x1,y2,z2),Point3D(x2,y2,z2)};

		// valeurs et marques aux huit sommets
		double vs[8];
		bool marques[8];
		for (int m=0; m<8; m++) 
		{
			vs[m] = f(s[m].x,s[m].y,s[m].z);
			marques[m] = vs[m]<v;
		}
		
		// la décomposition en cinq tetraedres suivant la parité de i+j+k 
		unsigned int t[5][4];
		bool ijkpair = (i+j+k)%2==0;
		for (int i1=0; i1<5; i1++)
		for (int i2=0; i2<4; i2++)
			t[i1][i2] = ijkpair ? 
			            PAVE_5TETRA_1[i1][i2] : PAVE_5TETRA_2[i1][i2];
		
		for (unsigned char l=0; l<5; l++)

// 		// la décomposition en six tetraedres 
// 		unsigned int t[6][4];
// 		for (int i1=0; i1<6; i1++)
// 		for (int i2=0; i2<4; i2++)
// 			t[i1][i2] = PAVE_6TETRA[i1][i2];
// 		
// 		for (unsigned char l=0; l<6; l++)
		{
			// traitement du tetraedre numero l
			unsigned int i0 = t[l][0];
			unsigned int i1 = t[l][1];
			unsigned int i2 = t[l][2];	
			unsigned int i3 = t[l][3];	
			// valeurs des quatre sommets de la grille	
			double v0 = vs[i0];
			double v1 = vs[i1];
			double v2 = vs[i2];
			double v3 = vs[i3];
			// marques des quatre sommets de la grille	
			bool m0 = marques[i0];
			bool m1 = marques[i1];
			bool m2 = marques[i2];
			bool m3 = marques[i3];
			// coordonnées des quatre sommets de la grille	
			Point3D S0 = s[i0];
			Point3D S1 = s[i1];
			Point3D S2 = s[i2];
			Point3D S3 = s[i3];
			
			
			// configuration avec 3 intersections -> 1 triangle
			if (( m0 && !m1 && !m2 && !m3) ||
			    (!m0 &&  m1 &&  m2 &&  m3))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S0,S1]
				// B sur l'arete [S0,S2]
				// C sur l'arete [S0,S3]
				ajouter_triangle1(vt, S0,v0 , S1,v1 , S2,v2 , S3,v3);
			}
			
			if (( m1 && !m2 && !m3 && !m0) ||
			    (!m1 &&  m2 &&  m3 &&  m0))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S1,S2]
				// B sur l'arete [S1,S3]
				// C sur l'arete [S1,S0]
				ajouter_triangle1(vt, S1,v1 , S2,v2 , S3,v3 , S0,v0);
			}
			
			if (( m2 && !m3 && !m0 && !m1) ||
			    (!m2 &&  m3 &&  m0 &&  m1))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S2,S3]
				// B sur l'arete [S2,S0]
				// C sur l'arete [S2,S1]
				ajouter_triangle1(vt, S2,v2 , S3,v3 , S0,v0 , S1,v1);
			}
			
			if (( m3 && !m0 && !m1 && !m2) ||
			    (!m3 &&  m0 &&  m1 &&  m2))
			{
				// ajouter un triangle-intersection [A,B,C] avec 
				// A sur l'arete [S3,S0]
				// B sur l'arete [S3,S1]
				// C sur l'arete [S3,S2]
				ajouter_triangle1(vt, S3,v3 , S0,v0 , S1,v1 , S2,v2);
			}
			
			// configuration avec 4 intersections -> 2 triangles
			if (( m0 &&  m1 && !m2 && !m3) ||
			    (!m0 && !m1 &&  m2 &&  m3))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S1,S2]
				// B sur l'arete [S0,S2]
				// C sur l'arete [S0,S3]
				// D sur l'arete [S1,S3]
				ajouter_triangle2(vt, S0,v0 , S1,v1 , S2,v2 , S3,v3);
			}

			if (( m0 &&  m2 && !m3 && !m1) ||
			    (!m0 && !m2 &&  m3 &&  m1))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S2,S3]
				// B sur l'arete [S0,S3]
				// C sur l'arete [S0,S1]
				// D sur l'arete [S2,S1]
				ajouter_triangle2(vt, S0,v0 , S2,v2 , S3,v3 , S1,v1);
			}

			if (( m0 &&  m3 && !m1 && !m2) ||
			    (!m0 && !m3 &&  m1 &&  m2))
			{
				// ajouter deux triangles-intersection [A,B,C] et [D,C,B]
				// A sur l'arete [S3,S1]
				// B sur l'arete [S0,S1]
				// C sur l'arete [S0,S2]
				// D sur l'arete [S3,S2]
				ajouter_triangle2(vt, S0,v0 , S3,v3 , S1,v1 , S2,v2);
			}

		}
	}
}

