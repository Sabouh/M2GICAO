// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KEYFRAMED_PARAMS
#define KEYFRAMED_PARAMS

#include "misc/extinclude.h"
#include "core/curve.h"
#include "core/sceneIOTags.h"

class LIBRARY_EXPORT KeyframedVar : public IOData {
 public:
  KeyframedVar(const QString &name);
  ~KeyframedVar();

  inline QString name() const {return _name;}
  inline unsigned int nbCurves() const {return static_cast<unsigned int>(_curves.size());}

  inline Curve *curve(unsigned int i=0) const {assert(i<nbCurves()); return _curves[i];}
  inline virtual QString curveName(unsigned int i=0) const {return QString::number(i);}

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  QString _name;
  std::vector<Curve *> _curves;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};



class LIBRARY_EXPORT KeyframedFloat : public KeyframedVar {
 public:
 KeyframedFloat(const QString &name,float defaultVal=0.0f) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal)));
  }

  inline float get()    const  {return _currentVal;}
  inline void  set(float v)    {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const float tmp = curve()->evalAt(x);
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

 private:
  float  _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class LIBRARY_EXPORT KeyframedVector2f : public KeyframedVar {
 public:
 KeyframedVector2f(const QString &name,const Vector2f &defaultVal=Vector2f(0.0f,0.0f)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[1])));
  }

  inline Vector2f get() const        {return _currentVal;}
  inline void set(const Vector2f &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector2f tmp(curve(0)->evalAt(x),curve(1)->evalAt(x));

    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector2f _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class LIBRARY_EXPORT KeyframedVector3f : public KeyframedVar {
 public:
 KeyframedVector3f(const QString &name,const Vector3f &defaultVal=Vector3f(0.0f,0.0f,0.0f)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[1])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[2])));
  }

  inline Vector3f get() const        {return _currentVal;}
  inline void set(const Vector3f &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector3f tmp(curve(0)->evalAt(x),curve(1)->evalAt(x),curve(2)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    case 2: return "Z";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector3f _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class LIBRARY_EXPORT KeyframedVector4f : public KeyframedVar {
 public:
 KeyframedVector4f(const QString &name,const Vector4f &defaultVal=Vector4f(0.0f,0.0f,0.0f,0.0f)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[1])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[2])));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal[3])));
  }

  inline Vector4f get() const        {return _currentVal;}
  inline void set(const Vector4f &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector4f tmp(curve(0)->evalAt(x),curve(1)->evalAt(x),curve(2)->evalAt(x),curve(3)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    case 2: return "Z";
    case 3: return "W";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector4f _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




class LIBRARY_EXPORT KeyframedInt : public KeyframedVar {
 public:
 KeyframedInt(const QString &name,int defaultVal=0) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal)));
  }

  inline int get()    const  {return _currentVal;}
  inline void  set(int v)    {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const int tmp = (int)curve()->evalAt(x);
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

 private:
  int  _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class LIBRARY_EXPORT KeyframedVector2i : public KeyframedVar {
 public:
 KeyframedVector2i(const QString &name,const Vector2i &defaultVal=Vector2i(0,0)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[1])));
  }

  inline Vector2i get() const        {return _currentVal;}
  inline void set(const Vector2i &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector2i tmp((int)curve(0)->evalAt(x),(int)curve(1)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector2i _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class LIBRARY_EXPORT KeyframedVector3i : public KeyframedVar {
 public:
 KeyframedVector3i(const QString &name,const Vector3i &defaultVal=Vector3i(0,0,0)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[1])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[2])));
  }

  inline Vector3i get() const        {return _currentVal;}
  inline void set(const Vector3i &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector3i tmp((int)curve(0)->evalAt(x),(int)curve(1)->evalAt(x),(int)curve(2)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    case 2: return "Z";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector3i _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class LIBRARY_EXPORT KeyframedVector4i : public KeyframedVar {
 public:
 KeyframedVector4i(const QString &name,const Vector4i &defaultVal=Vector4i(0,0,0,0)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[0])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[1])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[2])));
    _curves.push_back(new Curve(Vector2f(0.0f,(float)_currentVal[3])));
  }

  inline Vector4i get() const        {return _currentVal;}
  inline void set(const Vector4i &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Vector4i tmp((int)curve(0)->evalAt(x),(int)curve(1)->evalAt(x),
		       (int)curve(2)->evalAt(x),(int)curve(3)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal==tmp) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "X";
    case 1: return "Y";
    case 2: return "Z";
    case 3: return "W";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Vector4i _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class LIBRARY_EXPORT KeyframedQuaternionf : public KeyframedVar {
 public:
 KeyframedQuaternionf(const QString &name,const Quaternionf &defaultVal=Quaternionf(0.0f,0.0f,0.0f,0.0f)) 
   : KeyframedVar(name),_currentVal(defaultVal) {
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal.w())));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal.x())));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal.y())));
    _curves.push_back(new Curve(Vector2f(0.0f,_currentVal.z())));
  }

  inline Quaternionf get() const        {return _currentVal;}
  inline void set(const Quaternionf &v) {_currentVal = v;   }

  inline bool frameChanged(float x) {
    const Quaternionf tmp(curve(0)->evalAt(x),curve(1)->evalAt(x),curve(2)->evalAt(x),curve(3)->evalAt(x));
    
    // report that the new frame did not change the current variable
    if(_currentVal.w()==tmp.w() && 
       _currentVal.x()==tmp.x() && 
       _currentVal.y()==tmp.y() && 
       _currentVal.z()==tmp.z()) return false;
    
    _currentVal = tmp;
    return true;
  }

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  inline QString curveName(unsigned int i=0) const {
    switch(i) {
    case 0: return "W";
    case 1: return "X";
    case 2: return "Y";
    case 3: return "Z";
    default: return KeyframedVar::curveName(i);
    }
  }

 private:
  Quaternionf _currentVal;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // KEYFRAMED_PARAMS
