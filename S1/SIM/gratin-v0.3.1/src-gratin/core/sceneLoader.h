// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "misc/extinclude.h"
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QDir>

class IOData;
class SceneSaver;

class LIBRARY_EXPORT SceneLoader {
 public:
  SceneLoader();

  inline const QString loadingPath() const { return _loadingPath; } 
  inline QString absoluteToRelativePath(const QString &absFilePath);
  inline QString relativeToAbsolutePath(const QString &relFilePath);

  bool readFrom(QFile &file);
  bool readFrom(SceneSaver *saver);
  bool init(const QString &type,const QString &path=GRATIN_APPLI_DIR);

  // basic types 
  bool loadBool  (const QString &name,bool         &val);
  bool loadInt   (const QString &name,int          &val);
  bool loadUint  (const QString &name,unsigned int &val);
  bool loadFloat (const QString &name,float        &val);
  bool loadDouble(const QString &name,double       &val);
  bool loadString(const QString &name,QString      &val);

  // vectors, matrices, quaternions 
  bool loadVector2u(const QString &name,Vector2u &v);
  bool loadVector3u(const QString &name,Vector3u &v);
  bool loadVector4u(const QString &name,Vector4u &v);

  bool loadVector2i(const QString &name,Vector2i &v);
  bool loadVector3i(const QString &name,Vector3i &v);
  bool loadVector4i(const QString &name,Vector4i &v);
  bool loadVectorXi(const QString &name,VectorXi &v);

  bool loadVector2f(const QString &name,Vector2f &v);
  bool loadVector3f(const QString &name,Vector3f &v);
  bool loadVector4f(const QString &name,Vector4f &v);
  bool loadVectorXf(const QString &name,VectorXf &v);

  bool loadVector2d(const QString &name,Vector2d &v);
  bool loadVector3d(const QString &name,Vector3d &v);
  bool loadVector4d(const QString &name,Vector4d &v);
  bool loadVectorXd(const QString &name,VectorXd &v);

  bool loadMatrix2i(const QString &name,Matrix2i &m);
  bool loadMatrix3i(const QString &name,Matrix3i &m);
  bool loadMatrix4i(const QString &name,Matrix4i &m);
  bool loadMatrixXi(const QString &name,MatrixXi &m);

  bool loadMatrix2f(const QString &name,Matrix2f &m);
  bool loadMatrix3f(const QString &name,Matrix3f &m);
  bool loadMatrix4f(const QString &name,Matrix4f &m);
  bool loadMatrixXf(const QString &name,MatrixXf &m);

  bool loadMatrix2d(const QString &name,Matrix2d &m);
  bool loadMatrix3d(const QString &name,Matrix3d &m);
  bool loadMatrix4d(const QString &name,Matrix4d &m);
  bool loadMatrixXd(const QString &name,MatrixXd &m);

  bool loadQuaternionf(const QString &name,Quaternionf &q);
  bool loadQuaterniond(const QString &name,Quaterniond &q);

  // every data that inherits IOData
  bool loadIOData(const QString &name,IOData *data);

 private:
  // return the first child element with the given tag
  inline QDomElement search(const QString &tag);
  
  // return the first child element with the given tag and attribute
  inline QDomElement search(const QString &tag,const QString &attribName,const QString &attribVal);

  // search and position the cursor in the corresponding child (push) or parent (pop)
  inline bool push(const QString &tag,const QString &attribName,const QString &attribVal);
  inline bool pop();

  // document and cursor
  QDomDocument _doc;
  QDomElement  _current;

  // user given path (usually the one in witch the file is loaded)
  // useful to compute relative paths from this given directory 
  QString _loadingPath;

  // log file 
  void writeLog(const QString &name);
  QFile        _debugFile;
  QTextStream  _debugStream;
  bool         _debugInitialized;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline QDomElement SceneLoader::search(const QString &tag) {
  return _current.firstChildElement(tag);
}

inline QDomElement SceneLoader::search(const QString &tag,const QString &attribName,const QString &attribVal) {
  QDomElement elem = _current.firstChildElement(tag);
    
  while(!elem.isNull()) {
    if(elem.attribute(attribName)==attribVal) {
      break;
    }
    elem = elem.nextSiblingElement(tag);
  }

  return elem;
}

inline bool SceneLoader::push(const QString &tag,const QString &attribName,const QString &attribVal) {
  QDomElement el = search(tag,attribName,attribVal);
  if(el.isNull()) return false;
  _current = el;
  return true;
}

inline bool SceneLoader::pop() {
  _current = _current.parentNode().toElement();
  return true;
}

inline QString SceneLoader::absoluteToRelativePath(const QString &absFilePath) {
  QDir dir(_loadingPath);
  return dir.relativeFilePath(absFilePath);
}

inline QString SceneLoader::relativeToAbsolutePath(const QString &relFilePath) {
  QDir dir(_loadingPath);
  return dir.cleanPath(dir.absoluteFilePath(relFilePath));
}


#endif // SCENE_LOADER_H
