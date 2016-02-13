// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXT_DRAWER
#define TEXT_DRAWER

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "misc/gpuProgram.h"
#include "misc/texture2D.h"
#include "misc/vertexarrayObject.h"
#include <vector>

class LIBRARY_EXPORT TextDrawer : public GlContext {
 public:
  TextDrawer();
  ~TextDrawer();

  inline void draw(const QString  &text,
		   const Vector2f &pos,
		   const Vector2f &size,
		   const Matrix4f &mvp=Matrix4f::Identity(),
		   const Vector4f &tcolor=Vector4f(0,0,0,1),
		   const Vector4f &bcolor=Vector4f(0,0,0,0));
		  

 private:
  static void initGPUData();
  static void clearGPUData();

  inline void updateBuffers(const QString  &text,
			    const Vector2f &pos,
			    const Vector2f &size);

  static GPUProgram        *_ptext;
  static FloatTexture2D    *_tfont;
  static VertexarrayObject *_vao;
  static int                _nbInstances;
  static Vector2f           _cellSize;
  static const int          _nb = 16;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void TextDrawer::updateBuffers(const QString  &text,
				      const Vector2f &pos,
				      const Vector2f &size) {
  
  const unsigned int s = (unsigned int)text.size();
  const unsigned int n = s*6;

  std::vector<Vector2f> vertices(n);
  std::vector<Vector2f> uvcoords(n);
  unsigned int j=0;

  float stepx = size[0]/(float)s;
  
  for(unsigned int i=0;i<s;++i) {
    const Vector2f p = pos+Vector2f(i*stepx,0);
    const char character = text[i].unicode();
    const float uvx = (character%16)/16.0f;
    const float uvy = (character/16)/16.0f;
    
    vertices[j  ] = p;
    vertices[j+1] = p+Vector2f(stepx,0);
    vertices[j+2] = p+Vector2f(stepx,size[1]);
    vertices[j+3] = vertices[j+2];
    vertices[j+4] = p+Vector2f(0,size[1]);
    vertices[j+5] = p; 
    
    uvcoords[j  ] = Vector2f(uvx,1.0f-(uvy+1.0f/16.0f));
    uvcoords[j+1] = Vector2f(uvx+1.0f/16.0f,1.0f-(uvy+1.0f/16.0f));
    uvcoords[j+2] = Vector2f(uvx+1.0f/16.0f,1.0f-uvy);
    uvcoords[j+3] = uvcoords[j+2];
    uvcoords[j+4] = Vector2f(uvx,1.0f-uvy);
    uvcoords[j+5] = uvcoords[j];
    
    j+=6;
  }

  _vao->setAttribUnsync(0,n*sizeof(Vector2f),vertices[0].data(),2,0,GL_STREAM_DRAW);
  _vao->setAttribUnsync(1,n*sizeof(Vector2f),uvcoords[0].data(),2,0,GL_STREAM_DRAW);
}

inline void TextDrawer::draw(const QString  &text,
			     const Vector2f &pos,
			     const Vector2f &size,
			     const Matrix4f &mvp,
			     const Vector4f &tcolor,
			     const Vector4f &bcolor) {
  if(text.isEmpty()) return;

  updateBuffers(text,pos,size);

  _vao->bind();
  _ptext->enable();

  _ptext->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _ptext->setUniform4fv("tcolor",(GLfloat *)tcolor.data());
  _ptext->setUniform4fv("bcolor",(GLfloat *)bcolor.data());
  _ptext->setUniformTexture("font",GL_TEXTURE_2D,_tfont->id()); 

  _vao->drawArrays(GL_TRIANGLES,0,6*text.size()); 

  _ptext->disable();  
  _vao->unbind();
}

#endif // TEXT_DRAWER
