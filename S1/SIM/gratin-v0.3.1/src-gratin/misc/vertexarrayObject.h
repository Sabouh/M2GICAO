// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef VERTEXARRAY_OBJECT_H
#define VERTEXARRAY_OBJECT_H

#include "extinclude.h"
#include <assert.h>
#include <vector>
#include <iostream>

class LIBRARY_EXPORT VertexarrayObject : public GlContext {
 public:
  struct BufferData {
    GLuint      id;
    GLsizeiptr  buffersize;
    GLint       attribsize;
    GLsizei     stride;
  };
  
  struct IndexData {
    GLuint        id;
    GLsizeiptr    buffersize;
  };

 VertexarrayObject()
   : _target(GL_ARRAY_BUFFER),
    _type(GL_FLOAT),
    _indexTarget(GL_ELEMENT_ARRAY_BUFFER),
    _indexType(GL_UNSIGNED_INT),
    _indices(NULL) {
    _glf->glGenVertexArrays(1,&_vao);
  }
  
  ~VertexarrayObject() {
    clear();
    _glf->glDeleteVertexArrays(1,&_vao);
  }

  // increment the index layout each time this function is called (begins at 0)
  inline void addAttrib (GLsizeiptr buffersize,const void *data,GLint attribsize=4,
			 GLsizei stride=0,GLenum usage=GL_STATIC_DRAW);

  // set the attribute defined by the index layout (using glBufferData func)
  inline void setAttrib (GLuint index,GLsizeiptr buffersize,const void *data,GLint attribsize=4,
			 GLsizei stride=0,GLenum usage=GL_STATIC_DRAW);

  // set the attribute defined by the index layout (using glBufferData func)
  inline void setAttribUnsync(GLuint index,GLsizeiptr buffersize,const void *data,GLint attribsize=4,
			      GLsizei stride=0,GLenum usage=GL_STATIC_DRAW);
  
  // add/set indices
  inline void setIndices(GLsizeiptr buffersize,const void *data,GLenum usage=GL_STATIC_DRAW);
  inline void setIndicesUnsync(GLsizeiptr buffersize,const void *data,GLenum usage=GL_STATIC_DRAW);

  // update buffer functions (uses glBufferSubData func)
  inline void updateAttribArray(GLuint index,GLintptr offset,GLsizeiptr size,const void *data);
  inline void updateIndexArray(GLintptr offset,GLsizeiptr size,const void *data);

  // map and unmap functions (uses glMap/UnmapBuffer)
  inline void *mapAttrib       (GLuint index,GLenum access=GL_WRITE_ONLY);
  inline void *mapAttribUnsync (GLuint index,GLenum access=GL_WRITE_ONLY,GLenum usage=GL_DYNAMIC_DRAW);
  inline void *mapIndices      (GLenum access=GL_WRITE_ONLY);
  inline void *mapIndicesUnsync(GLenum access=GL_WRITE_ONLY,GLenum usage=GL_DYNAMIC_DRAW);
  inline bool  unmapAttrib();
  inline bool  unmapIndices();

  inline void removeIndices  ();
  inline void removeAttrib   (GLuint index);
  inline void clear          ();

  inline void bind();
  inline void unbind();

  // drawing using the buffer of indices 
  inline void drawElements(GLenum mode,GLsizei count);

  // drawing without indices
  inline void drawArrays(GLenum mode,GLint first,GLsizei count);

 private:
  GLuint _vao;
  GLenum _target;
  GLenum _type;
  GLenum _indexTarget;
  GLenum _indexType;

  IndexData  *_indices;

  std::vector<BufferData *> _userAttribs;

  inline void removeBufferData(BufferData *&bd);
  inline void removeIndexData (IndexData  *&id);
};

inline void VertexarrayObject::bind() {
  _glf->glBindVertexArray(_vao);
}

inline void VertexarrayObject::unbind() {
  _glf->glBindVertexArray(0);
}

inline void VertexarrayObject::drawElements(GLenum mode,GLsizei count) {
  assert(_indices!=NULL);
  _glf->glDrawElements(mode,count,_indexType,0);
}

inline void VertexarrayObject::drawArrays(GLenum mode,GLint first,GLsizei count) {
  _glf->glDrawArrays(mode,first,count);
}

inline void VertexarrayObject::setIndices(GLsizeiptr buffersize,const void *data,GLenum usage) {
  assert(buffersize>0);

  if(_indices==NULL) {
    _indices = new IndexData;
    _glf->glGenBuffers(1,&(_indices->id));
  }

  _indices->buffersize = buffersize;

  bind();
  _glf->glBindBuffer(_indexTarget,_indices->id);
  _glf->glBufferData(_indexTarget,_indices->buffersize,data,usage);
  unbind();
}

inline void VertexarrayObject::setIndicesUnsync(GLsizeiptr buffersize,const void *data,GLenum usage) {
  assert(buffersize>0);

  if(_indices==NULL) {
    _indices = new IndexData;
    _glf->glGenBuffers(1,&(_indices->id));
  }

  _indices->buffersize = buffersize;

  bind();
  _glf->glBindBuffer(_indexTarget,_indices->id);
  _glf->glBufferData(_indexTarget,_indices->buffersize,NULL,usage);
  _glf->glBufferData(_indexTarget,_indices->buffersize,data,usage);
  unbind();
}

inline void VertexarrayObject::addAttrib(GLsizeiptr buffersize,const void *data,GLint attribsize,GLsizei stride,GLenum usage) {
  assert(buffersize>0);

  BufferData *attrib = new BufferData;
  const unsigned int index = static_cast<unsigned int>(_userAttribs.size());

  attrib = new BufferData;
  _glf->glGenBuffers(1,&(attrib->id));
  _userAttribs.push_back(attrib);

  setAttrib(index,buffersize,data,attribsize,stride,usage);
  bind();
  _glf->glBindBuffer(_target,attrib->id);
  _glf->glVertexAttribPointer(index,attrib->attribsize,_type,GL_FALSE,attrib->stride,0);  
  _glf->glEnableVertexAttribArray(index); 
  unbind();
}


inline void VertexarrayObject::setAttrib(GLuint index,GLsizeiptr buffersize,const void *data,GLint attribsize,GLsizei stride,GLenum usage) {

  assert(buffersize>0);
  assert(index<_userAttribs.size());

  BufferData *attrib = _userAttribs[index];

  attrib->buffersize = buffersize;
  attrib->attribsize = attribsize;
  attrib->stride     = stride;

  _glf->glBindBuffer(_target,attrib->id);
  _glf->glBufferData(_target,attrib->buffersize,data,usage);
}

inline void VertexarrayObject::setAttribUnsync(GLuint index,GLsizeiptr buffersize,const void *data,GLint attribsize,GLsizei stride,GLenum usage) {

  assert(buffersize>0);
  assert(index<_userAttribs.size());

  BufferData *attrib = _userAttribs[index];

  attrib->buffersize = buffersize;
  attrib->attribsize = attribsize;
  attrib->stride     = stride;

  _glf->glBindBuffer(_target,attrib->id);
  _glf->glBufferData(_target,attrib->buffersize,NULL,usage);
  _glf->glBufferData(_target,attrib->buffersize,data,usage);
}

inline void VertexarrayObject::updateAttribArray(GLuint index,GLintptr offset,GLsizeiptr size,const void *data) {
  assert(index<_userAttribs.size());

  _glf->glBindBuffer(_target,_userAttribs[index]->id);
  _glf->glBufferSubData(_target,offset,size,data);
}

inline void VertexarrayObject::updateIndexArray(GLintptr offset,GLsizeiptr size,const void *data) {
  assert(_indices!=NULL);

  _glf->glBindBuffer(_indexTarget,_indices->id);
  _glf->glBufferSubData(_indexTarget,offset,size,data);
}

inline void *VertexarrayObject::mapAttrib(GLuint index,GLenum access) {
  assert(index<_userAttribs.size());
  _glf->glBindBuffer(_target,_userAttribs[index]->id);
  return _glf->glMapBuffer(_target,access);
}

inline void *VertexarrayObject::mapAttribUnsync(GLuint index,GLenum access,GLenum usage) {
  assert(index<_userAttribs.size());
  BufferData *attrib = _userAttribs[index];
  _glf->glBindBuffer(_target,attrib->id);
  _glf->glBufferData(_target,attrib->buffersize,NULL,usage);
  return _glf->glMapBuffer(_target,access);
}

inline void *VertexarrayObject::mapIndices(GLenum access) {
  _glf->glBindBuffer(_indexTarget,_indices->id);
  return _glf->glMapBuffer(_indexTarget,access);
}

inline void *VertexarrayObject::mapIndicesUnsync(GLenum access,GLenum usage) {
  _glf->glBindBuffer(_indexTarget,_indices->id);
  _glf->glBufferData(_indexTarget,_indices->buffersize,NULL,usage);
  return _glf->glMapBuffer(_indexTarget,access);
}

inline bool VertexarrayObject::unmapAttrib() {
  return _glf->glUnmapBuffer(_target);
}

inline bool VertexarrayObject::unmapIndices() {
  return _glf->glUnmapBuffer(_indexTarget);
}


inline void VertexarrayObject::removeIndices() {
  if(_indices) removeIndexData(_indices);
}

inline void VertexarrayObject::removeAttrib(GLuint index) {
  assert(index<_userAttribs.size());

  removeBufferData(_userAttribs[index]);
}

inline void VertexarrayObject::clear() {
  if(_indices) removeIndexData(_indices);    
  
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    removeBufferData(_userAttribs[i]);
  }
  
  _userAttribs.clear();
}

inline void VertexarrayObject::removeBufferData(BufferData *&bd) {
  _glf->glDeleteBuffers(1,&(bd->id));
  delete bd;
  bd = NULL;
}

inline void VertexarrayObject::removeIndexData(IndexData *&id) {
  _glf->glDeleteBuffers(1,&(id->id));
  delete id;
  id = NULL;
}

#endif // VERTEXARRAY_OBJECT_H
