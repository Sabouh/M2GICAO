// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/gpuShader.h"
#include "misc/glutils.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <QtGui>
#include <QTextStream>
#include <QString>
#include <QFile>

using namespace std;
      
GPUShader::GPUShader(SHADER_TYPE type,const QString &filename,bool printLog,bool fromFile)
  : _filename(filename),
    _log(),
    _type(type),
    _shaderId(0),
    _printLog(printLog),
    _fromFile(fromFile) {
  _created = createShader();

  loadAndCompile();
}

GPUShader::~GPUShader() {
  if(_created){
    _glf->glDeleteShader(_shaderId);
  }
}

bool GPUShader::load(RList replace) {

  QString res;
  if(_fromFile) {
    QFile f(_filename);
    
    if (!f.open(QFile::ReadOnly)) {
      std::cerr << "failed to load shader file " << _filename.toStdString() << "\n";
      return false;
    }
    
    QTextStream stream(&f);
    res = stream.readAll();
    f.close();
  } else {
    res = _filename;
  }

  for(int i=0;i<replace.size();++i) {
    res = res.replace(replace[i].first,replace[i].second);
  }

  string tmp = res.toStdString();
  int l = (int)tmp.length();

  const char *s = tmp.c_str();

  _glf->glShaderSource(_shaderId,1,&s,&l);
  return true;
}
  

bool GPUShader::compile() { 
  _glf->glCompileShader(_shaderId);
  
  int   infologLength = 0;
  int   charsWritten  = 0;
  char* infolog;
  _log = QString();

  _glf->glGetShaderiv(_shaderId,GL_INFO_LOG_LENGTH,&infologLength);
    
  if(infologLength>0) {
    infolog = (char*)malloc(infologLength);
    _glf->glGetShaderInfoLog(_shaderId,infologLength,&charsWritten,infolog);
    _log = QString::fromLocal8Bit(infolog);
    if(infolog[0]!='\0' && _printLog) {
      printInfoLog();
    }
    free(infolog);
  }

  return true;
}

void GPUShader::printInfoLog() {
  cout << "\n\n\n===============================\nInfoLog ---> " << _filename.toStdString() << "\n";
  cout << _log.toStdString() << "\n";
}

bool GPUShader::loadAndCompile(RList replace) {
  return _created && load(replace) && compile();
}

bool GPUShader::createShader() {
  switch(_type) {

  case VERT:
    _shaderId = _glf->glCreateShader(GL_VERTEX_SHADER);
    break;
      
  case FRAG:
    _shaderId = _glf->glCreateShader(GL_FRAGMENT_SHADER);
    break;
      
  case GEOM:
    _shaderId = _glf->glCreateShader(GL_GEOMETRY_SHADER);
    break;
      
  case TESSC:
    _shaderId = _glf->glCreateShader(GL_TESS_CONTROL_SHADER);
    break;

  case TESSE:
    _shaderId = _glf->glCreateShader(GL_TESS_EVALUATION_SHADER);
    break;

  default:
    cout << "Warning : unknown shader type !" << endl;
    return false;
    break;
  }
    
  if(_shaderId==0) {
    cout << "Warning : shader " << _filename.toStdString() << " is not created !" << endl;
    return false;
  }
      
  return true;
} 
