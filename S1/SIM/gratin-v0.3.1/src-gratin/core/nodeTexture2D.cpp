// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/nodeTexture2D.h"
#include "core/pbgraph.h"
#include "misc/texture2D.h"

#include <QDebug>

using namespace std;

void TextureData::createTex(const Vector2f &size,GLenum filter,GLint intFormat,GLint format) {
  deleteTex();
  _tex = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,size[0],size[1],intFormat,format,GL_FLOAT),
			    TextureParams(filter,filter));
}

void TextureData::deleteTex() {
  delete _tex; _tex = NULL;
}

NodeTexture2D::NodeTexture2D(PbGraph *parent,NodeHandle *handle)
  : NodeInterface(parent,handle),
    _outputSize(Vector2f(512,512)),
  _pcontent(":/node-tex-content.vert",":/node-tex-content.frag"),
  _textureIO(TextureIO()) {

  _buffers = new GLenum[FramebufferObject::getMaxColorAttachments()];
  for(int i=0;i<FramebufferObject::getMaxColorAttachments();++i) {
    _buffers[i] = GL_COLOR_ATTACHMENT0+i;
  }

  _pcontent.addUniform("mvp");
  _pcontent.addUniform("img");
}

NodeTexture2D::~NodeTexture2D() {

}

void NodeTexture2D::initEmptyData(const Vector2f &pos) {
  cleanData();

  _pos = pos;
  
  _inputData  = vector<Data *>(nbInputs());
  _outputData = vector<Data *>(nbOutputs());
  _tmpData    = vector<Data *>(nbTmps());


  for(unsigned int i=0;i<nbInputs();++i) {
    _inputData[i] = new TextureData();
  }

  for(unsigned int i=0;i<nbOutputs();++i) {
    _outputData[i] = new TextureData();
  }

  for(unsigned int i=0;i<nbTmps();++i) {
    _tmpData[i] = new TextureData();
  }

  initDefaultPositions();
}

void NodeTexture2D::cleanData() {

  for(unsigned int i=0;i<_outputData.size();++i) {
    ((TextureData *)_outputData[i])->deleteTex();
  }

  for(unsigned int i=0;i<_tmpData.size();++i) {
    ((TextureData *)_tmpData[i])->deleteTex();
    delete _tmpData[i];
    _tmpData[i] = NULL;
  }

  _tmpData.clear();

  NodeInterface::cleanData();
}

void NodeTexture2D::cleanEverything() {
  cleanOutputData();
  delete _buffers;
  NodeInterface::cleanEverything();
}


void NodeTexture2D::initOutputData()   {
  if(!checkInputData(inputs())) {
    qDebug() << "Warning: trying to init output data while input not ready!" << endl;
    return;
  }

  // clean everything 
  cleanOutputData();

  const Vector2f size = (_inputData.empty() || 
			 input(0)->type()!=Data::TEX2D || 
			 !outputSizeEqualInputSize()) ? 
    outputSize() : Vector2f(((TextureData *)input(0))->tex()->w(),
			    ((TextureData *)input(0))->tex()->h());

  //_offset = size/10.0f;

  for(unsigned int i=0;i<_outputData.size();++i) {
    TextureData *data = (TextureData *)output(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  for(unsigned int i=0;i<_tmpData.size();++i) {
    TextureData *data = (TextureData *)tmp(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  // create tmp textures and FBO
  initFBO();
  //initVertices();
  _ready = true;

  updatePositions();

  initDrawingBuffers();
}

void NodeTexture2D::cleanOutputData()  {
  
  for(unsigned int i=0;i<_outputData.size();++i) {
    TextureData *data = (TextureData *)output(i);
    data->deleteTex();
  }

  for(unsigned int i=0;i<_tmpData.size();++i) {
    TextureData *data = (TextureData *)tmp(i);
    data->deleteTex();
  }

  cleanFBO();
  //cleanVertices();

  _ready = false;
}

// void NodeTexture2D::initVertices() {
//   unsigned int k=0,l=0;
//   _boxesPts = vector<Vector2f>(nbOutputs()*6);
//   _boxesCos = vector<Vector2f>(nbOutputs()*6);

//   for(unsigned int i=0;i<nbOutputs();++i) {
//     _boxesPts[k++]  = Vector2f(-1,-1);
//     _boxesPts[k++]  = Vector2f(1,-1);
//     _boxesPts[k++]  = Vector2f(1,1);
//     _boxesPts[k++]  = Vector2f(1,1);
//     _boxesPts[k++]  = Vector2f(-1,1);
//     _boxesPts[k++]  = Vector2f(-1,-1);

//     _boxesCos[l++]  = Vector2f(0.0f,0.0f);
//     _boxesCos[l++]  = Vector2f(1.0f,0.0f);
//     _boxesCos[l++]  = Vector2f(1.0f,1.0f);
//     _boxesCos[l++]  = Vector2f(1.0f,1.0f);
//     _boxesCos[l++]  = Vector2f(0.0f,1.0f);
//     _boxesCos[l++]  = Vector2f(0.0f,0.0f);
//   }
// }

// void NodeTexture2D::cleanVertices() {
//   _boxesPts.clear();
//   _boxesCos.clear();
// }

void NodeTexture2D::initFBO() {
  _fbo.bind();
  
  for(unsigned int i=0;i<nbOutputs();++i) {
    TextureData *data = (TextureData *)output(i);
    data->tex()->bind();
    _fbo.attachTexture(data->tex()->format().target(),data->tex()->id(),GL_COLOR_ATTACHMENT0+i);
  }
  
  for(unsigned int i=nbOutputs();i<nbTmps()+nbOutputs();++i) {
    TextureData *data = (TextureData *)tmp(i-nbOutputs());
    data->tex()->bind();
    _fbo.attachTexture(data->tex()->format().target(),data->tex()->id(),GL_COLOR_ATTACHMENT0+i);
  }
      
  _fbo.isValid();
  FramebufferObject::unbind();
}

void NodeTexture2D::cleanFBO() {
  _fbo.bind();
  _fbo.unattachAll();
  FramebufferObject::unbind();
}

const QString NodeTexture2D::prepareToExportOutputContent(unsigned int) {
  return _textureIO.askForSaveParams();
}

bool NodeTexture2D::exportOutputContent(const QString &filename,unsigned int i) {
  assert(i<nbOutputs());
  FloatTexture2D *tex = outputTex(i);
  return _textureIO.save(filename,tex);
}
