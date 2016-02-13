// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/trackballCamera.h"
#include <iostream>
using namespace std;

TrackBall::TrackBall(const Vector2f &screenCenter,float screenRadius)
  : _rotation(AngleAxisf(0,Vector3f::UnitZ())),
    _center(screenCenter),
    _radius(screenRadius),
    _startPos(0,0),
    _startSphPos(0,0,0) {

}

bool TrackBall::save(SceneSaver  *saver ) {
  bool ret = true;
  if(!saver->saveQuaternionf("rotation"    ,_rotation)) ret = false;
  if(!saver->saveVector2f   ("screenCenter",_center))   ret = false;
  if(!saver->saveFloat      ("screenRadius",_radius))   ret = false;
  return ret;
}

bool TrackBall::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadQuaternionf("rotation"    ,_rotation)) ret = false;
  if(!loader->loadVector2f   ("screenCenter",_center))   ret = false;
  if(!loader->loadFloat      ("screenRadius",_radius))   ret = false;
  return ret;
}

TrackballCamera::TrackballCamera(const Vector2i &viewport,const Vector3f &sceneCenter,float sceneRadius,int proj,int rot)
  : _interaction(NONE),
    _projType(proj),
    _rotType(rot),
    _viewport(viewport),
    _startPoint(0,0),
    _sceneCenter(sceneCenter),
    _sceneRadius(sceneRadius),
    _fovy(45.0f),
    _shift(0,0,3.0f*_sceneRadius) {


  Vector2f v((float)(_viewport[0]/2),(float)(_viewport[1]/2));
  _trackball = TrackBall(v,std::min(_viewport[0],_viewport[1]));

  initModelMatrix();
  initViewMatrix();
  initProjectionMatrix();
}


void TrackballCamera::initProjectionMatrix() {

  if(_projType==PERSP) {
    const float c      = 100.0f;
    const float f      = 1.0f/tan(_fovy/2.0f);
    const float aspect = (float)_viewport[0]/(float)_viewport[1];
    const float ne     = _sceneRadius/c;
    const float fa     = _sceneRadius*c;

    _proj << 
      f/aspect,0,0,0, 
      0,f,0,0, 
      0,0,(fa+ne)/(ne-fa),(2*fa*ne)/(ne-fa),
      0,0,-1,0;

  } else if(_projType==ORTHO) {
    const float c      =  100.0f;
    const float right  = _shift[2];
    const float top    = right;
    const float left   = -right;
    const float bottom = -top;
    const float ne     = _sceneRadius/c;
    const float fa     = _sceneRadius*c;

    const float rml = right-left;
    const float tmb = top-bottom;
    const float fmn = fa-ne;

    _proj << 
      2.0f/rml,0,0,-(right+left)/rml, 
      0,2.0f/tmb,0,-(top+bottom)/tmb,
      0,0,-2.0f/fmn,-(fa+ne)/fmn,
      0,0,0,1;

  } else {
    _proj.setIdentity();
  }
}

void TrackballCamera::initModelMatrix() {
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
}

void TrackballCamera::initViewMatrix() {
  //const Vector3f s = _projType==ORTHO ? Vector3f(_shift[0],_shift[1],3.0f*_sceneRadius) : _shift;
  const Vector3f s = _shift;
  const Vector3f e = _rotType==WORLD_CENTERED ? _sceneCenter+s : s;      // eye pos
  const Vector3f c = _rotType==WORLD_CENTERED ? _sceneCenter : Vector3f(0,0,0);        // center 
  const Vector3f w = Vector3f(0,1,0);     // up vector
  const Vector3f v = (c-e).normalized();  // view vector 
  const Vector3f r = v.cross(w);          // right vector 
  const Vector3f u = r.cross(v);          // up vector (normalization)

  _view << 
    r[0],u[0],-v[0],-e[0], 
    r[1],u[1],-v[1],-e[1], 
    r[2],u[2],-v[2],-e[2], 
    0,0,0,1;

  _zmin = _shift[2]-_sceneRadius;
  _zmax = _shift[2]+_sceneRadius;
}

bool TrackballCamera::save(SceneSaver  *saver ) {
  return (saver->saveInt     ("projType"   ,_projType)    && 
	  saver->saveInt     ("rotType"    ,_rotType)     &&
	  saver->saveVector2i("viewport"   ,_viewport)    &&
	  saver->saveVector3f("sceneCenter",_sceneCenter) &&
	  saver->saveFloat   ("sceneRadius",_sceneRadius) &&
	  saver->saveFloat   ("fovy"       ,_fovy)        &&
	  saver->saveVector3f("shift"      ,_shift)       &&
	  saver->saveIOData  ("trackball"  ,&_trackball));
}

bool TrackballCamera::load(SceneLoader *loader) {
  bool ret = (loader->loadInt     ("projType"   ,_projType)    && 
	      loader->loadInt     ("rotType"    ,_rotType)     &&
	      loader->loadVector2i("viewport"   ,_viewport)    &&
	      loader->loadVector3f("sceneCenter",_sceneCenter) &&
	      loader->loadFloat   ("sceneRadius",_sceneRadius) &&
	      loader->loadFloat   ("fovy"       ,_fovy)        &&
	      loader->loadVector3f("shift"      ,_shift)       &&
	      loader->loadIOData  ("trackball"  ,&_trackball));

  initModelMatrix();
  initViewMatrix();
  initProjectionMatrix();

  return ret;
}
