///////////////////////////////////////////////////////////////////////////////
// types et opérations géométriques en 3D
///////////////////////////////////////////////////////////////////////////////

#include<math.h>
#include<stdio.h>
#include<iostream>
#include"geom.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// routines de calcul d'elements propres
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/* racine cubique de x */
double sqrt3(double x)
{
  return (x>=0 ? pow(x,1.0/3.0) : -pow(-x,1.0/3.0));
}

/* racine cubique de z = a+i*b */
void sqrt3complexe(double a, double b, double *c, double *d)
{
  double rho, theta;
  rho = sqrt(a*a+b*b);
  theta = atan2(b,a);
  *c = sqrt3(rho)*cos(theta/3.0);
  *d = sqrt3(rho)*sin(theta/3.0);
}

/* calcul des racines d'un polynome de degré 3 à coeff. réels */
/* P(x) = x^3+a*x^2+b*x+c */
/* Entree : a,b,c = les coef réels de P */
/* Sortie : p1, p2, p3 = pointeurs vers 3 reels v1, v2, v3 */
/* La routine renvoie une valeur entière res 0 et 1 */
/*   res = 0 : il y a une racine réelle x1= v1 et deux racines complexes */
/*     conjuguées x2 = v2 + i*v3 et x3 = v2-i*v3 */
/*   res = 1 : les trois racines sont réelles vi=xi */
/* reference : Methode_de_Cardan */
int racines_polynome_degre3(double a, double b, double c,
			    double *p1, double *p2, double *p3)
{
  double p, q;
  double delta;
  double x1, x2, x3;
  int res;
  double jRe = -0.5;
  double jIm = sqrt(3)/2.0;
	
  /* p, q coeff. de l'equation P(x) = X^3 + p*X + q = 0  avec x = X-a/3 */
  p = b-a*a/3.0;
  q = c+(2*a*a-9*b)*a/27.0;
	
	
  delta = q*q+p*p*p*4.0/27.0;
	
  if (delta > 0)
    {
      double u, v;
      u = sqrt3(0.5*(-q+sqrt(delta)));
      v = sqrt3(0.5*(-q-sqrt(delta)));
      x1 = u+v - a/3;
      x2 = jRe*(u+v) - a/3;
      x3 = jIm*(u-v);
      res=0;
    }
  else if (delta<0)
    {
      double uRe, uIm;
      sqrt3complexe(-q/2.0, sqrt(-delta)/2.0, &uRe, &uIm);
      x1 = 2*uRe - a/3;
      x2 = 2*(uRe*jRe-uIm*jIm) - a/3;
      x3 = 2*(uRe*jRe+uIm*jIm) - a/3;
      res=1;
    }
  else // cas delta=0
    {
      x1 = 2*sqrt3(-q/2.0) - a/3;
      x2 = x3 = -sqrt3(-q/2.0) - a/3;
      res=1;
    }
  *p1 = x1;
  *p2 = x2;
  *p3 = x3;
  return res;
}

// calcul de la plus petite valeur propre et du vecteur propre associé
// d'une matrice A symétrique de dimension 3
//     ( A11 A12 A13)
// A = ( A12 A22 A23)
//     ( A13 A23 A33)
// Entrée : A11,A12,A13,A22,A23,A33 = les six valeurs définissant 
//             la matrice symetrique
// Sortie : lmin = la plus petite valeur propre de A
//          vmin = le vecteur propre unitaire associé à lmin
void elements_propres_min_mat_sym(double A11, double A12, double A13, 
				  double A22, double A23, double A33,
				  double &lmin, Point3D &vmin)
{
  /* coefficients du polynome caractéristique */
  /* f(l) = l^3+a*l^2+b*l+c */
  /*      = (l-A11)*(l-A22)*(l-A33)-2*A12*A13*A23 */
  /*      + (A11-l)*A23^2 + (A22-l)*A13^2  + (A33-l)*A12^2 */
  double a,b,c;
  double l1,l2,l3,l;
  double v1,v2,v3,nv;
  double A21=A12;
  double A31=A13;
  double A32=A23;
  double delta1,delta2,delta3;
  double eps;
  double Amax;
  int res;
	
  /* normalisation pour que Max|Aij| = 1 */
  Amax = 0.0;
  if (Amax < ABS(A11)) Amax = ABS(A11);
  if (Amax < ABS(A12)) Amax = ABS(A12);
  if (Amax < ABS(A13)) Amax = ABS(A13);
  if (Amax < ABS(A22)) Amax = ABS(A22);
  if (Amax < ABS(A23)) Amax = ABS(A23);
  if (Amax < ABS(A33)) Amax = ABS(A33);
  if (Amax == 0.0)
    {
      /* A matrice nulle : lmin = 0 et v quelconque */
      lmin =0.0;
      vmin = Point3D(1.0,0.0,0.0);
      return; 
    }
	
  /* diviser Aij par Amax */
  A11 /= Amax; A12 /= Amax; A13 /= Amax; 
  A21 /= Amax; A22 /= Amax; A23 /= Amax; 
  A31 /= Amax; A32 /= Amax; A33 /= Amax; 
	
  eps=1.0e-10;
	
  a = -A11-A22-A33;
  b = A11*A22+A11*A33+A22*A33-A12*A12-A13*A13-A23*A23;
  c = A11*A23*A23+A22*A13*A13+A33*A12*A12-2*A12*A13*A23-A11*A22*A33;
	
  /* calcul des racines de f */
  res = racines_polynome_degre3(a, b, c, &l1, &l2, &l3);
  if (res==0)
    /* erreur numérique (l3 très petit) -> l3 nul */
    /* -> une racine double l3 = l2 */
    l3 = l2;
	
  /* l = min(li) */
  l = MIN(MIN(l1,l2),l3);
	
  /* matrice A = A-l*I de rang <= 2 */
  A11 -= l; 
  A22 -= l; 
  A33 -= l; 
	
  /* calcul d'un vecteur propre associé à l : */
  /* résoudre le système Av = 0 avec v non nul */
  delta1 = A22*A33-A32*A23;
  delta2 = A33*A11-A13*A31;
  delta3 = A11*A22-A21*A12;
  if (ABS(delta1) >= ABS(delta2) && 
      ABS(delta1) >= ABS(delta3) && 
      ABS(delta1) >= eps)
    {
      /* calculer un vecteur propre non nul avec les lignes 2 et 3 de A */
      /*  en fixant la 1ère coordonnée égale à 1 */
      v2 = (A23*A31-A21*A33)/delta1;
      v3 = (A21*A23-A22*A31)/delta1;
      v1 = 1;			
    }
  else if (ABS(delta2) >= ABS(delta3) && 
	   ABS(delta2) >= ABS(delta1) && 
	   ABS(delta2) >= eps)
    {
      /* calculer un vecteur propre non nul avec les lignes 1 et 3 de A */
      /*  en fixant la 2ème coordonnée égale à 1 */
      v3 = (A31*A12-A32*A11)/delta2;
      v1 = (A32*A31-A33*A12)/delta2;
      v2 = 1;			
    }
  else if (ABS(delta3) >= eps)
    {
      /* calculer un vecteur propre non nul avec les lignes 1 et 2 de A */
      /*  en fixant la 3ème coordonnée égale à 1 */
      v1 = (A12*A23-A13*A22)/delta3;
      v2 = (A13*A12-A11*A23)/delta3;
      v3 = 1;			
    }
	
  else
    /* les trois delta_i nuls : A est de rang <= 1 */
    {
      if (ABS(A11) >= ABS(A22) && 
	  ABS(A11) >= ABS(A33) && 
	  ABS(A11) >= eps)
	{
	  /* calculer un vecteur propre non nul avec la ligne 1 de A */
	  /*  en fixant la 3ème coordonnée égale à 0 */
	  v1 = -A12;
	  v2 =  A11;
	  v3 = 0;
	}
      else if (ABS(A22) >= ABS(A33) && 
	       ABS(A22) >= ABS(A11) && 
	       ABS(A22) >= eps)
	{
	  /* calculer un vecteur propre non nul avec la ligne 2 de A */
	  /*  en fixant la 1ère coordonnée égale à 0 */
	  v2 = -A23;
	  v3 =  A22;
	  v1 = 0;
	}
      else if (ABS(A33) >= eps)
	{
	  /* calculer un vecteur propre non nul avec la ligne 3 de A */
	  /*  en fixant la 2ème coordonnée égale à 0 */
	  v3 = -A31;
	  v1 =  A33;
	  v2 = 0;		
	}
      else /* la matrice A est nulle */
	{
	  /* A de rang 0 : v quelconque */
	  v1 = 1; v2 = v3 = 0;
	}
    }
		
  /* normalisation du vecteur */
  nv = sqrt(v1*v1+v2*v2+v3*v3);
  vmin = Point3D(v1,v2,v3)/nv;
  lmin = l*Amax;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// les définitions des différentes méthodes de la classe Point3D
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

double Point3D::norme()
{
  return sqrt(x*x+y*y+z*z);
}

void Point3D::normalise()
{
  double n = sqrt(x*x+y*y+z*z);
  if (n>0.0)	{x /= n; y /= n; z /= n; }
}

void Point3D::print(const char* format)
{
  printf("(");	
  printf(format, x);    
  printf(",");
  printf(format, y);    
  printf(",");
  printf(format, z);    
  printf(")");	
}

Point3D& Point3D::operator+=(Point3D P2)
{
  x += P2.x;
  y += P2.y;
  z += P2.z;
  return *this;
}

Point3D& Point3D::operator-=(Point3D P2)
{
  x -= P2.x;
  y -= P2.y;
  z -= P2.z;
  return *this;
}

Point3D& Point3D::operator*=(double r2)
{
  x *= r2;
  y *= r2;
  z *= r2;
  return *this;
}

Point3D& Point3D::operator/=(double r2)
{
  x /= r2;
  y /= r2;
  z /= r2;
  return *this;
}


///////////////////////////////////////////////////////////////////////////////
// autres routines
///////////////////////////////////////////////////////////////////////////////
Point3D operator+(Point3D P1, Point3D P2)
{
  return Point3D  (
		   P1.x+P2.x ,
		   P1.y+P2.y ,
		   P1.z+P2.z   );
}

Point3D operator-(Point3D P1, Point3D P2)
{
  return Point3D  (
		   P1.x-P2.x ,
		   P1.y-P2.y ,
		   P1.z-P2.z   );
}

Point3D operator*(double r1, Point3D P2)
{
  return Point3D  (
		   r1*P2.x ,
		   r1*P2.y ,
		   r1*P2.z   );
}

Point3D operator*(Point3D P1, double r2)
{
  return Point3D  (
		   P1.x*r2 ,
		   P1.y*r2 ,
		   P1.z*r2   );
}

Point3D operator/(Point3D P1, double r2)
{
  return Point3D  (
		   P1.x/r2 ,
		   P1.y/r2 ,
		   P1.z/r2   );
}

Point3D operator+(Point3D P1)
{
  return Point3D  (
		   P1.x ,
		   P1.y ,
		   P1.z   );
}

Point3D operator-(Point3D P1)
{
  return Point3D  (
		   -P1.x ,
		   -P1.y ,
		   -P1.z	);
}

bool operator==(Point3D P1, Point3D P2)
{
  return
    (P1.x == P2.x) &&
    (P1.y == P2.y) &&
    (P1.z == P2.z) ;
}

bool operator!=(Point3D P1, Point3D P2)
{
  return !(P1==P2);
}


ostream & operator<<(ostream & o, Point3D P)
{
  o << "(" << P.x << "," << P.y << "," << P.z << ")";
  return o;
}

void print(Point3D P, const char* format)
{
  P.print(format);
}

double norme(Point3D P1)
{
  return P1.norme();
}
double distance_(Point3D P1, Point3D P2)
{
  return norme(P1-P2);
}
double produit_scalaire(Point3D P1, Point3D P2)
{
  return P1.x*P2.x + P1.y*P2.y + P1.z*P2.z ;	  
}

Point3D normalise(Point3D P1)
{
  return P1/norme(P1);
}

Point3D produit_vectoriel(Point3D P1, Point3D P2)
{
  return Point3D(
		 P1.y*P2.z-P2.y*P1.z,
		 P1.z*P2.x-P2.z*P1.x,
		 P1.x*P2.y-P2.x*P1.y );
}

///////////////////////////////////////////////////////////////////////////////
// aire du parallelogramme engendre par les points a et b
double aire(Point3D a, Point3D b)
{
  return norme(produit_vectoriel(a,b));
}

///////////////////////////////////////////////////////////////////////////////
// produit mixte des 3 points a,b,c
double volume(Point3D a, Point3D b, Point3D c)
{
  return produit_scalaire(a,produit_vectoriel(b,c));
}

///////////////////////////////////////////////////////////////////////////////
// calcul de la distance entre un point P et un segment [S1,S2]              //
// Entrée : P = un point 3D                                                  //
//          S1,S2 = points-extrémité 3D du segment                           //
///////////////////////////////////////////////////////////////////////////////
double distance_point_segment(Point3D P, Point3D S1, Point3D S2)
{
  double t;
  Point3D v1,v2,v12;
  double p2;
	
  v1  = P-S1;
  v2  = P-S2;
  v12 = S2-S1;
  p2 = produit_scalaire(v12,v12);
  if (p2>0.0)
    {
      // S1 différent de S2
      t = produit_scalaire(v1 ,v12)/
	produit_scalaire(v12,v12);
      if (t<0)
	return norme(v1);
      if (t>1)
	return norme(v2);
      return distance_( v1, t*v12 );
    }
  else
    // S1 égal à S2
    return norme(v1);	
}

///////////////////////////////////////////////////////////////////////////////
// calcul de la distance entre un point P et un triangle [S1,S2,S3]          //
// Entrée : P = un point 3D                                                  //
//          S1,S2,S3 = points-extrémité 3D du triangle                       //
///////////////////////////////////////////////////////////////////////////////
double distance_point_triangle(Point3D P, Point3D S1, Point3D S2, Point3D S3)
{
  double u,v,w,a11,a12,a22,b1,b2,delta;
  Point3D v1,v12,v13;
	
  v1  = P-S1;
  v12 = S2-S1;
  v13 = S3-S1;
	
  a11 = produit_scalaire(v12,v12);
  a12 = produit_scalaire(v12,v13);
  a22 = produit_scalaire(v13,v13);
  b1  = produit_scalaire(v1,v12);
  b2  = produit_scalaire(v1,v13);
  delta = a11*a22-a12*a12;
	
  u = (b1*a22-b2*a12)/delta;
  v = (b2*a11-b1*a12)/delta;
  w = 1.0-u-v;
  //cout << "u = " << u << ", v = " << v << endl;
	
  if (u<0)
    return(distance_point_segment(P,S1,S3));
  if (v<0)
    return(distance_point_segment(P,S1,S2));
  if (w<0)
    return(distance_point_segment(P,S2,S3));
	
  return distance_(v1, u*v12+v*v13);  
}

// calcule la boite englobante des points du vecteur P
// Entree : P = vecteur de Point3D
// Sortie : Pmin, Pmax = bornes de la boite englobante
void boite_englobante(v_Point3D& P, Point3D &Pmin, Point3D &Pmax)
{
  Pmin = Pmax = P[0];
  for (unsigned int i=1; i<P.size(); i++)
    {
      if (Pmin.x > P[i].x) Pmin.x = P[i].x;
      if (Pmin.y > P[i].y) Pmin.y = P[i].y;
      if (Pmin.z > P[i].z) Pmin.z = P[i].z;
      if (Pmax.x < P[i].x) Pmax.x = P[i].x;
      if (Pmax.y < P[i].y) Pmax.y = P[i].y;
      if (Pmax.z < P[i].z) Pmax.z = P[i].z;
    }
}

// effectue un changement d'echelle e sur le vecteur de points P
void echelle(v_Point3D& P, double e)
{
  for (unsigned i=0; i<P.size(); i++)
    P[i] *= e;
}

///////////////////////////////////////////////////////////////////////////////
// calcule les ecarts dmin et dmax pour un ensemble de points P
// dmin = min( min(|P(i)-P(j)|)
//         i  j!=i
// dmax = max( min(|P(i)-P(j)|)
//         i  j!=i
void ecart_min_max(v_Point3D P,double &dmin, double &dmax)
{  
  dmin = norme(P[0]-P[1]);
  dmax = dmin;
  int n = P.size();
  for (int i=0; i<n; i++)
    {
      double dmin_i = norme(P[i]-P[(i+1)%n]);
      for (int j=0; j<n; j++)
	{
	  if (j!=i)
	    {
	      double d = norme(P[i]-P[j]);
	      if (dmin_i>d) dmin_i = d;				
	    }
	}
      if (dmin > dmin_i) dmin = dmin_i;
      if (dmax < dmin_i) dmax = dmin_i;
    }
}

//////////////////////////////////////////////////////////////////////////////
// étant donné n vecteur, détermine deux vecteurs unitaires u et v
// tels que <u.n> = <(v.n> = <u.v> = 0
void calcul_repere(Point3D n, Point3D &u, Point3D &v)
{
  double nx = ABS(n.x);
  double ny = ABS(n.y);
  double nz = ABS(n.z);
	
  if (nx==0.0 && ny==0.0 && nz==0.0)
    {
      u = Point3D(1.0,0.0,0.0);
      v = Point3D(0.0,1.0,0.0);
    }
  else
    {
      if (nx <= ny && nx <= nz)
	{
	  u = Point3D(0.0,n.z,-n.y);
	  normalise(u);
	}
      else if (ny <= nz && ny <= nx)
	{
	  u = Point3D(-n.z,0.0,n.x);
	  normalise(u);
	}
      else
	{
	  u = Point3D(n.y,-n.x,0.0);
	  normalise(u);
	}
      v = produit_vectoriel(n,u);
      normalise(v);
    }
}


void calcul_repere_vecteurs_propres(double A11,double A12,double A13, double A22,double A23,double A33,Point3D &u,Point3D &v,Point3D &n) {
  // calcule le repère associé à la matrice carrée symétrique suivante : 
  //     ( A11 A12 A13)
  // A = ( A12 A22 A23)
  //     ( A13 A23 A33)  
  double l;
  elements_propres_min_mat_sym(A11, A12, A13, A22, A23, A33, l, n);
  calcul_repere(n, u, v);
}
