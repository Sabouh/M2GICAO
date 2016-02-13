/////////////////////////////////////////////////////////////////////////////
// module de définition de structure de données et algos.
/////////////////////////////////////////////////////////////////////////////

#include"data_struct_algo.h"
#include<cstdlib>
#include<stack>
#include<algorithm>
#include<list>
#include<vector>



//////////////////////////////////////////////////////////////////////////////
// struct Triangle
//////////////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream& o, Triangle& T)
{
  o << "[" << T.s1 << "," << T.s2 << "," << T.s3;
  o << " ; " << (int)T.t1 << "," << (int)T.t2 << "," << (int)T.t3 << "]";
  return o;
}

void ecrire(ostream& o, Triangle *T, unsigned int nT)
{
  o << "Triangle[" << nT << "] : " << endl; 
  for (unsigned int i=0; i<nT; i++)
    o << " " << T[i] << endl;
}

// reorientation et calcul des triangles adjacents d'une triangulation
struct DemiArete
{
  unsigned int s1, s2; // les deux sommets de la demi-arete avec s1<s2
  unsigned int t;      // le triangle incident à la demi-arete
  unsigned int ta;     // l'eventuel triangle adjacent
  char m;              // marque pour les algos.
};

ostream& operator<<(ostream& o, DemiArete& da)
{
  o << "["   << (int)da.s1 << "," << (int)da.s2;
  o << " ; " << (int)da.t << "," << (int)da.ta << "] "; 
  o << (da.m==0 ? -1 : (int)da.m);
  return o;
}

void ecrire(ostream& o, DemiArete *DA, unsigned int nDA)
{
  o << "DemiArete[" << nDA << "] : " << endl; 
  for (unsigned int i=0; i<nDA; i++)
    o << " " << DA[i] << endl;
}

struct TriangleInfo
{
  unsigned int s[3]; // les 3 sommets
  unsigned int a[3]; // les 3 aretes, a[i] opposee à s[i]
  unsigned int t[3]; // les 3 triangles adjacents, t[i] adjacent suivant a[i]
  char m;              // marque pour les algos.	
  // pour deux sommets sa1 et sa2 du triangle, 
  // détermine l'indice i du troisieme sommet
  // et a[i] prend la valeur num  
  unsigned int Arete(unsigned int sa1, unsigned int sa2, unsigned int num);
};

ostream& operator<<(ostream& o, TriangleInfo& ti)
{
  o << "["   << (int)ti.s[0] << "," << (int)ti.s[1] << "," << (int)ti.s[2];
  o << " ; " << (int)ti.a[0] << "," << (int)ti.a[1] << "," << (int)ti.a[2];
  o << " ; " << (int)ti.t[0] << "," << (int)ti.t[1] << "," << (int)ti.t[2];
  o << "] " << (ti.m==0 ? -1 : (int)ti.m);
  return o;
}

void ecrire(ostream& o, TriangleInfo *TI, unsigned int nTI)
{
  o << "TriangleInfo[" << nTI << "] : " << endl; 
  for (unsigned int i=0; i<nTI; i++)
    o << " " << TI[i] << endl;
}


unsigned int TriangleInfo::Arete
(unsigned int sa1, unsigned int sa2, unsigned int num)
{
  unsigned int res = 0;
  for (unsigned int i = 0; i<3; i++)
    if (s[i]!=sa1 && s[i]!=sa2)
      {
	a[i] = num; res = i; break;
      }
  return res;
}

// routine de comparaison pour les demi-aretes
int compar_DemiArete(const void *p1, const void *p2)
{
  DemiArete* pa1 = (DemiArete*)p1;
  DemiArete* pa2 = (DemiArete*)p2;
  if (pa1->s1 < pa2->s1)
    return -1;
  if (pa1->s1 > pa2->s1)
    return  1;
  if (pa1->s2 < pa2->s2)
    return -1;
  if (pa1->s2 > pa2->s2)
    return  1;
  return 0;
}

template<class T> void permute(T& x, T& y)
{
  T r;
  r = x; x = y; y = r;
}

// reorientation de la triangulation T avec calcul des triangles adjacents
void reoriente(Triangle *T, unsigned int nT)
{
  DemiArete DA[3*nT];
  TriangleInfo TI[nT];
	
  // lister les demi-aretes
  for (unsigned int i = 0; i<nT; i++)
    {
      unsigned int s0, s1, s2;
      s0 = TI[i].s[0] = T[i].s1;
      s1 = TI[i].s[1] = T[i].s2;
      s2 = TI[i].s[2] = T[i].s3;
		
      DA[3*i  ].s1 = s0<s1 ? s0 : s1;
      DA[3*i  ].s2 = s0>s1 ? s0 : s1;
      DA[3*i+1].s1 = s0<s2 ? s0 : s2;
      DA[3*i+1].s2 = s0>s2 ? s0 : s2;
      DA[3*i+2].s1 = s1<s2 ? s1 : s2;
      DA[3*i+2].s2 = s1>s2 ? s1 : s2;
      DA[3*i  ].t  = i;
      DA[3*i+1].t  = i;
      DA[3*i+2].t  = i;
    }
	
  // trier les demi-aretes
  qsort(&DA[0], 3*nT, sizeof(DemiArete), compar_DemiArete);
	
  // faire correspondre les TriangleInfo et les demi-aretes
  for (unsigned int i = 0; i<3*nT; i++)
    {
      unsigned int s1 = DA[i].s1;
      unsigned int s2 = DA[i].s2;
      unsigned int t  = DA[i].t;
      DA[i].m = TI[t].Arete(s1,s2,i);
    }
	
  // determiner les triangles adjacents
  unsigned int k = 0;
  while (k<3*nT)
    {
      if (DA[k].s1 == DA[k+1].s1 && DA[k].s2 == DA[k+1].s2)
	{
	  DA[k  ].ta = DA[k+1].t;
	  DA[k+1].ta = DA[k  ].t;
	  k+=2;
	}
      else
	{
	  DA[k  ].ta = TRIANGLE_VIDE;
	  k++;
	}
    }
  if (k<3*nT-1)
    {
      DA[k  ].ta = TRIANGLE_VIDE;
      k++;
    }
	
  // initialiser le reordonnancement
  for (unsigned int i = 0; i<nT; i++)
    {
      TI[i].m = 0;
      for (unsigned int j = 0; j<3; j++)
	TI[i].t[j] = TRIANGLE_VIDE;
    }

  // cout << endl << "reoriente : avant la boucle" << endl;
  // ecrire(cout, DA, 3*nT);
  // ecrire(cout, TI, nT);
	
  stack<unsigned int> P;
	
  // empiler le triangle 0
  P.push(0);
  TI[0].m = 1;
	
  while (!P.empty())
    {
      // depiler le triangle t
      unsigned int t = P.top();
      P.pop();
		
      for (unsigned int j = 0; j<3; j++)
	{
	  // trouver l'eventuel triangle ta adjacent à t suivant a[j]
	  if (TI[t].t[j] == TRIANGLE_VIDE)
	    {
	      unsigned int a = TI[t].a[j];
	      unsigned int ta = DA[a].ta;
	      if (ta != TRIANGLE_VIDE)
		{
		  TI[t].t[j] = ta;
					
		  // empiler le triangle ta si cela n'a pas ete fait
		  if (TI[ta].m == 0)
		    {
		      P.push(ta);
		      TI[ta].m = 1;
		    }
					
		  // reorientation eventuelle de ta
		  unsigned sa1 = TI[t].s[(j+1)%3]; // sommet initial de a
		  unsigned sa2 = TI[t].s[(j+2)%3]; // sommet final de a
		  if (
		      (TI[ta].s[0]==sa1 && TI[ta].s[1]==sa2) ||
		      (TI[ta].s[1]==sa1 && TI[ta].s[2]==sa2) ||
		      (TI[ta].s[2]==sa1 && TI[ta].s[0]==sa2)
		      )
		    {
		      // permuter les sommets 0 et 1 du triangle ta
		      permute<unsigned int>(TI[ta].s[0],TI[ta].s[1]);
		      permute<unsigned int>(TI[ta].a[0],TI[ta].a[1]);
		      permute<unsigned int>(TI[ta].t[0],TI[ta].t[1]);
		      DA[TI[ta].a[0]].m = 0;
		      DA[TI[ta].a[1]].m = 1;
		    }
		}
	    }
	}
      // cout << endl << "reoriente : apres traitement triangle " << t << endl;
      // ecrire(cout, DA, 3*nT);
      // ecrire(cout, TI, nT);
    }
	
  // copier TI dans T
  for (unsigned int i = 0; i<nT; i++)
    {
      T[i].s1 = TI[i].s[0];
      T[i].s2 = TI[i].s[1];
      T[i].s3 = TI[i].s[2];
      T[i].t1 = TI[i].t[0];
      T[i].t2 = TI[i].t[1];
      T[i].t3 = TI[i].t[2];
    }
}


//////////////////////////////////////////////////////////////////////////////
// class Grille3D
//////////////////////////////////////////////////////////////////////////////

// creation de la grille
Grille3D::Grille3D(double xmin0, double ymin0, double zmin0, 
		   double xmax0, double ymax0, double zmax0,
		   unsigned int nx0, unsigned int ny0, unsigned int nz0)
{
  x_min = xmin0;
  y_min = ymin0;
  z_min = zmin0;
  x_max = xmax0;
  y_max = ymax0;
  z_max = zmax0;
  n_x = nx0;
  n_y = ny0;
  n_z = nz0;
  x_pas = (x_max-x_min)/n_x;
  y_pas = (y_max-y_min)/n_y;
  z_pas = (z_max-z_min)/n_z;
}

Grille3D::Grille3D()
{
  x_min = 0.0;
  y_min = 0.0;
  z_min = 0.0;
  x_max = 1.0;
  y_max = 1.0;
  z_max = 1.0;
  n_x = 1;
  n_y = 1;
  n_z = 1;
  x_pas = (x_max-x_min)/n_x;
  y_pas = (y_max-y_min)/n_y;
  z_pas = (z_max-z_min)/n_y;
}

// evalue une fonction f(x,y,z) sur la grille G
// version où le tableau de valeurs est passé en parametre
// et a déjà été alloué
void Grille3D::eval(Fct3Var f, double* vf)
{
  unsigned int l = 0;
  for (unsigned int k = 0; k<=n_z; k++)
    for (unsigned int j = 0; j<=n_y; j++)
      for (unsigned int i = 0; i<=n_x; i++)
	vf[l++] = f(x(i),y(j),z(k));
}

// version où le tableau de valeurs est renvoyé
// et alloué dynamiquement dans la procedure
double* Grille3D::eval(Fct3Var f)
{
  double *vf;
  vf = new double[(n_x+1)*(n_y+1)*(n_z+1)];
  eval(f, vf);
  return vf;
}

// fonction de modif.
void Grille3D::xmin(double xmin0)
{
  x_min = xmin0;
  x_pas = (x_max-x_min)/n_x;
}

void Grille3D::xmax(double xmax0)
{
  x_max = xmax0;
  x_pas = (x_max-x_min)/n_x;
}

void Grille3D::ymin(double ymin0)
{
  y_min = ymin0;
  y_pas = (y_max-y_min)/n_y;
}

void Grille3D::ymax(double ymax0)
{
  y_max = ymax0;
  y_pas = (y_max-y_min)/n_y;
}

void Grille3D::zmin(double zmin0)
{
  z_min = zmin0;
  z_pas = (z_max-z_min)/n_z;
}

void Grille3D::zmax(double zmax0)
{
  z_max = zmax0;
  z_pas = (z_max-z_min)/n_z;
}

void Grille3D::nx(unsigned int nx0)
{
  n_x = nx0;
  x_pas = (x_max-x_min)/n_x;
}

void Grille3D::ny(unsigned int ny0)
{
  n_y = ny0;
  y_pas = (y_max-y_min)/n_y;
}

void Grille3D::nz(unsigned int nz0)
{
  n_z = nz0;
  z_pas = (z_max-z_min)/n_z;
}

//////////////////////////////////////////////////////////////////////////////
// class Triangulation3D
//////////////////////////////////////////////////////////////////////////////


//// constructeurs-destructeur ////

// allocation memoire
void Triangulation3D::alloc_mem()
{
  if (nS>0)
    ts = new Sommet3D[nS];
  else
    ts = (Sommet3D*)(0L);
  if (nS>0)
    tt = new Triangle[nT];
  else
    tt = (Triangle*)(0L);
}

// triangulation vide
Triangulation3D::Triangulation3D()
{
  nS = nT = 0;
  alloc_mem();
}

// triangulation avec nS0 sommets et nT0 triangles
Triangulation3D::Triangulation3D(unsigned int nS0, unsigned int nT0)
{
  nS = nS0;
  nT = nT0;
	
  // allocation memoire
  alloc_mem();
}
 
// triangulation à partir d'une structure existante
// la structure est dupliquée
Triangulation3D::Triangulation3D(Sommet3D *ts0, unsigned int nS0, 
				 Triangle *tt0, unsigned int nT0)
{
  nS = nS0;
  nT = nT0;
	
  // allocation memoire
  alloc_mem();
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = ts0[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = tt0[i];
}

// copie
Triangulation3D::Triangulation3D(const Triangulation3D& T)
{
  nS = T.nS;
  nT = T.nT;
	
  // allocation memoire
  alloc_mem();
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = T.ts[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = T.tt[i];
	
}
// destructeur
Triangulation3D::~Triangulation3D()
{
  if (ts != (Sommet3D*)(0L)) delete[] ts;
  if (tt != (Triangle*)(0L)) delete[] tt;
}

//// fonctions d'acces ////
Sommet3D Triangulation3D::sommet(unsigned int i)
{
  return ts[i];
}
Triangle Triangulation3D::triangle(unsigned int i)
{
  return tt[i];
}

//// fonction de modif. ////
Triangulation3D& Triangulation3D::operator=(const Triangulation3D& T)
{
  if (ts != (Sommet3D*)(0L)) delete[] ts;
  if (tt != (Triangle*)(0L)) delete[] tt;
	
  nS = T.nS;
  nT = T.nT;
	
  // allocation memoire
  ts = new Sommet3D[nS];
  tt = new Triangle[nT];
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = T.ts[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = T.tt[i];
	
  return *this;
}

void Triangulation3D::sommet(unsigned int i, Sommet3D s0)
{
  ts[i] = s0;
}

void Triangulation3D::triangle(unsigned int i, Triangle t0)
{
  tt[i] = t0;
}

ostream& operator<<(ostream& o, Triangulation3D& T)
{
  o << "Triangulation3D : " << T.nb_sommets() << " sommets - ";
  o << T.nb_triangles() << " triangles" << endl;
  o << "Sommets :" << endl;
  for (unsigned int i=0; i<T.nb_sommets(); i++)
    {
      Sommet3D s = T.sommet(i);
      o << s.p << endl;
    }
  o << "Triangles :" << endl;
  for (unsigned int i=0; i<T.nb_triangles(); i++)
    {
      Triangle t = T.triangle(i);
      o << "[" << t.s1 << "," << t.s2 << "," << t.s3 << "]" << endl;
    }
  return o;
}

//////////////////////////////////////////////////////////////////////////////
// class Tetraedrisation3D
//////////////////////////////////////////////////////////////////////////////
//// constructeurs-destructeur ////

// triangulation vide
Tetraedrisation3D::Tetraedrisation3D()
{
  nS = nT = 0;
  ts = (Sommet3D*)(0L);
  tt = (Tetraedre*)(0L);
}
// triangulation à partir d'une structure existante
// la structure est dupliquée
Tetraedrisation3D::Tetraedrisation3D(Sommet3D *ts0, unsigned int nS0, 
				     Tetraedre *tt0, unsigned int nT0)
{
  nS = nS0;
  nT = nT0;
	
  // allocation memoire
  ts = new Sommet3D[nS];
  tt = new Tetraedre[nT];
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = ts0[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = tt0[i];
}

// tetraedrisation d'une grille 3D
void Tetraedrisation3D::tetraedrisation_grille(Grille3D G)
{
  unsigned int nx = G.nx();
  unsigned int ny = G.ny();
  unsigned int nz = G.ny();
  unsigned int l;
	
  // delta-indices pour les sommets
  unsigned int dsx = 1;
  unsigned int dsy = nx+1;
  unsigned int dsz = (nx+1)*(ny+1);
	
  // delta-indices pour les tetraedres
  unsigned int dtx = 6;
  unsigned int dty = 6*nx;
  unsigned int dtz = 6*nx*ny;
	
  nS = (nx+1)*(ny+1)*(nx+1);
  nT = 6*nx*ny*ny;
	
  // allocation memoire
  ts = new Sommet3D[nS];
  tt = new Tetraedre[nT];
	
  // les sommets
  l = 0;
  for (unsigned int k=0; k<=nz; k++)
    for (unsigned int j=0; j<=ny; j++)
      for (unsigned int i=0; i<=nx; i++)
	ts[l++].p = G.pt(i,j,k);
	
  // les tetraedres
  l = 0;
  for (unsigned int k=0; k<nz; k++)
    for (unsigned int j=0; j<ny; j++)
      for (unsigned int i=0; i<nx; i++)
	{
	  Tetraedre t1, t2, t3, t4, t5, t6;
		
	  // les indices des 8 sommets
	  unsigned int s1 = i+dsy*j+dsz*k;
	  unsigned int s2 = s1+dsx;
	  unsigned int s3 = s1+dsy;
	  unsigned int s4 = s3+dsx;
	  unsigned int s5 = s1+dsz;
	  unsigned int s6 = s5+dsx;
	  unsigned int s7 = s5+dsy;
	  unsigned int s8 = s7+dsx;
		
	  //  Tetraedre 1 = [s1,s2,s4,s6]
	  //  Tetraedre 2 = [s1,s4,s6,s5]
	  //  Tetraedre 3 = [s8,s4,s5,s6]
	  //  Tetraedre 4 = [s3,s4,s5,s8]
	  //  Tetraedre 5 = [s7,s3,s5,s8]
	  //  Tetraedre 6 = [s1,s4,s5,s3]
	  t1.s1 = s1; t1.s2 = s2; t1.s3 = s4; t1.s4 = s6; 
	  t1.t1 = (i<nx) ? l+dtx+5 : TETRAEDRE_VIDE;
	  t1.t2 =          l+1                     ;
	  t1.t3 = (j>0)  ? l-dty+3 : TETRAEDRE_VIDE;  
	  t1.t4 = (k>0)  ? l-dtz+2 : TETRAEDRE_VIDE;  
		
	  t2.s1 = s1; t2.s2 = s4; t2.s3 = s6; t2.s4 = s5; 
	  t2.t1 =          l+2                     ;
	  t2.t2 = (j>0)  ? l-dty+4 : TETRAEDRE_VIDE;  
	  t2.t3 =          l+5                     ;
	  t2.t4 =          l                       ;
		
	  t3.s1 = s8; t3.s2 = s4; t3.s3 = s5; t3.s4 = s6; 
	  t3.t1 =          l+1                     ;
	  t3.t2 = (k<nz) ? l+dtz   : TETRAEDRE_VIDE;  
	  t3.t3 = (i<nx) ? l+dtx+4 : TETRAEDRE_VIDE;
	  t3.t4 =          l+3                     ;

	  t4.s1 = s3; t4.s2 = s4; t4.s3 = s5; t4.s4 = s8; 
	  t4.t1 =          l+2                     ;
	  t4.t2 =          l+4                     ;
	  t4.t3 = (j<ny) ? l+dty   : TETRAEDRE_VIDE;
	  t4.t4 =          l+5                     ;
		
	  t5.s1 = s7; t5.s2 = s3; t5.s3 = s5; t5.s4 = s8; 
	  t5.t1 =          l+3                     ;
	  t5.t2 = (k<nz) ? l+dtz+5 : TETRAEDRE_VIDE;  
	  t5.t3 = (j<ny) ? l+dty+1 : TETRAEDRE_VIDE;
	  t5.t4 = (i>0 ) ? l-dtx+2 : TETRAEDRE_VIDE;

	  t6.s1 = s1; t6.s2 = s4; t6.s3 = s5; t6.s4 = s3; 
	  t6.t1 =          l+3                     ;
	  t6.t2 = (i>0 ) ? l-dtx   : TETRAEDRE_VIDE;
	  t6.t3 = (k>0 ) ? l-dtz+4 : TETRAEDRE_VIDE;  
	  t6.t4 =          l+1                     ;
		
	  tt[l  ] = t1;
	  tt[l+1] = t2;
	  tt[l+2] = t3;
	  tt[l+3] = t4;
	  tt[l+4] = t5;
	  tt[l+5] = t6;
	  l +=6;
	}
}

// triangulation d'une grille 2D
Tetraedrisation3D::Tetraedrisation3D(Grille3D G)
{
  tetraedrisation_grille(G);
}

// triangulation d'une grille 2D avec valeurs aux sommets 
// données par une fonction f
Tetraedrisation3D::Tetraedrisation3D(Grille3D G, Fct3Var f)
{
  tetraedrisation_grille(G);

  // evaluer la fonction f sur chaque sommet
  for (unsigned int i=0; i<nS; i++)
    {
      ts[i].v = f(ts[i].p.x,ts[i].p.y,ts[i].p.z);
    }
}

// triangulation d'une grille 2D avec valeurs aux sommets 
// données par un tableau de valeurs vf
// le sommet S_k a pour valeur vf[k] et correspond au point (i,j,k) de G
// i.e. k = i+DIM_X*(j+DIM_Y*k)
Tetraedrisation3D::Tetraedrisation3D(Grille3D G, double *vf)
{
  tetraedrisation_grille(G);

  // evaluer la fonction f sur chaque sommet
  for (unsigned int i=0; i<nS; i++)
    {
      ts[i].v = vf[i];
    }
}

// copie
Tetraedrisation3D::Tetraedrisation3D(const Tetraedrisation3D& T)
{
  nS = T.nS;
  nT = T.nT;
	
  // allocation memoire
  ts = new Sommet3D[nS];
  tt = new Tetraedre[nT];
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = T.ts[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = T.tt[i];
	
}
// destructeur
Tetraedrisation3D::~Tetraedrisation3D()
{
  if (ts != (Sommet3D*)(0L)) delete[] ts;
  if (tt != (Tetraedre*)(0L)) delete[] tt;
}

//// fonctions d'acces ////
Sommet3D Tetraedrisation3D::sommet(unsigned int i)
{
  return ts[i];
}
Tetraedre Tetraedrisation3D::tetraedre(unsigned int i)
{
  return tt[i];
}

//// fonction de modif. ////
Tetraedrisation3D& Tetraedrisation3D::operator=(const Tetraedrisation3D& T)
{
  if (ts != (Sommet3D*)(0L)) delete[] ts;
  if (tt != (Tetraedre*)(0L)) delete[] tt;
	
  nS = T.nS;
  nT = T.nT;
	
  // allocation memoire
  ts = new Sommet3D[nS];
  tt = new Tetraedre[nT];
	
  // copie
  for (unsigned int i=0; i<nS; i++)
    ts[i] = T.ts[i];
  for (unsigned int i=0; i<nT; i++)
    tt[i] = T.tt[i];
	
  return *this;
}

void Tetraedrisation3D::sommet(unsigned int i, Sommet3D s0)
{
  ts[i] = s0;
}

void Tetraedrisation3D::tetraedre(unsigned int i, Tetraedre t0)
{
  tt[i] = t0;
}


//////////////////////////////////////////////////////////////////////////////
// class Graphe
//////////////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream& o, const Noeud& n)
{
  o << "(m=" << n.m << " - la=" << n.la << ")";
  return o;
}

ostream& operator<<(ostream& o, const Arc& a)
{
  o << "(m=" << a.m << " - v=" << a.v << " - n1=" << a.n1 << " - n2=" << a.n2 << ")";
  return o;
}

ostream& operator<<(ostream& o, const v_Arc& va)
{
  o << "[ ";
  for (unsigned int i=0; i<va.size(); i++)
    {
      o << va[i] << endl;
    }
  o << "]" << endl;
  return o;
}

ostream& operator<<(ostream& o, const l_int& la)
{
  o << "{ ";
  for (l_int::const_iterator i=la.begin(); i!=la.end(); i++)
    {
      o << *i << " ";
    }
  o << "}" << endl;
  return o;
}

ostream& operator<<(ostream& o, const v_Noeud& vn)
{
  o << "[ ";
  for (unsigned int i=0; i<vn.size(); i++)
    {
      o << vn[i] << " ";
    }
  o << "]" << endl;
  return o;
}

ostream& operator<<(ostream& o, const Graphe& G)
{
  o << G.vn.size() << " noeuds : " << endl;
  o << G.vn << endl;
  o << G.va.size() << " arcs : " << endl;
  o << G.va << endl;
  return o;
}


//// constructeurs-destructeur ////

// graphe avec nn0 noeuds et aucun arc
Graphe::Graphe(unsigned int nn0)
{
  vn.resize(nn0);
  va.clear();
}

// copie
Graphe::Graphe(const Graphe& G)
{
  vn.clear();
  va.clear();
  vn = G.vn;
  va = G.va;
}

Graphe::~Graphe()
{
  vn.clear();
  va.clear();
}

//// fonctions d'acces ////

// renvoie (une référence sur) le noeud d'indice i
Noeud& Graphe::noeud(unsigned int i)
{
  return vn[i];
}

// renvoie (une référence sur) l'arc d'indice i
Arc& Graphe::arc(unsigned int i)
{
  return va[i];
}

// ecrit à l'ecrant le graphe G
void put(Graphe& G);

//// fonctions de modif. ////
unsigned int Graphe::ajouter_arc(unsigned int n1, unsigned int n2, double v)
{
  if (n1>n2) 
    {
      unsigned int r = n1; n1 = n2; n2 = r;
    }
  Arc a = {n1,n2,v,0};
  int num_arc = va.size();
  va.push_back(a);
  vn[n1].la.push_back(num_arc);
  vn[n2].la.push_back(num_arc);
  return num_arc;
}

// fonction de comparaison < entre 2 arcs
bool operator<(Arc a1, Arc a2)
{
  return a1.v < a2.v;
}

// calcul d'un arbre couvrant minimal d'un graphe G
Graphe Graphe::arbre_couvrant_minimal()
{
  // tri des arcs de G
  v_Arc va_aux = va;
  sort(va_aux.begin(), va_aux.end());
	
  // creation de l'arbre couvrant minimal
  unsigned int nN = nb_noeuds();
  Graphe ACM(nN);
  for (unsigned int i=0; i<nN; i++)
    ACM.noeud(i).m = 0;

  unsigned int nb_arcs_ACM=0;
  unsigned int iA;
  unsigned int num_sous_arbre=0;
  unsigned int nA = nb_arcs();
  iA = 0;
  while (nb_arcs_ACM<nN-1 && iA<nA)
    {
      // noeuds de l'arete iA
      unsigned int n1 = va_aux[iA].n1;
      unsigned int n2 = va_aux[iA].n2;
  
      // numeros associes aux sommets
      // pour un sommet, le numero est :
      //   >0 si le sommet appartient au sous-arbre de meme numero
      //   =0 si le sommet n'appartient à aucun sous-arbre
      unsigned int num1 = ACM.noeud(n1).m;
      unsigned int num2 = ACM.noeud(n2).m;
  
      if (num1==0 && num2==0)
	{
	  // creer un nouveau sous-arbre réduit à l'arete [n1,n2]
	  num_sous_arbre++;
	  ACM.noeud(n1).m = num_sous_arbre;
	  ACM.noeud(n2).m = num_sous_arbre;
			
	  // ajouter l'arc nb_arc = [n1,n2]
	  //		    ACM.noeud(n1).la.push_back(nb_arcs_ACM);
	  //		    ACM.noeud(n2).la.push_back(nb_arcs_ACM);
	  ACM.ajouter_arc(n1,n2);
	  nb_arcs_ACM++;
	}
      else if (num1==0)
	{
	  // ajouter le sommet n1 au sous-arbre de n2
	  ACM.noeud(n1).m = num2;
			
	  // ajouter l'arc nb_arc = [n1,n2]
	  //		    ACM.noeud(n1).la.push_back(nb_arcs_ACM);
	  //		    ACM.noeud(n2).la.push_back(nb_arcs_ACM);
	  ACM.ajouter_arc(n1,n2);
	  nb_arcs_ACM++;
	}
      else if (num2==0)
	{
	  // ajouter le sommet n2 au sous-arbre de n1
	  ACM.noeud(n2).m = num1;
			
	  // ajouter l'arc nb_arc = [n1,n2]
	  //		    ACM.noeud(n1).la.push_back(nb_arcs_ACM);
	  //		    ACM.noeud(n2).la.push_back(nb_arcs_ACM);
	  ACM.ajouter_arc(n1,n2);
	  nb_arcs_ACM++;
	}
      else if (num1 != num2)
	{
	  // fusionner les arbres de n1 et n2 : changer le numero 
	  // de l'arbre contenant n2
	  l_int L;
	  L.push_back(n2);
	  while (!L.empty())
	    {
	      unsigned int s = L.back();
	      L.pop_back();
	      Noeud& noeud_s = ACM.noeud(s);
	      noeud_s.m = num1;
	      for (l_int::iterator k=noeud_s.la.begin(); 
		   k!=noeud_s.la.end(); k++)
		{
		  Arc a = ACM.arc(*k);
		  unsigned int j = a.n1 == s ? a.n2 : a.n1;
		  if (ACM.noeud(j).m == num2)
		    L.push_back(j);
		}
	    }
			
	  // ajouter l'arc nb_arc = [n1,n2]
	  //		    ACM.noeud(n1).la.push_back(nb_arcs_ACM);
	  //		    ACM.noeud(n2).la.push_back(nb_arcs_ACM);
	  ACM.ajouter_arc(n1,n2);
	  nb_arcs_ACM++;
	}
      // 		printf("iA = %4d, nb_arcs_ACM = %d, n = %d\n", 
      // 		  iA, nb_arcs_ACM, n);
      iA++;
    }
	
  // 	// ecrit l'ACM
  // 	for (int i=0; i<n; i++)
  // 	{
  // 		printf("sommet %d (%d) : [", i, noeuds[i].m);
  // 		for (int k=0; k<noeuds[i].la.size(); k++)
  // 		{
  // 			int j = noeuds[i].voisins[k];
  // 			printf(" %d",j);
  // 		}
  // 		printf(" ]\n");
  // 	}
	
  // retourner l'arbre CM
  return ACM;
}
