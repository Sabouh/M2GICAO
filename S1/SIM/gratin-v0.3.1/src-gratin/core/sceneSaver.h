// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SCENE_SAVER_H
#define SCENE_SAVER_H

#include "misc/extinclude.h"
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>

class IOData;

class LIBRARY_EXPORT SceneSaver {
 public:
  inline const QDomDocument &document()   { return _doc;        }
  inline const QString savingPath() const { return _savingPath; } 
  inline QString absoluteToRelativePath(const QString &absFilePath);
  inline QString relativeToAbsolutePath(const QString &relFilePath);

  bool init(const QString &type,const QString &path=GRATIN_APPLI_DIR);
  bool writeTo(QFile &file);

  // basic types 
  bool saveBool  (const QString &name,bool           val);
  bool saveInt   (const QString &name,int            val);
  bool saveUint  (const QString &name,unsigned int   val);
  bool saveFloat (const QString &name,float          val);
  bool saveDouble(const QString &name,double         val);
  bool saveString(const QString &name,const QString &val);

  // vectors and matrices 
  bool saveVector2u(const QString &name,const Vector2u &v);
  bool saveVector3u(const QString &name,const Vector3u &v);
  bool saveVector4u(const QString &name,const Vector4u &v);

  bool saveVector2i(const QString &name,const Vector2i &v);
  bool saveVector3i(const QString &name,const Vector3i &v);
  bool saveVector4i(const QString &name,const Vector4i &v);
  bool saveVectorXi(const QString &name,const VectorXi &v);

  bool saveVector2f(const QString &name,const Vector2f &v);
  bool saveVector3f(const QString &name,const Vector3f &v);
  bool saveVector4f(const QString &name,const Vector4f &v);
  bool saveVectorXf(const QString &name,const VectorXf &v);

  bool saveVector2d(const QString &name,const Vector2d &v);
  bool saveVector3d(const QString &name,const Vector3d &v);
  bool saveVector4d(const QString &name,const Vector4d &v);
  bool saveVectorXd(const QString &name,const VectorXd &v);

  bool saveMatrix2i(const QString &name,const Matrix2i &m);
  bool saveMatrix3i(const QString &name,const Matrix3i &m);
  bool saveMatrix4i(const QString &name,const Matrix4i &m);
  bool saveMatrixXi(const QString &name,const MatrixXi &m);

  bool saveMatrix2f(const QString &name,const Matrix2f &m);
  bool saveMatrix3f(const QString &name,const Matrix3f &m);
  bool saveMatrix4f(const QString &name,const Matrix4f &m);
  bool saveMatrixXf(const QString &name,const MatrixXf &m);

  bool saveMatrix2d(const QString &name,const Matrix2d &m);
  bool saveMatrix3d(const QString &name,const Matrix3d &m);
  bool saveMatrix4d(const QString &name,const Matrix4d &m);
  bool saveMatrixXd(const QString &name,const MatrixXd &m);

  bool saveQuaternionf(const QString &name,const Quaternionf &q);
  bool saveQuaterniond(const QString &name,const Quaterniond &q);

  // every data that inherits IOData
  bool saveIOData(const QString &name,IOData *data);

 private:
  // search and position the cursor in the corresponding child (push) or parent (pop)
  //inline bool push(const QString &tag);
  inline bool push(const QString &tag,const QString &attribName,const QString &attribVal);
  inline bool pop();

  // document and cursor
  QDomDocument _doc;
  QDomElement  _current;

  // user given path (usually the one in witch the file will be saved)
  // useful to compute relative paths from this given directory 
  QString _savingPath;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline bool SceneSaver::push(const QString &tag,const QString &attribName,const QString &attribVal) {
  QDomElement el = _doc.createElement(tag);
  el.setAttribute(attribName,attribVal);
  _current.appendChild(el);
  _current = el;
  return true;
}

inline bool SceneSaver::pop() {
  _current = _current.parentNode().toElement();
  return true;
}

inline QString SceneSaver::absoluteToRelativePath(const QString &absFilePath) {
  QDir dir(_savingPath);
  return dir.relativeFilePath(absFilePath);
}

inline QString SceneSaver::relativeToAbsolutePath(const QString &relFilePath) {
  QDir dir(_savingPath);
  return dir.cleanPath(dir.absoluteFilePath(relFilePath));
}

#endif // SCENE_SAVER_H
