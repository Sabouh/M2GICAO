// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GLSLOADER_H
#define GLSLOADER_H

#include <QObject>
#include "genericCustomNode.h"
#include "genericCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"
#include "misc/vertexarrayObject.h"

#include <Patate/grenaille.h>
#include "surface_mesh/Surface_mesh.h"
using namespace Grenaille;
using namespace surface_mesh;

class GLSPoint
{
public:
    enum {Dim = 3};
    typedef surface_mesh::Scalar Scalar;
    typedef surface_mesh::Point  VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;

    MULTIARCH inline GLSPoint(const VectorType& pos    = Point::Zero(),
                             const VectorType& normal = Point::Zero())
        : _pos   (Eigen::Map< const VectorType >(pos.data())),
          _normal(Eigen::Map< const VectorType >(normal.data()))
    {}
    MULTIARCH inline const Eigen::Map< const VectorType >& pos()    const { return _pos; }
    MULTIARCH inline const Eigen::Map< const VectorType >& normal() const { return _normal; }
private:
    Eigen::Map< const VectorType > _pos, _normal;
};

typedef Grenaille::DistWeightFunc<GLSPoint, Grenaille::SmoothWeightKernel<Scalar> > WeightFunc;
typedef Basket<GLSPoint, WeightFunc, OrientedSphereFit, GLSParam, OrientedSphereScaleSpaceDer, GLSDer, GLSCurvatureHelper, GLSGeomVar> Fit;


class GLSLoaderNode;

class GLSLoaderWidget : public GenericCustomWidget {
  Q_OBJECT
    
public:
  GLSLoaderWidget(GLSLoaderNode *node);

public slots:
  void loadClicked();
  void computeClicked();
  void defaultClicked();
  Vector2f curvMinMax() const { return _curvMinMax->val(); }
  float radius() const { return _radius->val(); }

private:
  QPushButton           *_load;
  QPushButton           *_compute;
  QCheckBox             *_updateRadius;
  QPushButton           *_default;
  TrackballCameraWidget *_camWidget;
  Vector2fSpinWidget    *_curvMinMax;
  QCheckBox             *_updateMinMax;
  FloatSliderWidget     *_radius;
  static QDir            _currentPath;

  friend class GLSLoaderNode;
};


class GLSLoaderNode : public GenericCustomNode {
 public:
  GLSLoaderNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~GLSLoaderNode();

  void apply ();

  void mousePressEvent(const Vector2f &m,QMouseEvent *);
  void mouseMoveEvent (const Vector2f &m,QMouseEvent *);
  void wheelEvent     (const Vector2f &,QWheelEvent *);

  inline NodeWidget *widget() {return _w;}

  virtual const QString constantVertHead();
  virtual const QString defaultVertBody(); 
  virtual const QString constantFragHead();
  virtual const QString defaultFragBody(); 
  virtual const QString constantTessCHead();
  virtual const QString defaultTessCBody(); 
  virtual const QString constantTessEHead();
  virtual const QString defaultTessEBody(); 
  virtual const QString constantGeomHead(); 
  virtual const QString defaultGeomBody(); 

  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);

  void loadObject(const QString &filename);
  void fit(bool init=false);
  float findMinRadius();
  void initCam();
  
  inline TrackballCamera *camera() {return _camera;}

 protected:
  const QStringList defaultUniformNames() const { 
    return QStringList() << "model" << "view" << "proj" << "zmin" << "zmax" << "curvMinMax"; }
  const QStringList defaultUniformTypes() const { 
    return QStringList() << "mat4" << "mat4" << "mat4" << "float" << "float" << "vec2"; }

  void initFBO();
  void cleanFBO();

 private:
  QString               _filename;
  TrackballCamera      *_camera;
  VertexarrayObject    *_vao;
  FloatTexture2D       *_depth;
  GLSLoaderWidget      *_w;
  Surface_mesh          _mesh;
  Vector3f              _meshBarycenter;
  float                 _meshRadius;
  unsigned int          _nbElements;
};

class GLSLoaderHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.GLSLoader")

 public:
  const QString     stringId() const {return "GLSLoaderId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "GLSLoader";}
  const QString     path    () const {return "Geometry/";}
  const QString     desc    () const {return "Curvature computation on 3D objects";}
  const QString     help    () const {
    return tr("This node loads a mesh and computes per-vertex principal curvature values "
              "and directions, as well as geometric variation using Patate Lib (http://patate.gforge.inria.fr)\n\n"
              "The radius parameter (expressed as a fraction of the mesh bounding radius) "
              "controls the neighborhood used to perform this computation.\n\n"
              "Gaussian curvature is visualized using a blue to red color-code whose range can "
              "be adjusted using the Min/Max curvature parameter."
	      );
  }
  
  const QStringList inputNames () const {return QStringList();}
  const QStringList outputNames() const {return QStringList() << "outBuffer0";}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new GLSLoaderNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new GLSLoaderNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};


#endif //GLSLOADER
