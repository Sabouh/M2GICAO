#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/manipulatedFrame.h>
#include <QGLViewer/qglviewer.h>
#include "eventWidget.h"
#include "pointsToSurface.h"

class Viewer : public QGLViewer {
 public:
  Viewer(const QString &filename);
  ~Viewer();
  
  void drawInputPoints();
  void drawNonOrientedNormals();
  void drawMinimalSpanningTree();
  void drawOrientedNormals(bool invertNormals);
  void drawSurface(bool invertNormals);
  void drawBox();

 protected :
  virtual void draw();
  virtual void init();

 private:
  QString                       _filename;
  PointsToSurface               _pointsToSurface;
  qglviewer::ManipulatedFrame  *_light;
  EventWidget                  *_widget;

  void drawMinimalSpanningTreeRec(Graphe &g,Noeud &n,unsigned int v,v_Point3D &pts,std::vector<bool> &visited);

  std::vector<Point3D> _f;
  std::vector<Point3D> _c;
};


#endif // VIEWER_H
