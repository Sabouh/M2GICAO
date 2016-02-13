/////////////////////////////////////////////////////////////////////////////
// module de définition de structure de données et algos.
/////////////////////////////////////////////////////////////////////////////

#ifndef _DATA_STRUCT_ALGO_H_
#define _DATA_STRUCT_ALGO_H_

#include<list>
#include<vector>
#include"geom.h"

using namespace std;

typedef double (*Fct3Var)(double,double,double);

struct Segment3D
{
	Point3D S0,S1;
};
typedef list<Segment3D> l_Segment3D;
struct Triangle3D
{
	Point3D S0,S1,S2;
};
typedef list<Triangle3D> l_Triangle3D;
typedef vector<Triangle3D> v_Triangle3D;

typedef list<unsigned int> l_int;


// grille réguliere composée de n_x * n_y * n_z rectangles
// correspondant au pavé [x_min,x_max] x [y_min,y_max] x [z_min,z_max]
class Grille3D
{
	double x_min, y_min, z_min; 
	double x_max, y_max, z_max; 
	unsigned int n_x;
	unsigned int n_y;
	unsigned int n_z;
	double x_pas, y_pas, z_pas;
public :
	// creation de la grille
	Grille3D(double xmin0, double ymin0, double zmin0, 
	         double xmax0, double ymax0, double zmax0,
	         unsigned int nx0, unsigned int ny0, unsigned int nz0);
	Grille3D();
	
	// fonction d'acces
	double xmin() {return x_min; }
	double xmax() {return x_max; }
	double ymin() {return y_min; }
	double ymax() {return y_max; }
	double zmin() {return z_min; }
	double zmax() {return z_max; }
	unsigned int nx() {return n_x; }
	unsigned int ny() {return n_y; }
	unsigned int nz() {return n_z; }
	double xpas() {return x_pas; }
	double ypas() {return y_pas; }
	double zpas() {return z_pas; }
	
	// renvoie l'ordonnée x correspondant à i
	double x(unsigned int i) {return x_min+i*x_pas; }
	// renvoie l'ordonnée y correspondant à j
	double y(unsigned int j) {return y_min+j*y_pas; }
	// renvoie l'ordonnée z correspondant à k
	double z(unsigned int k) {return z_min+k*z_pas; }
	// renvoie le point correspondant à (i,j,k)
	Point3D pt(unsigned int i, unsigned int j, unsigned int k) 
	{ return Point3D(x(i),y(j),z(k)); }
	// evalue une fonction f(x,y,z) sur la grille G
	// version où le tableau de valeurs est passé en parametre
	// et a déjà été alloué
	void eval(Fct3Var f, double* vf);
	// version où le tableau de valeurs est renvoyé
	// et alloué dynamiquement dans la procedure
	double* eval(Fct3Var f);
	
	// fonction de modif.
	void xmin(double xmin0);
	void xmax(double xmax0);
	void ymin(double ymin0);
	void ymax(double ymax0);
	void zmin(double zmin0);
	void zmax(double zmax0);
	void nx(unsigned int nx0);
	void ny(unsigned int ny0);
	void nz(unsigned int nz0);
};

// structure sommet 3D
struct Sommet3D
{
	Point3D p; // coordonnées
	double v;  // valeur associée
	unsigned long int m; // valeur utilisée pour certains algos 
};

#define TRIANGLE_VIDE 0xFFFFFFFF

// structure Triangle
struct Triangle
{
	unsigned int s1, s2, s3; // indices de ses trois sommets
	unsigned int t1, t2, t3; // indices de ses trois triangles adjacents
	// ti triangle adjacent suivant l'arete opposee à si
	// ti = TRIANGLE_VIDE s'il n'existe pas
};

// reorientation et calcul des triangles adjacents d'une triangulation
void reoriente(Triangle *T, unsigned int nT);

ostream& operator<<(ostream& o, Triangle& T);
void ecrire(ostream& o, Triangle *T, unsigned int nT);

#define TETRAEDRE_VIDE 0xFFFFFFFF

// structure Tétraèdre
struct Tetraedre
{
	unsigned int s1, s2, s3, s4; // indices de ses quatre sommets
	unsigned int t1, t2, t3, t4; // indices de ses quatre tetra. adjacents
	// ti tetra. adjacent suivant la face opposee à si
	// ti = TETRAEDRE_VIDE s'il n'existe pas
};

// triangulation dans l'espace
class Triangulation3D
{
	unsigned int nS; // nb de sommets 
	unsigned int nT; // nb de triangles
	Sommet3D *ts; // tableau des sommets
	Triangle *tt; // tableau des triangles
	void alloc_mem();
public :
	//// constructeurs-destructeur ////
	
	// triangulation vide
	Triangulation3D(); 
	// triangulation avec nS0 sommets et nT0 triangles
	Triangulation3D(unsigned int nS0, unsigned int nT0); 
	// triangulation à partir d'une structure existante
	// la structure est dupliquée
	Triangulation3D(Sommet3D *ts0, unsigned int nS0, 
	                Triangle *tt0, unsigned int nT0); 
	// copie
	Triangulation3D(const Triangulation3D& T);
	// destructeur
	~Triangulation3D();
	
	//// fonctions d'acces ////
	unsigned int nb_sommets() {return nS;}
	unsigned int nb_triangles() {return nT;}
	Sommet3D sommet(unsigned int i);
	Triangle triangle(unsigned int i);
	
	//// fonction de modif. ////
	Triangulation3D& operator=(const Triangulation3D& T);
	void sommet(unsigned int i, Sommet3D s0);
	void triangle(unsigned int i, Triangle t0);
	void reorientation() { reoriente(tt,nT); }
};

ostream& operator<<(ostream& o, Triangulation3D& T);

// tetraedrisation spatiale
class Tetraedrisation3D
{
	unsigned int nS; // nb de sommets 
	unsigned int nT; // nb de tetraedres
	Sommet3D *ts; // tableau des sommets
	Tetraedre *tt; // tableau des tetraedres
	void tetraedrisation_grille(Grille3D G);
public :
	//// constructeurs-destructeur ////
	
	// tetraedrisation vide
	Tetraedrisation3D(); 
	// tetraedrisation à partir d'une structure existante
	// la structure est dupliquée
	Tetraedrisation3D(Sommet3D *ts0, unsigned int nS0, 
	                Tetraedre *tt0, unsigned int nT0); 
	// tetraedrisation d'une grille 3D
	Tetraedrisation3D(Grille3D G);
	// tetraedrisation d'une grille 3D avec valeurs aux sommets 
	// données par une fonction f
	Tetraedrisation3D(Grille3D G, Fct3Var f);
	// tetraedrisation d'une grille 3D avec valeurs aux sommets 
	// données par un tableau de valeurs vf
	Tetraedrisation3D(Grille3D G, double *vf);
	// copie
	Tetraedrisation3D(const Tetraedrisation3D& T);
	// destructeur
	~Tetraedrisation3D();
	
	//// fonctions d'acces ////
	unsigned int nb_sommets() {return nS;}
	unsigned int nb_tetraedres() {return nT;}
	Sommet3D sommet(unsigned int i);
	Tetraedre tetraedre(unsigned int i);
	
	//// fonction de modif. ////
	Tetraedrisation3D& operator=(const Tetraedrisation3D& T);
	void sommet(unsigned int i, Sommet3D s0);
	void tetraedre(unsigned int i, Tetraedre t0);
};

// noeud d'un graphe 
struct Noeud
{
	l_int la; // liste des arcs incidents
	unsigned long int m; // utilisé dans certains algos.
	
	void ajouter_arc(unsigned int a) {la.push_back(a);}
};
ostream& operator<<(ostream& o, const Noeud& n);

typedef vector<Noeud> v_Noeud;

ostream& operator<<(ostream& o, const v_Noeud& vn);

// arc d'un graphe valué
struct Arc
{
	unsigned int n1,n2; // indices des deux noeuds de l'arc avec n1 < n2
	double v; // valeur associée
	unsigned long int m; // utilisé dans certains algos.
};
ostream& operator<<(ostream& o, const Arc& a);

typedef vector<Arc> v_Arc;

ostream& operator<<(ostream& o, const v_Arc& va);
ostream& operator<<(ostream& o, const l_int& la);

// graphe valué non orienté
class Graphe
{
	v_Noeud vn;  // tableau des noeuds
	v_Arc va;      // tableau des arcs
public :
	//// constructeurs-destructeur ////
	
	// graphe avec nn0 noeuds et aucun arc
	Graphe(unsigned int nn0);
	Graphe(const Graphe& G);
	~Graphe();
	
	//// fonctions d'acces ////
	unsigned int nb_noeuds() {return vn.size();};
	unsigned int nb_arcs() {return va.size();};
	Noeud& noeud(unsigned int i);
	Arc& arc(unsigned int i);
	v_Noeud& noeuds() {return vn;}
	v_Arc& arcs() {return va;}
	void put();
	friend ostream& operator<<(ostream& o, const Graphe& G);
	// calcul d'un arbre couvrant minimal
	Graphe arbre_couvrant_minimal();
	
	//// fonctions de modif. ////
	unsigned int ajouter_arc(unsigned int n1, unsigned int n2, double v=0.0);
};


#endif
