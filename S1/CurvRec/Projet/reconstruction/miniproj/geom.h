///////////////////////////////////////////////////////////////////////////////
// types et opérations géométriques en 2D et 3D
///////////////////////////////////////////////////////////////////////////////

#ifndef _GEOM_H_
#define _GEOM_H_

#include<iostream>
#include<vector>

using namespace std;

#ifndef ABS
#define ABS(x) ((x)>0 ? (x) : (-x))
#endif

#ifndef MIN
#define MIN(x,y) ((x)>(y) ? (y) : (x))
#endif

#ifndef MAX
#define MAX(x,y) ((x)<(y) ? (y) : (x))
#endif

////////////////////////////////////////////////////////////////////////////// 
class Point3D { 
 public:
  // coordonnées
  double x,y,z;

  // constructeur
 Point3D(double x0=0.0, double y0=0.0, double z0=0.0):
  x(x0), y(y0), z(z0) {}
		
  // opérateurs de modification
  Point3D& operator+=(Point3D P2);	
  Point3D& operator-=(Point3D P2);	
  Point3D& operator*=(double r2);	
  Point3D& operator/=(double r2);	

  // autres méthodes simples
  double norme(); // le module
  void normalise(); // normalise le point
  void print(const char* format="%f"); // ecriture
};  
typedef vector<Point3D> v_Point3D;

// surcharge d'opérateurs ou routines existants
Point3D operator+(Point3D P1, Point3D P2);
Point3D operator-(Point3D P1, Point3D P2);
Point3D operator*(double r1, Point3D P2);
Point3D operator*(Point3D P1, double r2);
Point3D operator/(Point3D P1, double r2);
Point3D operator+(Point3D P1);
Point3D operator-(Point3D P1);
bool operator==(Point3D P1, Point3D P2);
bool operator!=(Point3D P1, Point3D P2);
ostream& operator<<(ostream& o, Point3D p);

// autres routines

// norme de P1
double norme(Point3D P1);

// distance entre P1 et P2
double distance_(Point3D P1, Point3D P2);

// produit scalaire <P1.P2>
double produit_scalaire(Point3D P1, Point3D P2);

// calcul de P1/norme(P1)
Point3D normalise(Point3D P1);

// produit vectoriel P1 par P2
Point3D produit_vectoriel(Point3D P1, Point3D P2);

// aire signee entre a et b = norme(a)*norme(a)*sin(angle(a,b))
double aire(Point3D a, Point3D b);

// volume signe entre a, b, c = produit_scalaire(a,produit_vectoriel(b,c))
double volume(Point3D a, Point3D b, Point3D c);

// distance entre le point P et le segment [S1 S2]
double distance_point_segment(Point3D P, Point3D S1, Point3D S2);

// distance entre le point P et le triangle [S1 S2 S3]
double distance_point_triangle(Point3D P, Point3D S1, Point3D S2, Point3D S3);

// boite englobante des points du vecteur P
// Pmin.x = min(P[i].x) Pmax.x = max(P[i].x)
// Pmin.y = min(P[i].y) Pmax.y = max(P[i].y)
// Pmin.z = min(P[i].z) Pmax.z = max(P[i].z)
void boite_englobante(v_Point3D& P, Point3D &Pmin, Point3D &Pmax);

// effectue un changement d'echelle de rapport e sur les points du vecteur R
void echelle(v_Point3D& P, double e);

// calcul les distances min et max entre points voisins du vecteur P
void ecart_min_max(v_Point3D P,double &dmin, double &dmax);

// calcule le repère associé à la matrice carrée symétrique
  //     ( A11 A12 A13)
  // A = ( A12 A22 A23)
  //     ( A13 A23 A33)  
void calcul_repere_vecteurs_propres(double A11,
				    double A12,
				    double A13, 
				    double A22,
				    double A23,
				    double A33,
				    Point3D &u,
				    Point3D &v,
				    Point3D &n);
#endif
