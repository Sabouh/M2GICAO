// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/gpuProgram.h"
#include "misc/glutils.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;
      
GPUProgram::GPUProgram(GPUShader* vs,GPUShader* fs,GPUShader *gs,GPUShader *tcs,GPUShader *tes,bool autoLogPrint)
  : _vs(vs),
    _fs(fs),
    _gs(gs),
    _tcs(tcs),
    _tes(tes),
    _printlog(autoLogPrint) {

  _programId = _glf->glCreateProgram();
  _linked = attachAndLink();
    }

GPUProgram::GPUProgram(const QString &vsFile,
                       const QString &fsFile,
                       const QString &gsFile,
                       const QString &tcsFile,
                       const QString &tesFile,
		       bool fromFile,
		       bool autoLogPrint) {

  _printlog = autoLogPrint;
  _vs = _fs = _gs = _tcs = _tes = NULL;

  if(vsFile!="")
    _vs = new GPUShader(VERT,vsFile,autoLogPrint,fromFile);

  if(fsFile!="")
    _fs = new GPUShader(FRAG,fsFile,autoLogPrint,fromFile);

  if(gsFile!="")
    _gs = new GPUShader(GEOM,gsFile,autoLogPrint,fromFile);

  if(tcsFile!="")
    _tcs = new GPUShader(TESSC,tcsFile,autoLogPrint,fromFile);

  if(tesFile!="")
    _tes = new GPUShader(TESSE,tesFile,autoLogPrint,fromFile);

  _programId = _glf->glCreateProgram();
  _linked = attachAndLink();
   }

      
GPUProgram::~GPUProgram() {
  detach();
    
  delete _vs;
  delete _fs;
  delete _gs;
  delete _tcs;
  delete _tes;
  
  _glf->glDeleteProgram(_programId);
}

void GPUProgram::attach() {
  if(_vs!=NULL) {
    _glf->glAttachShader(_programId,_vs->id()); 
  }

  if(_fs!=NULL) {
    _glf->glAttachShader(_programId,_fs->id());
  }

  if(_gs!=NULL) {
    _glf->glAttachShader(_programId,_gs->id());       
  }

  if(_tcs!=NULL) {
    _glf->glAttachShader(_programId,_tcs->id());       
  }

  if(_tes!=NULL) {
    _glf->glAttachShader(_programId,_tes->id());       
  }
}

void GPUProgram::detach() {
        
  if(_vs!=NULL) {
    _glf->glDetachShader(_programId,_vs->id()); 
  }

  if(_fs!=NULL) {
    _glf->glDetachShader(_programId,_fs->id());
  }

  if(_gs!=NULL) {
    _glf->glDetachShader(_programId,_gs->id());  
  }

  if(_tcs!=NULL) {
    _glf->glDetachShader(_programId,_tcs->id());  
  }

  if(_tes!=NULL) {
    _glf->glDetachShader(_programId,_tes->id());  
  }
}

bool GPUProgram::link() {
  _glf->glLinkProgram(_programId);
  GLint r;
  _glf->glGetProgramiv(_programId,GL_LINK_STATUS,&r);
  return r==GL_TRUE;
}

bool GPUProgram::attachAndLink() {
  attach();
  return link();
}

void GPUProgram::reload(GPUShader::RList replace) {
      
  detach();

  bool allOk = true;
  if(_vs!=NULL) {
    allOk = allOk && _vs->loadAndCompile(replace);      
  }

  if(_fs!=NULL) {
    allOk = allOk && _fs->loadAndCompile(replace);
  }

  if(_gs!=NULL) {
    allOk = allOk && _gs->loadAndCompile(replace);
  }

  if(_tcs!=NULL) {
    allOk = allOk && _tcs->loadAndCompile(replace);
  }

  if(_tes!=NULL) {
    allOk = allOk && _tes->loadAndCompile(replace);
  }
  
  if(!allOk){
    std::cout << "reload fail, maybe missing file" << std::endl;
  }

  attachAndLink();

  // reload uniforms 
  for(map<QString,GLint>::iterator i=_uniformLocations.begin();i!=_uniformLocations.end();i++) {
    const QByteArray ba = (*i).first.toLocal8Bit();
    _uniformLocations[(*i).first] = _glf->glGetUniformLocation(_programId,ba.data());
  }

  // free textures 
  _textures.clear();
}


void GPUProgram::addUniform(const QString &uniformName) {
  const QByteArray ba = uniformName.toLocal8Bit();

  if(_linked) {
    GLint location = _glf->glGetUniformLocation(_programId,ba.data());  
    _uniformLocations[uniformName] = location;
    if(location<0 && _printlog)
      cout << "Warning: the shader does not contain the uniform called " << uniformName.toStdString() << endl;
  } else if(_printlog) {
    cout << "Enable to add uniform " << uniformName.toStdString() << ": program not linked..." << endl;
  }
}

bool GPUProgram::haveShaderOfType(SHADER_TYPE type) {
    
  if(type==VERT)
    return _vs!=NULL;
    
  if(type==FRAG)
    return _fs!=NULL;
    
  if(type==GEOM)
    return _gs!=NULL;

  if(type==TESSC)
    return _tcs!=NULL;
    
  if(type==TESSE)
    return _tes!=NULL;

  cout << "Warning : unknown type !" << endl;
  
  return false;
}

QString GPUProgram::filename(SHADER_TYPE type) {
    
  if(type==VERT && _vs!=NULL)
    return _vs->filename();
    
  if(type==FRAG && _fs!=NULL)
    return _fs->filename();
    
  if(type==GEOM && _gs!=NULL)
    return _gs->filename();

  if(type==TESSC && _tcs!=NULL)
    return _tcs->filename();

  if(type==TESSE && _tes!=NULL)
    return _tes->filename();

  cout << "Warning : unknown type !" << endl;
    
  return "";
}

QString GPUProgram::log(SHADER_TYPE type) {
    
  if(type==VERT && _vs!=NULL)
    return _vs->log();
    
  if(type==FRAG && _fs!=NULL)
    return _fs->log();
    
  if(type==GEOM && _gs!=NULL)
    return _gs->log();

  if(type==TESSC && _tcs!=NULL)
    return _tcs->log();

  if(type==TESSE && _tes!=NULL)
    return _tes->log();

  if(type==VERT || 
     type==FRAG || 
     type==GEOM || 
     type==TESSC || 
     type==TESSE) {
    return "";
  } 
     
  cout << "Warning : unknown type !" << endl;
    
  return "";
}

