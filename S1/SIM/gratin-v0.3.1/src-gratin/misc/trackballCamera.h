// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TRACKBALL_CAMERA_H
#define TRACKBALL_CAMERA_H

#include "misc/extinclude.h"
#include "core/sceneIOTags.h"

#include <Eigen/Geometry>
#include <cmath>

class LIBRARY_EXPORT TrackBall : public IOData {
 public:
  TrackBall(const Vector2f &screenCenter=Vector2f(0,0),float screenRadius=1);
  virtual ~TrackBall() {}

  inline void beginTracking(const Vector2f &pt);
  inline const Quaternionf &track(const Vector2f &pt);
  inline const Quaternionf &rotation() const;

  inline void setScreenParams(const Vector2f &screenCenter,float screenRadius);
  inline void setRotation(const Quaternionf &rotation);

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

 private:
  Quaternionf _rotation;
  Vector2f    _center;
  float       _radius;
  Vector2f    _startPos;
  Vector3f    _startSphPos;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class LIBRARY_EXPORT TrackballCamera : public QObject, public IOData {
Q_OBJECT

 public:
  enum {NONE, ROTATE, MOVEXY, MOVEZ};
  enum {SCENE_CENTERED=0,WORLD_CENTERED=1};

  static const int PERSP=0;
  static const int ORTHO=1;

  TrackballCamera(const Vector2i &viewport,const Vector3f &sceneCenter,float sceneRadius,int proj=PERSP,int rot=SCENE_CENTERED);
  virtual ~TrackballCamera() {}

  inline void setFovy(float fovy);
  inline void setProjection(int projType);
  inline void setRotType(int rotType);
  inline void setShift(const Vector3f &shift);
  inline void setRotation(const Quaternionf &rotation);
  inline void setShiftRotation(const Vector3f &shift,const Quaternionf &rotation);
  inline void setSceneParams(const Vector3f &center,float radius);
  inline void reinit(const Vector2i &viewport,const Vector3f &center,float radius);
  inline void setViewport(const Vector2i &viewport);

  inline void initRotation(const Vector2f &p);
  inline void initMoveXY(const Vector2f &p);
  inline void initMoveZ(const Vector2f &p);
  inline void move(const Vector2f &p);

  inline float zmin()  const { return _zmin;  }
  inline float zmax()  const { return _zmax;  }
  inline float fovy()  const { return _fovy;  }

  // Note: matrices are orthogonals. Normal matrix if the upper-left of mdv 
  inline const Matrix4f &projMatrix ()  const {return _proj; }
  inline const Matrix4f &modelMatrix()  const {return _model;}
  inline const Matrix4f &viewMatrix ()  const {return _view; }
  inline const Vector3f &shift      ()  const {return _shift;}
  inline const Vector3f &sceneCenter()  const {return _sceneCenter;}
  inline float sceneRadius          ()  const {return _sceneRadius;}
  inline const Quaternionf &rotation()  const {return _trackball.rotation();}
  inline int projType() const {return _projType;}
  inline int rotType () const {return _rotType ;}

  // load and save 
  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  inline void rotate(const Vector2f &p);
  inline void moveXY(const Vector2f &p);
  inline void moveZ(const Vector2f &p);

 signals:
  void cameraChanged();

 private:
  void initProjectionMatrix();
  void initModelMatrix();
  void initViewMatrix();

  int       _interaction;
  int       _projType;
  int       _rotType;
  Vector2i  _viewport; 
  Vector2f  _startPoint;
  Vector3f  _sceneCenter;
  float     _sceneRadius;
  float     _fovy;
  Vector3f  _shift;
  float     _zmin;
  float     _zmax;

  TrackBall _trackball;
  Matrix4f  _model;
  Matrix4f  _view;
  Matrix4f  _proj;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void TrackBall::beginTracking(const Vector2f &pt) {
  _startPos    = -(pt-_center);
  _startSphPos = Vector3f(_startPos[0],_startPos[1],_radius);
}

inline const Quaternionf &TrackBall::track(const Vector2f &pt) {
  Vector2f p     = -(pt - _center);
  float    angle = (p-_startPos).norm()*4/_radius;
  Vector3f ptSph = Vector3f(p[0],p[1],_radius);
  Vector3f axis  = (ptSph.cross(_startSphPos)).normalized();
  _rotation      = Quaternionf(AngleAxisf(angle,axis))*_rotation;
  return _rotation;
}

inline void TrackBall::setScreenParams(const Vector2f &screenCenter,float screenRadius) {
  _center = screenCenter;
  _radius = screenRadius;
}

inline void TrackBall::setRotation(const Quaternionf &rotation) {
  _rotation = rotation;
}

inline const Quaternionf &TrackBall::rotation() const {
  return _rotation;
}

inline void TrackballCamera::initRotation(const Vector2f &p) {
  _interaction = ROTATE;
  _startPoint = p;
  _trackball.beginTracking(_startPoint);
}

inline void TrackballCamera::initMoveXY(const Vector2f &p) {
  _interaction = MOVEXY;
  _startPoint = p;
}

inline void TrackballCamera::initMoveZ(const Vector2f &p) {
  _interaction = MOVEZ;
  _startPoint = p;
}

inline void TrackballCamera::move(const Vector2f &p) {
  if(_startPoint==p) return;

  switch(_interaction) {
  case ROTATE: rotate(p); break;
  case MOVEXY: moveXY(p); break;
  case MOVEZ:  moveZ(p);  break;
  default: break;
  }
  emit cameraChanged();
}

inline void TrackballCamera::rotate(const Vector2f &p) {
  if(_rotType==WORLD_CENTERED) {
    _model << _trackball.track(p).matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
  } else {
    Matrix4f t = Matrix4f::Identity();
    t(0,3) = -_sceneCenter[0];
    t(1,3) = -_sceneCenter[1];
    t(2,3) = -_sceneCenter[2];
    Matrix4f r;
    r << _trackball.track(p).matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
    _model = r*t;
  }

  // update params
  _startPoint = p;
  _trackball.beginTracking(_startPoint);
}

inline void TrackballCamera::moveXY(const Vector2f &p) {
  const float s = _sceneRadius/100.0; // sensitivity

  _shift[0] = _shift[0] - (p[0]-_startPoint[0])*s;
  _shift[1] = _shift[1] - (p[1]-_startPoint[1])*s;
  initViewMatrix();

  _startPoint = p;
}

inline void TrackballCamera::moveZ(const Vector2f &p) {
  const float s = _sceneRadius/100.0; // sensitivity
  _shift[2] = _shift[2] + (p[1]-_startPoint[1])*s;
  _shift[2] = std::max(_shift[2],s/10.0f);

  if(_projType==PERSP)
    initViewMatrix();
  else if(_projType==ORTHO) 
    initProjectionMatrix();

  _startPoint = p;
}

inline void TrackballCamera::setFovy(float fovy) {
  _fovy = fovy;
  if(_projType==PERSP) {
    initProjectionMatrix();
    emit cameraChanged();
  }
}

inline void TrackballCamera::setProjection(int projType) {
  if(_projType!=projType) {
    _projType = projType;
    initProjectionMatrix();
    emit cameraChanged();
  }
}

inline void TrackballCamera::setRotType(int rotType) {
  _rotType = rotType;
  initViewMatrix();
  initModelMatrix();
  emit cameraChanged();
}

inline void TrackballCamera::setRotation(const Quaternionf &rotation) {
  _trackball.setRotation(rotation);

  if(_rotType==WORLD_CENTERED) {
    _model << _trackball.rotation().matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
  } else {
    Matrix4f t = Matrix4f::Identity();
    t(0,3) = -_sceneCenter[0];
    t(1,3) = -_sceneCenter[1];
    t(2,3) = -_sceneCenter[2];
    Matrix4f r;
    r << _trackball.rotation().matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
    _model = r*t;
  }
  emit cameraChanged();
}

inline void TrackballCamera::setShift(const Vector3f &shift) {
  _shift = shift;
  initViewMatrix();
  initProjectionMatrix();
  emit cameraChanged();
}

inline void TrackballCamera::setShiftRotation(const Vector3f &shift,const Quaternionf &rotation) {
  // set shift and rotation without sending signals 
  _shift = shift;
  _trackball.setRotation(rotation);

  if(_rotType==WORLD_CENTERED) {
    _model << _trackball.rotation().matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
  } else {
    Matrix4f t = Matrix4f::Identity();
    t(0,3) = -_sceneCenter[0];
    t(1,3) = -_sceneCenter[1];
    t(2,3) = -_sceneCenter[2];
    Matrix4f r;
    r << _trackball.rotation().matrix(),MatrixXf::Zero(3,1),MatrixXf::Zero(1,3),1;
    _model = r*t;
  }

  initViewMatrix();
  initProjectionMatrix();
}

inline void TrackballCamera::reinit(const Vector2i &viewport,const Vector3f &center,float radius) {
  _viewport    = viewport;
  _sceneCenter = center;
  _sceneRadius = radius;
  _startPoint = Vector2f(0,0);
  Vector2f v((float)(_viewport[0]/2),(float)(_viewport[1]/2));
  _trackball = TrackBall(v,std::min(_viewport[0],_viewport[1]));
  _shift = Vector3f(0,0,3.0f*_sceneRadius);

  initModelMatrix();
  initViewMatrix();
  initProjectionMatrix();
  emit cameraChanged();
}

inline void TrackballCamera::setSceneParams(const Vector3f &center,float radius) {
  _sceneCenter = center;
  _sceneRadius = radius;
  initProjectionMatrix();
  initViewMatrix();
  initModelMatrix();
  emit cameraChanged();
}

inline void TrackballCamera::setViewport(const Vector2i &viewport) {
  _viewport = viewport;
  const Vector2f center((float)(_viewport[0]/2),(float)(_viewport[1]/2));
  const float radius = std::min(center[0],center[1])*2.0f;
  _trackball.setScreenParams(center,radius);
  initProjectionMatrix();
  emit cameraChanged();
}

#endif // TRACKBALL_CAMERA_H
