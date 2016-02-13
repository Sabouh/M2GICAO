// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/extinclude.h"
#include "core/sceneLoader.h"
#include "core/sceneIOTags.h"
#include "core/sceneSaver.h"
#include <iostream>
#include <QTextStream>
//#include <QCoreApplication>

using namespace std;

SceneLoader::SceneLoader() 
  : _debugFile(GRATIN_LOG_FILE) {
  //  : _debugFile(QCoreApplication::applicationDirPath()+"/resources/config/gratin.log") {
  _debugInitialized = false;
}

bool SceneLoader::readFrom(QFile &file) {
  QString errorStr;
  int errorLine;
  int errorColumn;

  QFileInfo info(file);
  if(!("." + info.suffix() == GRATIN_CUSTOM_EXT) && !("." + info.suffix() == GRATIN_EXT)) {
    return false;
  }
  
  if(!_doc.setContent(&file,true,&errorStr,&errorLine,&errorColumn)) {
    return false;
  } 
  
  _debugInitialized = false;

  return true;
}

bool SceneLoader::readFrom(SceneSaver *saver) {
  _doc = saver->document();

  _debugInitialized = false;

  return true;
}

// bool SceneLoader::init(const QString &version) {
//   _current = _doc.documentElement();

//   _debugInitialized = false;

//   if(_current.tagName()!=Tags::rootTag() || 
//      _current.attribute(Tags::attribVersion())!=version)
//     return false;

//   return true;
// }

// bool SceneLoader::init(const QString &rootTag,const QString &attribName,const QString &attribVal) {
//   _current = _doc.documentElement();
//   _debugInitialized = false;

//   if(_current.tagName()!=rootTag || 
//      _current.attribute(attribName)!=attribVal)
//     return false;

//   return true;
// }

bool SceneLoader::init(const QString &type,const QString &path) {
  _loadingPath = path;
  _current = _doc.documentElement();

  _debugInitialized = false;

  if(_current.tagName()!=Tags::rootTag() || 
     _current.attribute(Tags::attribType())!=type)
    return false;

  int majorVersion = GRATIN_MAJOR_VERSION;
  int minorVersion = GRATIN_MINOR_VERSION;
  bool ret = true;

  if(!loadInt("majorVersion",majorVersion)) ret = false;
  if(!loadInt("minorVersion",minorVersion)) ret = false;

  IOData::LOAD_MAJOR_VERSION = majorVersion;
  IOData::LOAD_MINOR_VERSION = minorVersion;

  return ret;
}


bool SceneLoader::loadBool(const QString &name,bool &val) {
  QDomElement el = search(Tags::boolTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}

  QString v = el.attribute(Tags::attribVal());
  if(v.isEmpty()) {writeLog(name); return false;}

  bool ok;
  val = (bool)(v.toInt(&ok));

  if(!ok) writeLog(name);

  return ok;
}

bool SceneLoader::loadInt(const QString &name,int &val) {
  QDomElement el = search(Tags::intTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}

  QString v = el.attribute(Tags::attribVal());
  if(v.isEmpty()) {writeLog(name); return false;}

  bool ok;
  val = v.toInt(&ok);

  if(!ok) writeLog(name);

  return ok;
}

bool SceneLoader::loadUint(const QString &name,unsigned int &val) {
  QDomElement el = search(Tags::uintTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}

  QString v = el.attribute(Tags::attribVal());
  if(v.isEmpty()) {writeLog(name); return false;}

  bool ok;
  val = v.toUInt(&ok);

  if(!ok) writeLog(name);

  return ok;
}


bool SceneLoader::loadFloat(const QString &name,float &val) {
  QDomElement el = search(Tags::floatTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}

  QString v = el.attribute(Tags::attribVal());
  if(v.isEmpty()) {writeLog(name); return false;}

  bool ok;
  val = v.toFloat(&ok);
  
  if(!ok) writeLog(name);

  return ok;
}

bool SceneLoader::loadDouble(const QString &name,double &val) {
  QDomElement el = search(Tags::doubleTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}

  QString v = el.attribute(Tags::attribVal());
  if(v.isEmpty()) {writeLog(name); return false;}

  bool ok;
  val = v.toDouble(&ok);

  if(!ok) writeLog(name);

  return ok;
}

bool SceneLoader::loadString(const QString &name,QString &val) {
  QDomElement el = search(Tags::stringTag(),Tags::attribName(),name);
  if(el.isNull()) {writeLog(name); return false;}
  val = el.attribute(Tags::attribVal());
  return true;
}

bool SceneLoader::loadVector2u(const QString &name,Vector2u &v) {
  if(!push(Tags::vector2uTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = loadUint(Tags::attribId(0),v[0]) && loadUint(Tags::attribId(1),v[1]);  

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector3u(const QString &name,Vector3u &v) {
  if(!push(Tags::vector3uTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadUint(Tags::attribId(0),v[0]) && 
	      loadUint(Tags::attribId(1),v[1]) && 
	      loadUint(Tags::attribId(2),v[2]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector4u(const QString &name,Vector4u &v) {
  if(!push(Tags::vector4uTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadUint(Tags::attribId(0),v[0]) && 
	      loadUint(Tags::attribId(1),v[1]) && 
	      loadUint(Tags::attribId(2),v[2]) && 
	      loadUint(Tags::attribId(3),v[3]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}
  
bool SceneLoader::loadVector2i(const QString &name,Vector2i &v) {
  if(!push(Tags::vector2iTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = loadInt(Tags::attribId(0),v[0]) && loadInt(Tags::attribId(1),v[1]);  
  
  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector3i(const QString &name,Vector3i &v) {
  if(!push(Tags::vector3iTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadInt(Tags::attribId(0),v[0]) && 
	      loadInt(Tags::attribId(1),v[1]) && 
	      loadInt(Tags::attribId(2),v[2]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector4i(const QString &name,Vector4i &v) {
  if(!push(Tags::vector4iTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadInt(Tags::attribId(0),v[0]) && 
	      loadInt(Tags::attribId(1),v[1]) && 
	      loadInt(Tags::attribId(2),v[2]) && 
	      loadInt(Tags::attribId(3),v[3]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVectorXi(const QString &name,VectorXi &v) {
  if(!push(Tags::vectorXiTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  unsigned int size = v.size();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  v.resize(size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && loadInt(Tags::attribId(i),v[i]);
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector2f(const QString &name,Vector2f &v) {
  if(!push(Tags::vector2fTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = loadFloat(Tags::attribId(0),v[0]) && loadFloat(Tags::attribId(1),v[1]);

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector3f(const QString &name,Vector3f &v) {
  if(!push(Tags::vector3fTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadFloat(Tags::attribId(0),v[0]) && 
	      loadFloat(Tags::attribId(1),v[1]) && 
	      loadFloat(Tags::attribId(2),v[2]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector4f(const QString &name,Vector4f &v) {
  if(!push(Tags::vector4fTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadFloat(Tags::attribId(0),v[0]) && 
	      loadFloat(Tags::attribId(1),v[1]) && 
	      loadFloat(Tags::attribId(2),v[2]) && 
	      loadFloat(Tags::attribId(3),v[3]));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVectorXf(const QString &name,VectorXf &v) {
  if(!push(Tags::vectorXfTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  unsigned int size = v.size();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  v.resize(size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && loadFloat(Tags::attribId(i),v[i]);
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector2d(const QString &name,Vector2d &v) {
  if(!push(Tags::vector2dTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = loadDouble(Tags::attribId(0),v[0]) && loadDouble(Tags::attribId(1),v[1]);
 
  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVector3d(const QString &name,Vector3d &v) {
  if(!push(Tags::vector3dTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadDouble(Tags::attribId(0),v[0]) && 
	      loadDouble(Tags::attribId(1),v[1]) && 
	      loadDouble(Tags::attribId(2),v[2]));
  
  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}
 
  return ret;
}

bool SceneLoader::loadVector4d(const QString &name,Vector4d &v) {
  if(!push(Tags::vector4dTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadDouble(Tags::attribId(0),v[0]) && 
	      loadDouble(Tags::attribId(1),v[1]) && 
	      loadDouble(Tags::attribId(2),v[2]) && 
	      loadDouble(Tags::attribId(3),v[3]));
  
  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadVectorXd(const QString &name,VectorXd &v) {
  if(!push(Tags::vectorXdTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  unsigned int size = v.size();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  v.resize(size);
  for(unsigned int i=0;i<size;++i) {
    ret = ret && loadDouble(Tags::attribId(i),v[i]);
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix2i(const QString &name,Matrix2i &m) {
  if(!push(Tags::matrix2iTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector2i v[2];
  bool ret = loadVector2i(Tags::attribId(0),v[0]) && loadVector2i(Tags::attribId(1),v[1]);
  m.col(0) = v[0];
  m.col(1) = v[1];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix3i(const QString &name,Matrix3i &m) {
  if(!push(Tags::matrix3iTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector3i v[3];
  bool ret = (loadVector3i(Tags::attribId(0),v[0]) && 
	      loadVector3i(Tags::attribId(1),v[1]) && 
	      loadVector3i(Tags::attribId(2),v[2]));
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix4i(const QString &name,Matrix4i &m) {
  if(!push(Tags::matrix4iTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector4i v[4];
  bool ret = (loadVector4i(Tags::attribId(0),v[0]) && 
	      loadVector4i(Tags::attribId(1),v[1]) && 
	      loadVector4i(Tags::attribId(2),v[2]) && 
	      loadVector4i(Tags::attribId(3),v[3])); 
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];
  m.col(3) = v[3];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrixXi(const QString &name,MatrixXi &m) {
  if(!push(Tags::matrixXiTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  unsigned int size = m.cols();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  m.resize(m.rows(),size);
  for(unsigned int i=0;i<size;++i) {
    VectorXi v;
    ret = ret && loadVectorXi(Tags::attribId(i),v);
    m.col(i) = v;
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix2f(const QString &name,Matrix2f &m) {
  if(!push(Tags::matrix2fTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector2f v[2];
  bool ret = loadVector2f(Tags::attribId(0),v[0]) && loadVector2f(Tags::attribId(1),v[1]);
  m.col(0) = v[0];
  m.col(1) = v[1];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix3f(const QString &name,Matrix3f &m) {
  if(!push(Tags::matrix3fTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector3f v[3];
  bool ret = (loadVector3f(Tags::attribId(0),v[0]) && 
	      loadVector3f(Tags::attribId(1),v[1]) && 
	      loadVector3f(Tags::attribId(2),v[2]));
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix4f(const QString &name,Matrix4f &m) {
  if(!push(Tags::matrix4fTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector4f v[4];
  bool ret = (loadVector4f(Tags::attribId(0),v[0]) && 
	      loadVector4f(Tags::attribId(1),v[1]) && 
	      loadVector4f(Tags::attribId(2),v[2]) && 
	      loadVector4f(Tags::attribId(3),v[3])); 
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];
  m.col(3) = v[3];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrixXf(const QString &name,MatrixXf &m) {
  if(!push(Tags::matrixXfTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  unsigned int size = m.cols();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  m.resize(m.rows(),size);
  for(unsigned int i=0;i<size;++i) {
    VectorXf v;
    ret = ret && loadVectorXf(Tags::attribId(i),v);
    m.col(i) = v;
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix2d(const QString &name,Matrix2d &m) {
  if(!push(Tags::matrix2dTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector2d v[2];
  bool ret = loadVector2d(Tags::attribId(0),v[0]) && loadVector2d(Tags::attribId(1),v[1]);
  m.col(0) = v[0];
  m.col(1) = v[1];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix3d(const QString &name,Matrix3d &m) {
  if(!push(Tags::matrix3dTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector3d v[3];
  bool ret = (loadVector3d(Tags::attribId(0),v[0]) && 
	      loadVector3d(Tags::attribId(1),v[1]) && 
	      loadVector3d(Tags::attribId(2),v[2]));
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrix4d(const QString &name,Matrix4d &m) {
  if(!push(Tags::matrix4dTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  Vector4d v[4];
  bool ret = (loadVector4d(Tags::attribId(0),v[0]) && 
	      loadVector4d(Tags::attribId(1),v[1]) && 
	      loadVector4d(Tags::attribId(2),v[2]) && 
	      loadVector4d(Tags::attribId(3),v[3])); 
  m.col(0) = v[0];
  m.col(1) = v[1];
  m.col(2) = v[2];
  m.col(3) = v[3];

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadMatrixXd(const QString &name,MatrixXd &m) {
  if(!push(Tags::matrixXdTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  unsigned int size = m.cols();
  bool ret = loadUint(Tags::attribNumber(),size);
  if(!ret) {pop(); writeLog(name); return false;}
  m.resize(m.rows(),size);
  for(unsigned int i=0;i<size;++i) {
    VectorXd v;
    ret = ret && loadVectorXd(Tags::attribId(i),v);
    m.col(i) = v;
  }

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadQuaternionf(const QString &name,Quaternionf &q) {
  if(!push(Tags::quaternionfTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadFloat(Tags::attribId(0),q.w()) && 
	      loadFloat(Tags::attribId(1),q.x()) && 
	      loadFloat(Tags::attribId(2),q.y()) && 
	      loadFloat(Tags::attribId(3),q.z()));

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadQuaterniond(const QString &name,Quaterniond &q) {
  if(!push(Tags::quaterniondTag(),Tags::attribName(),name))  {writeLog(name); return false;}
  bool ret = (loadDouble(Tags::attribId(0),q.w()) && 
	      loadDouble(Tags::attribId(1),q.x()) && 
	      loadDouble(Tags::attribId(2),q.y()) && 
	      loadDouble(Tags::attribId(3),q.z()));
  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

bool SceneLoader::loadIOData(const QString &name,IOData *data) {
  if(!data || !push(Tags::ioDataTag(),Tags::attribName(),name)) {writeLog(name); return false;}
  bool ret = data->load(this);

  if(!ret) writeLog(name);
  if(!pop()) {writeLog(name); return false;}

  return ret;
}

void SceneLoader::writeLog(const QString &name) {
    
  
  if(!_debugInitialized) {
    if(_debugFile.isOpen()) 
      _debugFile.close();

    _debugFile.open(QIODevice::WriteOnly | QIODevice::Text);
    _debugStream.setDevice(&_debugFile);
    _debugInitialized = true;
  }

  QDomElement elem = _current;
  _debugStream << "error while trying to load " << name << ": tagName = " << elem.tagName() << "\n";
  
  while(!elem.isNull() && elem.isElement()) {
    _debugStream << "\t From " << elem.tagName() << " (" << elem.attribute(Tags::attribName()) << ")\n";
    elem = elem.parentNode().toElement();
  }
  
  _debugStream << "\n\n";
}

