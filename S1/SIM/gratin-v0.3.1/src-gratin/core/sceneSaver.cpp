// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/sceneSaver.h"
#include "core/sceneIOTags.h"

#include <QTextStream>

// bool SceneSaver::init(const QString &rootTag,const QString &attribName,const QString &attribVal) {
//   _doc = QDomDocument();
//   _current = _doc.createElement(rootTag);
//   _doc.appendChild(_current);
//   _current.setAttribute(attribName,attribVal);

//   return true;
// }

// bool SceneSaver::init(const QString &version) {
//   _doc = QDomDocument();
//   _current = _doc.createElement(Tags::rootTag());
//   _doc.appendChild(_current);
//   _current.setAttribute(Tags::attribVersion(),version);

//   return true;
// }


bool SceneSaver::init(const QString &type,const QString &path) {
  _savingPath = path;
  _doc = QDomDocument();
  _current = _doc.createElement(Tags::rootTag());
  _doc.appendChild(_current);
  _current.setAttribute(Tags::attribType(),type);

  bool ret = true;
  
  if(!saveInt("majorVersion",GRATIN_MAJOR_VERSION)) ret = false;
  if(!saveInt("minorVersion",GRATIN_MINOR_VERSION)) ret = false;

  IOData::SAVE_MAJOR_VERSION = GRATIN_MAJOR_VERSION;
  IOData::SAVE_MINOR_VERSION = GRATIN_MINOR_VERSION;

  return ret;
}


bool SceneSaver::saveBool(const QString &name,bool val) {
  QDomElement el = _doc.createElement(Tags::boolTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveInt(const QString &name,int val) {
  QDomElement el = _doc.createElement(Tags::intTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveUint(const QString &name,unsigned int val) {
  QDomElement el = _doc.createElement(Tags::uintTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveFloat(const QString &name,float val) {
  QDomElement el = _doc.createElement(Tags::floatTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveDouble(const QString &name,double val) {
  QDomElement el = _doc.createElement(Tags::doubleTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveString(const QString &name,const QString &val) {
  QDomElement el = _doc.createElement(Tags::stringTag());
  el.setAttribute(Tags::attribName(),name);
  el.setAttribute(Tags::attribVal(),val);
  _current.appendChild(el);
  return true;
}

bool SceneSaver::saveVector2u(const QString &name,const Vector2u &v) {
  if(!push(Tags::vector2uTag(),Tags::attribName(),name)) return false;
  bool ret = saveUint(Tags::attribId(0),v[0]) && saveUint(Tags::attribId(1),v[1]);
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector3u(const QString &name,const Vector3u &v) {
  if(!push(Tags::vector3uTag(),Tags::attribName(),name)) return false;
  bool ret = (saveUint(Tags::attribId(0),v[0]) && 
	      saveUint(Tags::attribId(1),v[1]) && 
	      saveUint(Tags::attribId(2),v[2]));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector4u(const QString &name,const Vector4u &v) {
  if(!push(Tags::vector4uTag(),Tags::attribName(),name)) return false;
  bool ret = (saveUint(Tags::attribId(0),v[0]) && 
	      saveUint(Tags::attribId(1),v[1]) && 
	      saveUint(Tags::attribId(2),v[2]) && 
	      saveUint(Tags::attribId(3),v[3])); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector2i(const QString &name,const Vector2i &v) {
  if(!push(Tags::vector2iTag(),Tags::attribName(),name)) return false;
  bool ret = saveInt(Tags::attribId(0),v[0]) && saveInt(Tags::attribId(1),v[1]);
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector3i(const QString &name,const Vector3i &v) {
  if(!push(Tags::vector3iTag(),Tags::attribName(),name)) return false;
  bool ret = (saveInt(Tags::attribId(0),v[0]) && 
	      saveInt(Tags::attribId(1),v[1]) && 
	      saveInt(Tags::attribId(2),v[2]));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector4i(const QString &name,const Vector4i &v) {
  if(!push(Tags::vector4iTag(),Tags::attribName(),name)) return false;
  bool ret = (saveInt(Tags::attribId(0),v[0]) && 
	      saveInt(Tags::attribId(1),v[1]) && 
	      saveInt(Tags::attribId(2),v[2]) && 
	      saveInt(Tags::attribId(3),v[3])); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVectorXi(const QString &name,const VectorXi &v) {
  if(!push(Tags::vectorXiTag(),Tags::attribName(),name)) return false;
  const unsigned int size = v.size();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveInt(Tags::attribId(i),v[i]);
  }
  if(!pop()) return false;
  return ret;
}


bool SceneSaver::saveVector2f(const QString &name,const Vector2f &v) {
  if(!push(Tags::vector2fTag(),Tags::attribName(),name)) return false;
  bool ret = saveFloat(Tags::attribId(0),v[0]) && saveFloat(Tags::attribId(1),v[1]);
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector3f(const QString &name,const Vector3f &v) {
  if(!push(Tags::vector3fTag(),Tags::attribName(),name)) return false;
  bool ret = (saveFloat(Tags::attribId(0),v[0]) && 
	      saveFloat(Tags::attribId(1),v[1]) && 
	      saveFloat(Tags::attribId(2),v[2]));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector4f(const QString &name,const Vector4f &v) {
  if(!push(Tags::vector4fTag(),Tags::attribName(),name)) return false;
  bool ret = (saveFloat(Tags::attribId(0),v[0]) && 
	      saveFloat(Tags::attribId(1),v[1]) && 
	      saveFloat(Tags::attribId(2),v[2]) && 
	      saveFloat(Tags::attribId(3),v[3])); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVectorXf(const QString &name,const VectorXf &v) {
  if(!push(Tags::vectorXfTag(),Tags::attribName(),name)) return false;
  const unsigned int size = v.size();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveFloat(Tags::attribId(i),v[i]);
  }
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector2d(const QString &name,const Vector2d &v) {
  if(!push(Tags::vector2dTag(),Tags::attribName(),name)) return false;
  bool ret = saveDouble(Tags::attribId(0),v[0]) && saveDouble(Tags::attribId(1),v[1]);
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector3d(const QString &name,const Vector3d &v) {
  if(!push(Tags::vector3dTag(),Tags::attribName(),name)) return false;
  bool ret = (saveDouble(Tags::attribId(0),v[0]) && 
	      saveDouble(Tags::attribId(1),v[1]) && 
	      saveDouble(Tags::attribId(2),v[2]));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVector4d(const QString &name,const Vector4d &v) {
  if(!push(Tags::vector4dTag(),Tags::attribName(),name)) return false;
  bool ret = (saveDouble(Tags::attribId(0),v[0]) && 
	      saveDouble(Tags::attribId(1),v[1]) && 
	      saveDouble(Tags::attribId(2),v[2]) && 
	      saveDouble(Tags::attribId(3),v[3])); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveVectorXd(const QString &name,const VectorXd &v) {
  if(!push(Tags::vectorXdTag(),Tags::attribName(),name)) return false;
  const unsigned int size = v.size();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveDouble(Tags::attribId(i),v[i]);
  }
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix2i(const QString &name,const Matrix2i &m) {
  if(!push(Tags::matrix2iTag(),Tags::attribName(),name)) return false;
  bool ret = saveVector2i(Tags::attribId(0),m.col(0)) && saveVector2i(Tags::attribId(1),m.col(1));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix3i(const QString &name,const Matrix3i &m) {
  if(!push(Tags::matrix3iTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector3i(Tags::attribId(0),m.col(0)) && 
	      saveVector3i(Tags::attribId(1),m.col(1)) && 
	      saveVector3i(Tags::attribId(2),m.col(2)));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix4i(const QString &name,const Matrix4i &m) {
  if(!push(Tags::matrix4iTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector4i(Tags::attribId(0),m.col(0)) && 
	      saveVector4i(Tags::attribId(1),m.col(1)) && 
	      saveVector4i(Tags::attribId(2),m.col(2)) && 
	      saveVector4i(Tags::attribId(3),m.col(3))); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrixXi(const QString &name,const MatrixXi &m) {
  if(!push(Tags::matrixXiTag(),Tags::attribName(),name)) return false;
  const unsigned int size = m.cols();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveVectorXi(Tags::attribId(i),m.col(i));
  }
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix2f(const QString &name,const Matrix2f &m) {
  if(!push(Tags::matrix2fTag(),Tags::attribName(),name)) return false;
  bool ret = saveVector2f(Tags::attribId(0),m.col(0)) && saveVector2f(Tags::attribId(1),m.col(1));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix3f(const QString &name,const Matrix3f &m) {
  if(!push(Tags::matrix3fTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector3f(Tags::attribId(0),m.col(0)) && 
	      saveVector3f(Tags::attribId(1),m.col(1)) && 
	      saveVector3f(Tags::attribId(2),m.col(2)));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix4f(const QString &name,const Matrix4f &m) {
  if(!push(Tags::matrix4fTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector4f(Tags::attribId(0),m.col(0)) && 
	      saveVector4f(Tags::attribId(1),m.col(1)) && 
	      saveVector4f(Tags::attribId(2),m.col(2)) && 
	      saveVector4f(Tags::attribId(3),m.col(3))); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrixXf(const QString &name,const MatrixXf &m) {
  if(!push(Tags::matrixXfTag(),Tags::attribName(),name)) return false;
  const unsigned int size = m.cols();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveVectorXf(Tags::attribId(i),m.col(i));
  }
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix2d(const QString &name,const Matrix2d &m) {
  if(!push(Tags::matrix2dTag(),Tags::attribName(),name)) return false;
  bool ret = saveVector2d(Tags::attribId(0),m.col(0)) && saveVector2d(Tags::attribId(1),m.col(1));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix3d(const QString &name,const Matrix3d &m) {
  if(!push(Tags::matrix3dTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector3d(Tags::attribId(0),m.col(0)) && 
	      saveVector3d(Tags::attribId(1),m.col(1)) && 
	      saveVector3d(Tags::attribId(2),m.col(2)));
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrix4d(const QString &name,const Matrix4d &m) {
  if(!push(Tags::matrix4dTag(),Tags::attribName(),name)) return false;
  bool ret = (saveVector4d(Tags::attribId(0),m.col(0)) && 
	      saveVector4d(Tags::attribId(1),m.col(1)) && 
	      saveVector4d(Tags::attribId(2),m.col(2)) && 
	      saveVector4d(Tags::attribId(3),m.col(3))); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveMatrixXd(const QString &name,const MatrixXd &m) {
  if(!push(Tags::matrixXdTag(),Tags::attribName(),name)) return false;
  const unsigned int size = m.cols();
  bool ret = saveUint(Tags::attribNumber(),size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && saveVectorXd(Tags::attribId(i),m.col(i));
  }
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveQuaternionf(const QString &name,const Quaternionf &q) {
  if(!push(Tags::quaternionfTag(),Tags::attribName(),name)) return false;
  bool ret = (saveFloat(Tags::attribId(0),q.w()) && 
	      saveFloat(Tags::attribId(1),q.x()) && 
	      saveFloat(Tags::attribId(2),q.y()) && 
	      saveFloat(Tags::attribId(3),q.z())); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveQuaterniond(const QString &name,const Quaterniond &q) {
  if(!push(Tags::quaterniondTag(),Tags::attribName(),name)) return false;
  bool ret = (saveDouble(Tags::attribId(0),q.w()) && 
	      saveDouble(Tags::attribId(1),q.x()) && 
	      saveDouble(Tags::attribId(2),q.y()) && 
	      saveDouble(Tags::attribId(3),q.z())); 
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::saveIOData(const QString &name,IOData *data) {
  if(!data || !push(Tags::ioDataTag(),Tags::attribName(),name)) return false;
  bool ret = data->save(this);
  if(!pop()) return false;
  return ret;
}

bool SceneSaver::writeTo(QFile &file) {
  QTextStream out(&file);
  out << _doc.toString();
  return true;
}

