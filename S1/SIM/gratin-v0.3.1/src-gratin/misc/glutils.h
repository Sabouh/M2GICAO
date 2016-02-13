// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GLUTILS_H
#define GLUTILS_H

#include "extinclude.h"
#include "texture1D.h"
#include "texture2D.h"
#include "texture3D.h"
#include <QImage>
#include <QString>
#include <QColor>
#include <iostream>

class LIBRARY_EXPORT Glutils : public GlContext {
 public:
  inline static GLenum defFormat() {return GL_RGBA32F;}
  inline static GLenum defFilter()   {return GL_LINEAR;}

  inline static void setViewport(GLsizei w,GLsizei h) {_glf->glViewport(0,0,w,h);}

  inline static const Matrix4f ortho(float l,float r,float b,float t,float n=-1.0f,float f=1.0f) {
    Matrix4f o;
    const float rml = r-l;
    const float tmb = t-b;
    const float fmn = f-n;
    o << 2.0f/rml,0,0,-(r+l)/rml,0,2.0f/tmb,0,-(t+b)/tmb,0,0,-2.0f/fmn,-(f+n)/fmn,0,0,0,1;
    return o;
  }

  inline static const Matrix4f squareMode(unsigned int w,unsigned int h) {
    setViewport(w,h); return ortho(0,w-1,h-1,0,0.0,-1.0);
  }

  inline static const Matrix4f screenMode(unsigned int w,unsigned int h,const Vector2f &offset,float scale) {
    setViewport(w,h);

    const float w2 = scale*(w/2.0f);
    const float h2 = scale*(h/2.0f);

    return ortho(-w2+offset[0],w2+offset[0],-h2+offset[1],h2+offset[1],0.0,-1.0);
  }

  static inline FloatTexture2D *qimageToTexture(const QString &filename,bool mipmap=false) {
    QImage image(filename);
    if(image.isNull()) {
      return NULL;
    }

    unsigned char *data = new unsigned char[image.width()*image.height()*4];

    unsigned int ind = 0;
    for(int i=image.height()-1;i>=0;--i) {
      for(int j=0;j<image.width();++j) {
	QRgb p = image.pixel(j,i);
	data[ind] = (char)qRed(p); ind++;
	data[ind] = (char)qGreen(p); ind++;
	data[ind] = (char)qBlue(p); ind++;
	data[ind] = (char)qAlpha(p); ind++;
      }
    }
    
    FloatTexture2D *tex;
    tex = mipmap ? 
      new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,image.width(),image.height(),
				       defFormat(),GL_RGBA,GL_UNSIGNED_BYTE,0,TextureFormat::MIPMAP_FBO_AUTOM),
			 TextureParams(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR),(float *)data) :
      new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,image.width(),image.height(),
				       defFormat(),GL_RGBA,GL_UNSIGNED_BYTE),
			 TextureParams(GL_LINEAR,GL_LINEAR),(float *)data);

    delete [] data;

    return tex;
  }
};

#endif // GLUTILS_H
