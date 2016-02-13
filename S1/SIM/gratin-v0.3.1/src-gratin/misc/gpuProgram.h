// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include "misc/extinclude.h"
#include "misc/gpuShader.h"
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <QString>
#include <QList>
#include <QPair>

#include <QOpenGLContext>

class LIBRARY_EXPORT GPUProgram : public GlContext {
    
 public:
  GPUProgram(GPUShader *vs,GPUShader *fs,GPUShader *gs=NULL,GPUShader *tcs=NULL,GPUShader *tes=NULL,
	     bool autoLogPrint=true);

  GPUProgram(const QString &vsFile,
             const QString &fsFile,
             const QString &gsFile="",
             const QString &tcsFile="",
             const QString &tesFile="",
	     bool fromFile = true,
	     bool autoLogPrint = true);
    
  ~GPUProgram();
    
  void addUniform(const QString &uniformName);
  void reload(GPUShader::RList replace=GPUShader::RList());

  inline GLuint id() const;
  bool haveShaderOfType(SHADER_TYPE type);
  QString filename(SHADER_TYPE type);
  QString log(SHADER_TYPE type);

  inline void enable();
  inline void disable();

  inline GLint getUniformLocation(const QString &uniformName);
  inline GLint getAttributeLocation(const QString &attributeName);

  inline void setUniform1f(const QString &uniformName,GLfloat v);
  inline void setUniform2f(const QString &uniformName,GLfloat v1,GLfloat v2);
  inline void setUniform3f(const QString &uniformName,GLfloat v1,GLfloat v2,GLfloat v3);
  inline void setUniform4f(const QString &uniformName,GLfloat v1,GLfloat v2,GLfloat v3,GLfloat v4);
  inline void setUniform1f(GLint loc,GLfloat v);
  inline void setUniform2f(GLint loc,GLfloat v1,GLfloat v2);
  inline void setUniform3f(GLint loc,GLfloat v1,GLfloat v2,GLfloat v3);
  inline void setUniform4f(GLint loc,GLfloat v1,GLfloat v2,GLfloat v3,GLfloat v4);

  inline void setUniform1i(const QString &uniformName,GLint v);
  inline void setUniform2i(const QString &uniformName,GLint v1,GLint v2);
  inline void setUniform3i(const QString &uniformName,GLint v1,GLint v2,GLint v3);
  inline void setUniform4i(const QString &uniformName,GLint v1,GLint v2,GLint v3,GLint v4);
  inline void setUniform1i(GLint loc,GLint v);
  inline void setUniform2i(GLint loc,GLint v1,GLint v2);
  inline void setUniform3i(GLint loc,GLint v1,GLint v2,GLint v3);
  inline void setUniform4i(GLint loc,GLint v1,GLint v2,GLint v3,GLint v4);

  inline void setUniform1fv(const QString &uniformName,GLfloat *v,GLsizei count=1);
  inline void setUniform2fv(const QString &uniformName,GLfloat *v,GLsizei count=1);
  inline void setUniform3fv(const QString &uniformName,GLfloat *v,GLsizei count=1);
  inline void setUniform4fv(const QString &uniformName,GLfloat *v,GLsizei count=1);
  inline void setUniform1fv(GLint loc,GLfloat *v,GLsizei count=1);
  inline void setUniform2fv(GLint loc,GLfloat *v,GLsizei count=1);
  inline void setUniform3fv(GLint loc,GLfloat *v,GLsizei count=1);
  inline void setUniform4fv(GLint loc,GLfloat *v,GLsizei count=1);

  inline void setUniform1iv(const QString &uniformName,GLint *v,GLsizei count=1);
  inline void setUniform2iv(const QString &uniformName,GLint *v,GLsizei count=1);
  inline void setUniform3iv(const QString &uniformName,GLint *v,GLsizei count=1);
  inline void setUniform4iv(const QString &uniformName,GLint *v,GLsizei count=1);
  inline void setUniform1iv(GLint loc,GLint *v,GLsizei count=1);
  inline void setUniform2iv(GLint loc,GLint *v,GLsizei count=1);
  inline void setUniform3iv(GLint loc,GLint *v,GLsizei count=1);
  inline void setUniform4iv(GLint loc,GLint *v,GLsizei count=1);
 
  inline void setUniformMatrix2fv(const QString &uniformName,GLfloat *v,GLsizei count=1,GLboolean transpose=false);
  inline void setUniformMatrix3fv(const QString &uniformName,GLfloat *v,GLsizei count=1,GLboolean transpose=false);
  inline void setUniformMatrix4fv(const QString &uniformName,GLfloat *v,GLsizei count=1,GLboolean transpose=false);
  inline void setUniformMatrix2fv(GLint loc,GLfloat *v,GLsizei count=1,GLboolean transpose=false);
  inline void setUniformMatrix3fv(GLint loc,GLfloat *v,GLsizei count=1,GLboolean transpose=false);
  inline void setUniformMatrix4fv(GLint loc,GLfloat *v,GLsizei count=1,GLboolean transpose=false);

  inline void setUniformTexture(const QString &uniformName,GLenum type,GLuint textureName);
  inline void setUniformTexture(GLint loc,GLenum type,GLuint textureName);


 protected:
  void attach();
  bool link();
  bool attachAndLink();
  void detach();

 private:
        
  GPUShader* _vs;
  GPUShader* _fs;
  GPUShader* _gs;
  GPUShader* _tcs;
  GPUShader* _tes;

  GLuint     _programId;
  bool       _printlog;
  bool       _linked;

  std::map<QString,GLint> _uniformLocations;

  // GLint   = location
  // GLenum1 = texture num
  // GLuint  = texture name
  // GLenum2 = texture type
  std::map<GLint,std::pair<GLenum,std::pair<GLuint,GLenum> > >_textures;
};
  
inline void GPUProgram::enable() {
  _glf->glUseProgram(_programId);
    
  for(std::map<GLint,std::pair<GLenum,std::pair<GLuint,GLenum> > >::iterator i=_textures.begin();i!=_textures.end();++i) {
    _glf->glActiveTexture((*i).second.first);
    _glf->glBindTexture((*i).second.second.second,(*i).second.second.first);
  }

}

inline void GPUProgram::disable() {
  for(std::map<GLint,std::pair<GLenum,std::pair<GLuint,GLenum> > >::reverse_iterator i=_textures.rbegin();i!=_textures.rend();++i) {
    _glf->glActiveTexture((*i).second.first);
  }

  _glf->glUseProgram(0);

  _glf->glActiveTexture(GL_TEXTURE0);
}

inline GLuint GPUProgram::id() const {
  return _programId;
}


inline void GPUProgram::setUniformTexture(const QString &uniformName,GLenum type,GLuint textureName) {
  _glf->glUseProgram(_programId);

  GLint loc = getUniformLocation(uniformName);
  setUniformTexture(loc, type, textureName);
}

inline void GPUProgram::setUniform1f(GLint loc,GLfloat v) {
  _glf->glUniform1f(loc,v);
}

inline void GPUProgram::setUniform2f(GLint loc,GLfloat v1,GLfloat v2) {
  _glf->glUniform2f(loc,v1,v2);
}

inline void GPUProgram::setUniform3f(GLint loc,GLfloat v1,GLfloat v2,GLfloat v3) {
  _glf->glUniform3f(loc,v1,v2,v3);
}
      
inline void GPUProgram::setUniform4f(GLint loc,GLfloat v1,GLfloat v2,GLfloat v3,GLfloat v4) {
  _glf->glUniform4f(loc,v1,v2,v3,v4);
}

inline void GPUProgram::setUniform1i(GLint loc,GLint v) {
  _glf->glUniform1i(loc,v);
}

inline void GPUProgram::setUniform2i(GLint loc,GLint v1,GLint v2) {
  _glf->glUniform2i(loc,v1,v2);
}

inline void GPUProgram::setUniform3i(GLint loc,GLint v1,GLint v2,GLint v3) {
  _glf->glUniform3i(loc,v1,v2,v3);
}
      
inline void GPUProgram::setUniform4i(GLint loc,GLint v1,GLint v2,GLint v3,GLint v4) {
  _glf->glUniform4i(loc,v1,v2,v3,v4);
}

inline void GPUProgram::setUniform1fv(GLint loc,GLfloat *v,GLsizei count) {
  _glf->glUniform1fv(loc,count,v);
}

inline void GPUProgram::setUniform2fv(GLint loc,GLfloat *v,GLsizei count) {
  _glf->glUniform2fv(loc,count,v);
}

inline void GPUProgram::setUniform3fv(GLint loc,GLfloat *v,GLsizei count) {
  _glf->glUniform3fv(loc,count,v);
}

inline void GPUProgram::setUniform4fv(GLint loc,GLfloat *v,GLsizei count) {
  _glf->glUniform4fv(loc,count,v);
}

inline void GPUProgram::setUniform1iv(GLint loc,GLint *v,GLsizei count) {
  _glf->glUniform1iv(loc,count,v);
}

inline void GPUProgram::setUniform2iv(GLint loc,GLint *v,GLsizei count) {
  _glf->glUniform2iv(loc,count,v);
}

inline void GPUProgram::setUniform3iv(GLint loc,GLint *v,GLsizei count) {
  _glf->glUniform3iv(loc,count,v);
}

inline void GPUProgram::setUniform4iv(GLint loc,GLint *v,GLsizei count) {
  _glf->glUniform4iv(loc,count,v);
}

inline void GPUProgram::setUniformMatrix2fv(GLint loc,GLfloat *v,GLsizei count,GLboolean transpose) {
  _glf->glUniformMatrix2fv(loc,count,transpose,v);
}

inline void GPUProgram::setUniformMatrix3fv(GLint loc,GLfloat *v,GLsizei count,GLboolean transpose) {
  _glf->glUniformMatrix3fv(loc,count,transpose,v);
}

inline void GPUProgram::setUniformMatrix4fv(GLint loc,GLfloat *v,GLsizei count,GLboolean transpose) {
  _glf->glUniformMatrix4fv(loc,count,transpose,v);
}

inline void GPUProgram::setUniform1f(const QString &uniformName,GLfloat v) {
  setUniform1f(_uniformLocations[uniformName],v);
}

inline void GPUProgram::setUniform2f(const QString &uniformName,GLfloat v1,GLfloat v2) {
  setUniform2f(_uniformLocations[uniformName],v1,v2);
}

inline void GPUProgram::setUniform3f(const QString &uniformName,GLfloat v1,GLfloat v2,GLfloat v3) {
  setUniform3f(_uniformLocations[uniformName],v1,v2,v3);
}

inline void GPUProgram::setUniform4f(const QString &uniformName,GLfloat v1,GLfloat v2,GLfloat v3,GLfloat v4) {
  setUniform4f(_uniformLocations[uniformName],v1,v2,v3,v4);
}

inline void GPUProgram::setUniform1i(const QString &uniformName,GLint v) {
  setUniform1i(_uniformLocations[uniformName],v);
}

inline void GPUProgram::setUniform2i(const QString &uniformName,GLint v1,GLint v2) {
  setUniform2i(_uniformLocations[uniformName],v1,v2);
}

inline void GPUProgram::setUniform3i(const QString &uniformName,GLint v1,GLint v2,GLint v3) {
  setUniform3i(_uniformLocations[uniformName],v1,v2,v3);
}
      
inline void GPUProgram::setUniform4i(const QString &uniformName,GLint v1,GLint v2,GLint v3,GLint v4) {
  setUniform4i(_uniformLocations[uniformName],v1,v2,v3,v4);
}

inline void GPUProgram::setUniform1fv(const QString &uniformName,GLfloat *v,GLsizei count) {
  setUniform1fv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform2fv(const QString &uniformName,GLfloat *v,GLsizei count) {
  setUniform2fv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform3fv(const QString &uniformName,GLfloat *v,GLsizei count) {
  setUniform3fv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform4fv(const QString &uniformName,GLfloat *v,GLsizei count) {
  setUniform4fv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform1iv(const QString &uniformName,GLint *v,GLsizei count) {
  setUniform1iv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform2iv(const QString &uniformName,GLint *v,GLsizei count) {
  setUniform2iv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform3iv(const QString &uniformName,GLint *v,GLsizei count) {
  setUniform3iv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniform4iv(const QString &uniformName,GLint *v,GLsizei count) {
  setUniform4iv(_uniformLocations[uniformName],v,count);
}

inline void GPUProgram::setUniformMatrix2fv(const QString &uniformName,GLfloat *v,GLsizei count,GLboolean transpose) {
  setUniformMatrix2fv(_uniformLocations[uniformName],v,count,transpose);
}

inline void GPUProgram::setUniformMatrix3fv(const QString &uniformName,GLfloat *v,GLsizei count,GLboolean transpose) {
  setUniformMatrix3fv(_uniformLocations[uniformName],v,count,transpose);
}

inline void GPUProgram::setUniformMatrix4fv(const QString &uniformName,GLfloat *v,GLsizei count,GLboolean transpose) {
  setUniformMatrix4fv(_uniformLocations[uniformName],v,count,transpose);
}

inline void GPUProgram::setUniformTexture(GLint loc,GLenum type,GLuint textureName) {
  GLenum textureNum;
  GLenum num;

  const std::map<GLint,std::pair<GLenum,std::pair<GLuint,GLenum> > >::iterator it = _textures.find(loc);

  if(it==_textures.end()) {
    textureNum = GL_TEXTURE0+static_cast<GLenum>(_textures.size());
  } else {
    textureNum = (*it).second.first;
  }
    
  _glf->glActiveTexture(textureNum);
  _glf->glBindTexture(type,textureName);  

  num = textureNum-GL_TEXTURE0;
  _glf->glUniform1i(loc,num);
  _textures[loc] = std::pair<GLenum,std::pair<GLuint,GLenum> >(textureNum,std::pair<GLuint,GLenum>(textureName,type));
}

inline GLint GPUProgram::getUniformLocation(const QString &uniformName) {
  std::map<QString,GLint>::const_iterator u = _uniformLocations.find(uniformName);
  if(u == _uniformLocations.end()) {return -1;}
  else {return u->second;}
}

#endif // GPUPROGRAM_H
