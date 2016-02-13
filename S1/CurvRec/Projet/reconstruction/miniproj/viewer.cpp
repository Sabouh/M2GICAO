#include "viewer.h"
#include "data_struct_algo.h"
#include "geom.h"

using namespace std;

Viewer::Viewer(const QString &filename)
  : QGLViewer(),
    _filename(filename),
    _pointsToSurface(filename),
    _widget(new EventWidget(_pointsToSurface,this)) {
  _widget->show();
}

Viewer::~Viewer() {
  
}

void Viewer::drawBox() {

  v_Triangle3D t = _pointsToSurface.surfacePoints();
  
  if(t.empty()) return;

  glPointSize(4.0f);
  glBegin(GL_POINTS);
  
  for(unsigned int i=0;i<_f.size();++i) {
    glColor3f(_c[i].x,_c[i].y,_c[i].z);
    glVertex3f(_f[i].x,_f[i].y,_f[i].z);
  }

  glEnd();
  glPointSize(1.0f);
}

void Viewer::drawSurface(bool invertNormals) {
  float pos[4] = {0.0,0.0,0.0,1.0};
  //_light->getPosition(pos[0],pos[1],pos[2]);
  glLightfv(GL_LIGHT1,GL_POSITION,pos);
 
  v_Point3D    p = _pointsToSurface.inputPoints();
  v_Triangle3D t = _pointsToSurface.surfacePoints();
  v_Triangle3D n = _pointsToSurface.surfaceNormals();

  glEnable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  glColor3f(0.8f,0.8f,0.8f);
  for(unsigned int i=0;i<t.size();++i) {
    
    if(invertNormals) {
      n[i].S0 = n[i].S0*-1.0;
      n[i].S1 = n[i].S1*-1.0;
      n[i].S2 = n[i].S2*-1.0;
    }

    glNormal3f(n[i].S0.x,n[i].S0.y,n[i].S0.z);
    glVertex3f(t[i].S0.x,t[i].S0.y,t[i].S0.z);
    
    glNormal3f(n[i].S1.x,n[i].S1.y,n[i].S1.z);
    glVertex3f(t[i].S1.x,t[i].S1.y,t[i].S1.z);

    glNormal3f(n[i].S2.x,n[i].S2.y,n[i].S2.z);
    glVertex3f(t[i].S2.x,t[i].S2.y,t[i].S2.z);

  }

  glEnd();

  glDisable(GL_LIGHTING);

  if (_light->grabsMouse())
    drawLight(GL_LIGHT1,1.2f);
  else
    drawLight(GL_LIGHT1);
}

void Viewer::drawMinimalSpanningTreeRec(Graphe &g,Noeud &n,unsigned int v,v_Point3D &pts,std::vector<bool> &visited) { 
  unsigned int orig;
  unsigned int dest;

  visited[v] = true;
  
  for(l_int::iterator i=n.la.begin();i!=n.la.end();++i) {
    Arc a = g.arc(*i);
    
    if(a.n1==v) {
      orig = a.n1;
      dest = a.n2;
    } else {
      orig = a.n2;
      dest = a.n1;
    }
    
    if(!visited[dest]) {
      glVertex3f(pts[orig].x,pts[orig].y,pts[orig].z);
      glVertex3f(pts[dest].x,pts[dest].y,pts[dest].z);
      
      drawMinimalSpanningTreeRec(g,g.noeud(dest),dest,pts,visited);
    }
  }
}

void Viewer::drawMinimalSpanningTree() {
  v_Point3D pts = _pointsToSurface.inputPoints();
  Graphe g = _pointsToSurface.minimalSpanningTree();

  if(g.nb_arcs()==0) return;

  std::vector<bool> visited(pts.size(),false);

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINES);
  glColor3f(1.0f,1.0f,0.0f);
  Noeud node = g.noeud(0);
  drawMinimalSpanningTreeRec(g,node,0,pts,visited);

  glEnd();
}

void Viewer::drawOrientedNormals(bool invertNormals) {
  v_Point3D pts = _pointsToSurface.inputPoints();
  v_Point3D  on = _pointsToSurface.orientedNormals();

  if(on.size()!=pts.size()) {
    // normals are not computed...
    return;
  }

  float size = distance_(_pointsToSurface.boundingMin(),_pointsToSurface.boundingMax())/50.0f;

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINES);
  glColor3f(0.0f,0.0f,1.0f);
  for(unsigned int i=0;i<pts.size();++i) {
    Point3D p = invertNormals ? pts[i]+on[i]*size : pts[i]-on[i]*size;
    glVertex3f(pts[i].x,pts[i].y,pts[i].z);
    glVertex3f(p.x,p.y,p.z);
  }

  glEnd();
}

void Viewer::drawNonOrientedNormals() {
  v_Point3D pts = _pointsToSurface.inputPoints();
  v_Point3D non = _pointsToSurface.nonOrientedNormals();

  if(non.size()!=pts.size()) {
    // normals are not computed...
    return;
  }

  float size = distance_(_pointsToSurface.boundingMin(),_pointsToSurface.boundingMax())/50.0f;

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINES);
  glColor3f(0.0f,1.0f,0.0f);
  for(unsigned int i=0;i<pts.size();++i) {
    Point3D p = pts[i]-non[i]*size;
    glVertex3f(pts[i].x,pts[i].y,pts[i].z);
    glVertex3f(p.x,p.y,p.z);
  }

  glEnd();
}

void Viewer::drawInputPoints() {
  v_Point3D pts = _pointsToSurface.inputPoints();
  
  glPointSize(2.0f);
  glEnable(GL_POINT_SMOOTH);
  
  glBegin(GL_POINTS);
  glColor3f(1.0f,0.0f,0.0f);
  for(unsigned int i=0;i<pts.size();++i) {
    glVertex3f(pts[i].x,pts[i].y,pts[i].z);
  }

  glEnd();
}

// Draws a spiral
void Viewer::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(_widget->showInputPoints()) 
    drawInputPoints();

  if(_widget->showNonOrientedNormals())
    drawNonOrientedNormals();

  if(_widget->showMinimalSpanningTree()) 
    drawMinimalSpanningTree();

  if(_widget->showOrientedNormals()) {
    if(_widget->invertNormals())
      drawOrientedNormals(true);
    else
      drawOrientedNormals(false);
  }
  
  switch(_widget->mode()) {
  case 0:
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_SMOOTH);
    break;
  case 1:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    break;
  case 2:
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glShadeModel(GL_FLAT);
    break;
  }

  if(_widget->showSurface()) {
    if(_widget->invertNormals())
      drawSurface(true);
    else 
      drawSurface(false);
  }

  if(_widget->box())
    drawBox();
}

void Viewer::init()
{
  // Restore previous viewer state.
  restoreStateFromFile();
  
  glDisable(GL_LIGHTING);

  Point3D bdmin  = _pointsToSurface.boundingMin();
  Point3D bdmax  = _pointsToSurface.boundingMax();
  Point3D center = (bdmax+bdmin)/2.0;
  float   radius = distance_(bdmax,bdmin)*3.0f;
  
  camera()->setSceneCenter(qglviewer::Vec(center.x,center.y,center.z));
  camera()->setSceneRadius(radius);

  _pointsToSurface.computeSurface();
  
  _light = new qglviewer::ManipulatedFrame();
  _light->setPosition(_pointsToSurface.boundingMax().x*5.0,
		      _pointsToSurface.boundingMax().y*5.0,
		      _pointsToSurface.boundingMax().z*5.0);
  //_light->setOrientation(qglviewer::Quaternion(qglviewer::Vec(0,0,1),-_light->position()));

  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  const GLfloat light_ambient2[4]  = {0.2f, 0.2f, 2.0, 1.0};
  const GLfloat light_diffuse2[4]  = {0.8f, 0.8f, 1.0, 1.0};
  const GLfloat light_specular2[4] = {0.0, 0.0, 1.0, 1.0};

  glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient2);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse2);

  setMouseTracking(true);

  Point3D pmin = _pointsToSurface.boundingMin();
  Point3D pmax = _pointsToSurface.boundingMax();
  
  float s  = 20.0;
  float sx = (pmax.x-pmin.x)/(float)s;
  float sy = (pmax.y-pmin.y)/(float)s;
  float sz = (pmax.z-pmin.z)/(float)s;

  float fmin =   100000000.0;
  float fmax =  -100000000.0;
  for(float i=pmin.x;i<pmax.x;i+=sx) {
    for(float j=pmin.y;j<pmax.y;j+=sy) {
      for(float k=pmin.z;k<pmax.z;k+=sz) {
	float f = (float)_pointsToSurface.computeImplicitFunc(i,j,k);

	if(f<fmin) fmin = f;
	if(f>fmax) fmax = f;

	_f.push_back(Point3D(i,j,k));
	_c.push_back(Point3D(f,0.0,0.0));
      }
    }
  }

  for(unsigned int i=0;i<_c.size();++i) {
    float f = _c[i].x;
    _c[i].x = (f+fmin)/(fmin-fmax);
    _c[i].y = 0.0;
    _c[i].z = 1.0-_c[i].x;
  }
}
