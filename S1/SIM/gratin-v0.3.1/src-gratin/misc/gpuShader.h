// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GPUSHADER_H
#define GPUSHADER_H

#include "misc/extinclude.h"
#include <QString>
#include <QList>
#include <QPair>

enum SHADER_TYPE {VERT,FRAG,TESSC,TESSE,GEOM};

class LIBRARY_EXPORT GPUShader : public GlContext {

 public: 
  typedef QList<QPair<QString,QString> > RList;

  GPUShader(SHADER_TYPE type,const QString &filename,bool printLog=true,bool fromFile=true);

  ~GPUShader();
    
  bool load(RList replace=RList());
  bool compile();
  bool loadAndCompile(RList replace=RList());
    
  inline GLuint id() const;
  inline SHADER_TYPE type() const;
  inline QString filename() const;
  inline QString log() const;

 protected:
  QString _filename;
  QString _log;

 private:
  SHADER_TYPE _type; 
    
  GLuint      _shaderId;
  bool        _printLog;    
  bool        _created;
  bool        _fromFile;

  bool createShader();
  void printInfoLog();
};
  
inline GLuint GPUShader::id() const {
  return _shaderId;
}
  
inline SHADER_TYPE GPUShader::type() const {
  return _type;
}

inline QString GPUShader::filename() const {
  return _filename;
}

inline QString GPUShader::log() const {
  return _log;
}

#endif // GPUSHADER
