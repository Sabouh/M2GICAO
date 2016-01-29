#ifndef POINTS_TO_SURFACE_H
#define POINTS_TO_SURFACE_H

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"data_struct_algo.h"
#include"iso_value.h"
#include <QString>

class PointsToSurface {
 public:
  PointsToSurface(const QString &filename);
  ~PointsToSurface();

  // ********* les fonctions à remplir ************
  void   computeNonOrientedNormals();
  void   computeOrientedNormals();
  void   computeMinimalSpanningTree();
  void   computeNormalsFromImplicitFunc();
  void   computeMesh();
  double computeImplicitFunc(double x,double y,double z);
  // **********************************************
    //Fonctions ajoutée :
  Point3D center_of_gravity(v_Point3D pts);

  // fonction qui appelle toutes les fonctions ci-dessus (rien à faire)
  void computeSurface();

  // fonctions d'accession (rien à faire)
  v_Point3D    inputPoints()         const {return _points;}
  v_Point3D    nonOrientedNormals()  const {return _noNormals;}
  v_Point3D    orientedNormals()     const {return _oNormals;}
  Graphe       minimalSpanningTree() const {return _acm;}
  v_Triangle3D surfacePoints()       const {return _surfacep;}
  v_Triangle3D surfaceNormals()      const {return _surfacen;}

  // bounding box
  Point3D   boundingMin() const {return _boundingBox[0];}
  Point3D   boundingMax() const {return _boundingBox[1];}

 private:
  // lecture du fichier de points (rien a faire)
  bool readInputFile(const QString &filename);

  // récupère les k-points les plus proches 
  v_Point3D kneighborhoodPoints(const Point3D &p,const v_Point3D &pts,unsigned int k) const;
  

  v_Point3D    _points;      // original input points (déjà rempli)

  // ********* les données à remplir ************
  v_Point3D    _noNormals;   // non-oriented normals
  v_Point3D    _oNormals;    // oriented normals
  Graphe       _acm;         // minimal spanning tree
  v_Triangle3D _surfacep;    // reconstructed surface points
  v_Triangle3D _surfacen;    // reconstructed surface normals
  // ********************************************

  
  // bounding box (rempli à la lecture du fichier de points)
  Point3D   _boundingBox[2]; 

  // mean distance between pairs of closest points (rempli à la lecture du fichier)
  double    _meanDist;
};


#endif // POINTS_TO_SURFACE_H
