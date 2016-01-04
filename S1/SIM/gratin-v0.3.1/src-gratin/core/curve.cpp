// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/curve.h"
#include <iostream>
#include <QRegExp>
#include <Eigen/Cholesky>

using namespace std;

CurveInterpolator::CurveInterpolator(const Vector2f &pt) {
  _points.push_back(pt);
}

CurveInterpolator::CurveInterpolator(CurveInterpolator *curve) {
  if(!curve) return;

  _points   = std::vector<Vector2f>(curve->nbPoints());
  _tangents = std::vector<Vector4f>(curve->nbTangents());
  
  for(unsigned int i=0;i<curve->nbPoints();++i) {
    _points[i] = curve->point(i);
  }
  
  for(unsigned int i=0;i<curve->nbTangents();++i) {
    _tangents[i] = curve->tangent(i);
  }
}

void CurveInterpolator::addKeyframe(const Vector2f &pt) {
  vector<Vector2f>::iterator it = _points.begin();
  
  while(it!=_points.end() && (*it)[0]<pt[0]) {
    it++;
  }

  if(it!=_points.end() && (*it)[0]==pt[0]) {
    // simply move the current keyframe
    (*it)[1] = pt[1];
  } else {
    // add a new control point
    _points.insert(it,pt);
  }
}

void CurveInterpolator::setKeyframe(const Vector2f &pt,unsigned int i) {
  assert(i<nbPoints());

  // points should be kept ordered
  if((i>0 && pt[0]<_points[i-1][0]) || 
     (i<nbPoints()-1 && pt[0]>_points[i+1][0]))
    return;

  _points[i] = pt;
}

void CurveInterpolator::setTangent(const Vector2f &pt, unsigned int i, unsigned int side) {
    if(i>=_tangents.size()) return;
    _tangents[i][2*side]   = pt[0];
    _tangents[i][2*side+1] = pt[1];
}

void CurveInterpolator::setTangent(const Vector4f &pt, unsigned int i){
    if(i>=_tangents.size()) return;
    _tangents[i] = pt;
}

void CurveInterpolator::delKeyframe(unsigned int i) {
  assert(i<nbPoints());

  if(nbPoints()>1)
    // we need to keep at least one point in a curve
    _points.erase(_points.begin()+i);
}

void SplineInterpolator::addKeyframe(const Vector2f &pt) {
  const unsigned int nb = nbPoints();
  CurveInterpolator::addKeyframe(pt);
  if(nb!=nbPoints()) {
    nbPointsChanged();
  }
  computeSolution();
}

void SplineInterpolator::setKeyframe(const Vector2f &pt,unsigned int i) {
  CurveInterpolator::setKeyframe(pt,i);
  computeSolution();
}

void SplineInterpolator::delKeyframe(unsigned int i) {
  const unsigned int nb = nbPoints();
  CurveInterpolator::delKeyframe(i);
  if(nb!=nbPoints()) {
    nbPointsChanged();
  }
  computeSolution();
}

void SplineInterpolator::computeSolution() {
  if(nbPoints()<3) return;
  
  const unsigned int ms = _A.rows();
  const unsigned int np = nbPoints();
  
  unsigned int p = 0;
  unsigned int i = 0;
  unsigned int j = 0;

  // constraint matrix  
  for(unsigned int s=0;s<np-1;++s) {

    // f(xi) = yi
    i++;
    _A(i,j  ) = 1.0f;               // 1 
    _A(i,j+1) = _points[p][0];      // x
    _A(i,j+2) = _A(i,j+1)*_A(i,j+1);// x^2
    _A(i,j+3) = _A(i,j+2)*_A(i,j+1);// x^3
    _b(i)     = _points[p][1];      // y 

    // f(xi+1) = yi+1
    i++;
    p++;
    _A(i,j  ) = 1.0f;               // 1 
    _A(i,j+1) = _points[p][0];      // x
    _A(i,j+2) = _A(i,j+1)*_A(i,j+1);// x^2
    _A(i,j+3) = _A(i,j+2)*_A(i,j+1);// x^3
    _b(i)     = _points[p][1];      // y 
  
    // check if we have to stop 
    if(s==np-2) break;

    // fi'(x)=fi+1'(x)
    i++;
    _A(i,j+1) = 1.0f;               // 1
    _A(i,j+2) = 2.0f*_points[p][0]; // 2x
    _A(i,j+3) = 3.0f*_A(i-1,j+2);   // 3x^2
    _A(i,j+5) = -_A(i,j+1);         // -1
    _A(i,j+6) = -_A(i,j+2);         // -2x
    _A(i,j+7) = -_A(i,j+3);         // -3x^2

    // fi''(x)=fi+1''(x)
    i++;
    _A(i,j+2) = 2.0f;               // 2
    _A(i,j+3) = 6.0f*_points[p][0]; // 6x
    _A(i,j+6) = -_A(i,j+2);         // -2
    _A(i,j+7) = -_A(i,j+3);         // -6x

    j+=4;
  }

  // boundary conditions (natural spline)
  _A(0,2)       = 2.0f;
  _A(0,3)       = 6.0f*_points[0][0];
  _A(ms-1,ms-2) = 2.0f;
  _A(ms-1,ms-1) = 6.0f*_points[np-1][0];

  _x = _A.inverse()*_b;

  // tried this, but does not work... 
  //_A.ldlt().solve(_b,&_x);
}

void SplineInterpolator::nbPointsChanged() {
  if(nbPoints()<3) return;
  const unsigned int nb = (nbPoints()-1)*4;  
  _A = MatrixXf::Zero(nb,nb);
  _b = VectorXf::Zero(nb);
}

bool SplineInterpolator::load(SceneLoader *loader) {
  bool ret = CurveInterpolator::load(loader);
  
  nbPointsChanged();
  computeSolution();
  
  return ret;
}


void HermiteInterpolator::splitAt(float t, unsigned int i) {
    // De Casteljau
    Vector2f pt2 = _points[i-1] + _tangents[i-1].head<2>();
    Vector2f pt3 = _points[i+1] + _tangents[i+1].tail<2>();

    Vector2f pt12 = _tangents[i-1].head<2>()*t + _points[i-1];
    Vector2f pt23 = (pt2-pt3)*t + pt2;
    Vector2f pt34 = -_tangents[i+1].tail<2>()*t + pt3;

    Vector2f pt123 = (pt23 - pt12)*t + pt12;
    Vector2f pt234 = (pt34 - pt23)*t + pt23;

    Vector2f pt1234 = (pt234 - pt123) * t + pt123;

    _tangents[i-1].head<2>() = pt12 - _points[i-1];
    _tangents[ i ].tail<2>() = pt123 - pt1234;
    _tangents[ i ].head<2>() = pt234 - pt1234;
    _tangents[i+1].tail<2>() = pt34 - _points[i+1];
}

void HermiteInterpolator::addKeyframe(const Vector2f &pt) {
    const unsigned int nb = nbPoints();

    unsigned int i=0;
    float t = findParam(pt[0],i);

    CurveInterpolator::addKeyframe(pt);
    if(nb!=nbPoints()) {
        for(unsigned int i=0;i<nbPoints();++i) {
            if(_points[i]==pt) {
                Vector4f tangent(2,0,-2,0);
                _tangents.insert(_tangents.begin()+i,tangent);

                if(i>0 && i+1<nbPoints())
                    splitAt(t,i);
                break;
            }
        }
    }
}

void HermiteInterpolator::delKeyframe(unsigned int i) {
  const unsigned int nb = nbPoints();
  CurveInterpolator::delKeyframe(i);
  if(nb!=nbPoints())
    _tangents.erase(_tangents.begin()+i);
}

void HermiteInterpolator::setTangent(const Vector2f &pt, unsigned int i, unsigned int side) {
    CurveInterpolator::setTangent(pt,i,side);
}

void HermiteInterpolator::initTangents() {
    // add tangents if needed
    int nb = (int)nbPoints()-(int)_tangents.size();
    for(int i=0;i<nb;++i) {
        _tangents.push_back(Vector4f(2,0,-2,0));
    }

    // remove tangents if needed
    nb = (int)_tangents.size()-(int)nbPoints();
    for(int i=0;i<nb;++i) {
        _tangents.pop_back();
    }
}


const vector<Vector2f> CurveInterpolator::samplePoints(float x1,float x2,unsigned int nb) const {
  vector<Vector2f> pts;

  const float s = fabs(x2-x1)/(float)nb;

  for(float x=x1;x<=x2;x+=s) {
    pts.push_back(Vector2f(x,evalAt(x)));
  }

  return pts;
}

const vector<Vector2f> CurveInterpolator::sampleLines(float x1,float x2,unsigned int nb) const {
  return samplePoints(x1,x2,nb);
}

inline bool CurveInterpolator::save(SceneSaver *saver) {
  bool ret = true;

  if(!saver->saveUint("nbPoints"  ,nbPoints()))   ret = false;
  if(!saver->saveUint("nbTangents",nbTangents())) ret = false;

  for(unsigned int i=0;i<nbPoints();++i) {
    if(!saver->saveVector2f(Tags::attribId("controlPoint",i),_points[i])) 
      ret = false;
  }
  
  for(unsigned int i=0;i<nbTangents();++i) {
    if(!saver->saveVector4f(Tags::attribId("controlTangent",i),_tangents[i]))
      ret = false;
  }

  return ret;
}

inline bool CurveInterpolator::load(SceneLoader *loader) {
  unsigned int nbp=1,nbt=1;
  bool ret = true;

  if(!loader->loadUint("nbPoints"  ,nbp)) ret = false;
  if(!loader->loadUint("nbTangents",nbt)) ret = false;

  _points   = vector<Vector2f>(nbp);
  _tangents = vector<Vector4f>(nbt);

  for(unsigned int i=0;i<nbp;++i) {
    if(!loader->loadVector2f(Tags::attribId("controlPoint",i),_points[i])) 
      ret = false;
  }
  
  for(unsigned int i=0;i<nbt;++i) {
    if(!loader->loadVector4f(Tags::attribId("controlTangent",i),_tangents[i]))
      ret = false;
  }
  
  return ret;
}



QStringList Curve::_interpNames   = QStringList() << "Linear" << "Step" << "Shepard" << "Spline" << "Hermite";
QStringList Curve::_behaviorNames = QStringList() << "Constant" << "Linear" << "Repeat" << "Repeat offset" << "Repeat mirror";


Curve::Curve(const Vector2f &pt,int interpolation,int leftBehavior,int rightBehavior)
  : _interpType(interpolation),
    _leftType(leftBehavior),
    _rightType(rightBehavior) {
  
  _interpolator  = createInterpolator(_interpType,pt);
  _leftBehavior  = createBehavior(CurveBehavior::LEFT,leftBehavior);
  _rightBehavior = createBehavior(CurveBehavior::RIGHT,rightBehavior);
}

Curve::~Curve() {
  delete _leftBehavior;
  delete _rightBehavior;
  delete _interpolator;
}

void Curve::setInterpolation(int interpolation) {
  if(interpolation==interpType()) return;
  
  if(CurveInterpolator *ci = createInterpolator(interpolation,_interpolator)) {
    delete _interpolator; _interpolator = ci;
    _interpType = interpolation;
    _leftBehavior->setInterpolator(_interpolator);
    _rightBehavior->setInterpolator(_interpolator);
  }
}

void Curve::setLeftBehavior(int behavior) {
  if(behavior==_leftType) return;

  if(CurveBehavior *cb = createBehavior(CurveBehavior::LEFT,behavior)) {
    delete _leftBehavior; 
    _leftBehavior = cb;
    _leftType = behavior;
  }
}

void Curve::setRightBehavior(int behavior) {
  if(behavior==_rightType) return;

  if(CurveBehavior *cb = createBehavior(CurveBehavior::RIGHT,behavior)) {
    delete _rightBehavior; 
    _rightBehavior = cb;
    _rightType = behavior;
  }
}

CurveInterpolator *Curve::createInterpolator(int interpolation,const Vector2f &pt) {
  switch(interpolation) {
  case LINEAR_INTERP:   return new LinearInterpolator (pt);break;
  case STEP_INTERP:     return new StepInterpolator   (pt);break;
  case SHEPARD_INTERP:  return new ShepardInterpolator(pt);break;
  case SPLINE_INTERP:   return new SplineInterpolator (pt);break;
  case HERMITE_INTERP:  return new HermiteInterpolator(pt);break;
  default:              return new LinearInterpolator (pt);break;
  }
}

CurveInterpolator *Curve::createInterpolator(int interpolation,CurveInterpolator *curve) {
  if(!curve) return NULL;

  switch(interpolation) {
  case LINEAR_INTERP:   return new LinearInterpolator (curve);break;
  case STEP_INTERP:     return new StepInterpolator   (curve);break;
  case SHEPARD_INTERP:  return new ShepardInterpolator(curve);break;
  case SPLINE_INTERP:   return new SplineInterpolator (curve);break;
  case HERMITE_INTERP:  return new HermiteInterpolator(curve);break;
  default:              return new LinearInterpolator (curve);break;
  }
}

CurveBehavior *Curve::createBehavior(int side,int behavior) {
  switch(behavior) {
  case CONSTANT_BEHAVIOR:      return new ConstantBehavior    (side,_interpolator);break;
  case LINEAR_BEHAVIOR:        return new LinearBehavior      (side,_interpolator);break;
  case REPEAT_BEHAVIOR:        return new RepeatBehavior      (side,_interpolator);break;
  case REPEAT_OFFSET_BEHAVIOR: return new RepeatOffsetBehavior(side,_interpolator);break;
  case REPEAT_MIRROR_BEHAVIOR: return new RepeatMirrorBehavior(side,_interpolator);break;
  default:                     return new ConstantBehavior    (side,_interpolator);break;
  }
}

const vector<Vector2f> CurveBehavior::samplePoints(float x1,float x2,unsigned int nb) const {
  vector<Vector2f> pts;

  const float s = fabs(x2-x1)/(float)nb;

  for(float x=x1;x<=x2;x+=s) {
    pts.push_back(Vector2f(x,evalAt(x)));
  }

  return pts;
}

const vector<Vector2f> CurveBehavior::sampleLines(float x1,float x2,unsigned int nb) const {
  return samplePoints(x1,x2,nb);
}
const vector<Vector2f> Curve::samplePoints(float x1,float x2,unsigned int nb) const {
  vector<Vector2f> pts;

  const float s = fabs(x2-x1)/(float)nb;

  for(float x=x1;x<=x2;x+=s) {
    pts.push_back(Vector2f(x,evalAt(x)));
  }

  return pts;
}

const vector<Vector2f> Curve::sampleLines(float x1,float x2,unsigned int nb) const {
  return samplePoints(x1,x2,nb);
}

inline bool Curve::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveInt("interpType",_interpType))        ret = false;
  if(!saver->saveInt("leftType"  ,_leftType))          ret = false;
  if(!saver->saveInt("rightType" ,_rightType))         ret = false;
  if(!saver->saveIOData("interpolator",_interpolator)) ret = false;
  return ret;
}

inline bool Curve::load(SceneLoader *loader) {
  int interp=LINEAR_INTERP,bleft=CONSTANT_BEHAVIOR,bright=CONSTANT_BEHAVIOR;
  bool ret = true;

  if(!loader->loadInt("interpType",interp)) ret = false;
  if(!loader->loadInt("leftType"  ,bleft )) ret = false;
  if(!loader->loadInt("rightType" ,bright)) ret = false;

  setInterpolation(interp);
  setLeftBehavior(bleft);
  setRightBehavior(bright);

  if(!loader->loadIOData("interpolator",_interpolator)) ret = false;
  return ret;
}
