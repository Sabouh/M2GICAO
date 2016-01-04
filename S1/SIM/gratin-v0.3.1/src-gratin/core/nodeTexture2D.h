// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_TEXTURE2D_H
#define NODE_TEXTURE2D_H

#include "core/nodeInterface.h"
#include "misc/framebufferObject.h"
#include "misc/texture2D.h"
#include "misc/glutils.h"
#include "core/textureIO.h"
#include <Eigen/Dense>

using namespace Eigen;

// This class implements a texture interface in which
// each input/output must be a texture2D
// a simple fbo is prepared to render the data, as well as a
// specific nodeData class for drawing textures


class LIBRARY_EXPORT TextureData : public Data {
 public:
 TextureData(const Vector2f &p=Vector2f(0,0),const Vector2f &s=Vector2f(0,0))
   : Data(TEX2D,p,s), _tex(NULL) {}
  ~TextureData() {}

  inline FloatTexture2D *tex() const {return _tex;}
  
  inline virtual bool ready() const {return _tex;}
  inline virtual bool sameAs(Data *d) const {return _type==d->type() && ((TextureData *)d)->tex()==tex();}
  inline virtual void copy(Data *d) {_pos = d->pos();_size = d->size();_tex = ((TextureData *)d)->tex();}

  void createTex(const Vector2f &size,GLenum filter=Glutils::defFilter(),GLint intFormat=Glutils::defFormat(),GLint format=GL_RGBA);
  void deleteTex();

 private:
  FloatTexture2D *_tex;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class LIBRARY_EXPORT NodeTexture2D : public NodeInterface {
 public:
  NodeTexture2D(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  virtual ~NodeTexture2D();

  // specify nb input/output needed
  //inline virtual unsigned int nbInputs()  const {return 1;}
  //inline virtual unsigned int nbOutputs() const {return 1;}

  // processing functions (called only if isReady() returns true)
  inline virtual void apply () {}   // process the input(s)
  inline virtual void reload() {}   // reload data/shaders
  inline virtual void init  () {}   // init data (at connection)
  inline virtual void clean () {}   // clean data (at deconnection)

  // return a widget if you want your node to be controled by user parameters
  inline virtual NodeWidget *widget() {return NULL;}

  // interaction events (only if isReady() is true)
  inline virtual void keyPressEvent    (QKeyEvent *          ) {}
  inline virtual void mousePressEvent  (const Vector2f &,QMouseEvent *) {}
  inline virtual void mouseMoveEvent   (const Vector2f &,QMouseEvent *) {}
  inline virtual void mouseReleaseEvent(const Vector2f &,QMouseEvent *) {}
  inline virtual void wheelEvent       (const Vector2f &,QWheelEvent *) {}

  // control output size 
  inline virtual bool outputSizeEqualInputSize() const {return true;}

  // constant output size functions (used only if outputSizeEqualInputSize is false)
  inline const Vector2f &outputSize() const {return _outputSize;}
  inline void  setOutputSize(const Vector2f &size);

  // received if isReady() is true and if a special change has been made (like the content size has changed, etc)
  inline virtual void receiveEvent(const NodeEvent &event);

 protected:
  // nb tmp textures needed
  inline virtual unsigned int nbTmps() const {return 0;}

  // access functions for output buffers and node data 
  inline GLenum  bufferOfOutputTex (int i) const {return _buffers[i];}
  inline GLenum  bufferOfTmpTex    (int i) const {return _buffers[nbOutputs()+i];}
  inline GLenum *buffersOfOutputTex(int i) const {return &_buffers[i];}
  inline GLenum *buffersOfTmpTex   (int i) const {return &_buffers[nbOutputs()+i];}
  inline Data   *tmp(unsigned int i=0)     const {return _tmpData[i];}

  // convenient functions for accessing data textures 
  inline FloatTexture2D *inputTex (unsigned int i=0) {return input(i)  ? ((TextureData *)input(i))->tex()  : NULL;}
  inline FloatTexture2D *outputTex(unsigned int i=0) {return output(i) ? ((TextureData *)output(i))->tex() : NULL;}
  inline FloatTexture2D *tmpTex   (unsigned int i=0) {return tmp(i)    ? ((TextureData *)tmp(i))->tex()    : NULL;}

  // use this function to draw a quad with the same size of texture0 at the given buffer pos in the fbo
  inline void drawOutputs(GLenum *buff,int nbOutputs=1,bool needBinding=true,bool needUnbinding=true);

 protected:
  // the data
  std::vector<Data *> _tmpData;
  FramebufferObject   _fbo;
  GLenum             *_buffers;
  Vector2f            _outputSize; 
  //Vector2f            _offset;

  // initializing / cleaning FBO
  virtual void initFBO();
  virtual void cleanFBO();
  /* virtual void initVertices(); */
  /* virtual void cleanVertices(); */

  /* std::vector<Vector2f> _boxesPts; */
  /* std::vector<Vector2f> _boxesCos; */

  // only used to draw the node and to remap coordinates
  //inline const Vector2f &offset() const {return _offset;}

public:
  // specific functions for the texture node

  // init all the output data and set the _ready variable (be careful that inputReady is equal to true)
  virtual void initOutputData();

  // clean data and set the _ready variable
  virtual void cleanOutputData();
  
  // create input/output arrays
  virtual void initEmptyData(const Vector2f &pos=Vector2f(0,0));
  
  // clean inputs and outputs 
  virtual void cleanData();

  // clean data + widget
  virtual void cleanEverything();

  // dedicated function for drawing the node+output data when rendering (OpenGL)
  //inline virtual void draw(const Matrix4f &mvp=Matrix4f::Identity());
  inline virtual void drawOutputContent(const Matrix4f &mvp=Matrix4f::Identity(),
					const Vector2f &position=Vector2f(0,0),unsigned int i=0);

  inline virtual const Vector2f outputContentSize(unsigned int i);
 
  // return the full path+filename in which the output i will be stored, or empty if no file 
  virtual const QString prepareToExportOutputContent(unsigned int i=0);

  // save the outut i at the specified filename
  virtual bool exportOutputContent(const QString &filename,unsigned int i);

 protected:
  GPUProgram  _pcontent;
  TextureIO   _textureIO;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void NodeTexture2D::drawOutputs(GLenum *buff,int nbOutputs,bool needBinding,bool needUnbinding) {
  if(needBinding) {
    _fbo.bind();
  }

  _glf->glDrawBuffers(nbOutputs,buff);
  _unitSquareVao->bind();
  _unitSquareVao->drawArrays(GL_TRIANGLES,0,6);
  _unitSquareVao->unbind();

  if(needUnbinding) {
    FramebufferObject::unbind();
  }  
}


inline void NodeTexture2D::drawOutputContent(const Matrix4f &mvp,const Vector2f &position,unsigned int i) {  
  FloatTexture2D *t = ((TextureData *)output(i))->tex();

  float *pts = (float *)_rectangleVao->mapAttribUnsync(0);
    
  pts = copyToBuffer(position,pts); 
  pts = copyToBuffer(position+Vector2f(t->w(),0),pts); 
  pts = copyToBuffer(position+Vector2f(t->w(),t->h()),pts); 
  pts = copyToBuffer(position+Vector2f(t->w(),t->h()),pts); 
  pts = copyToBuffer(position+Vector2f(0,t->h()),pts); 
  pts = copyToBuffer(position,pts); 

  _rectangleVao->unmapAttrib();

  _rectangleVao->bind();
  _pcontent.enable();
  _pcontent.setUniformTexture("img",GL_TEXTURE_2D,t->id());
  _pcontent.setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _rectangleVao->drawArrays(GL_TRIANGLES,0,6);
  _pcontent.disable();
  _rectangleVao->unbind();
}

inline void NodeTexture2D::receiveEvent(const NodeEvent &event) {
  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    reloadInputData();
    initOutputData();
  } 

  NodeInterface::receiveEvent(event);
}

inline void NodeTexture2D::setOutputSize(const Vector2f &size) {
  _outputSize = size; 
  sendEvent(NodeEvent(NodeEvent::OUTPUT_SIZE_HAS_CHANGED));
}

inline const Vector2f NodeTexture2D::outputContentSize(unsigned int i) {
  FloatTexture2D *t;
  if(!isReady() || !(t=((TextureData *)output(i))->tex())) 
    return NodeInterface::outputContentSize(i);
  
  return Vector2f(t->w(),t->h());
}

#endif // NODE_TEXTURE2D_H
