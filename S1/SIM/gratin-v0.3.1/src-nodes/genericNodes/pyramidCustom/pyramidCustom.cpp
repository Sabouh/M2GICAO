// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "pyramidCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"

using namespace std;

PyramidCustomWidget::PyramidCustomWidget(PyramidCustomNode *node)
  : GenericCustomWidget(node),
    _modeBox(new QComboBox()),
    _update(true) {

  _modeBox->addItems(QStringList() << "Finest to coarsest" << "Coarsest to finest");
  QWidget *tmp = new QWidget();
  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(new QLabel("Mode"));
  l->addWidget(_modeBox);
  tmp->setLayout(l);
  userLayout()->addWidget(tmp);

  connect(_modeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(modeChanged(int)));
}

void PyramidCustomWidget::modeChanged(int) {
  if(_update)
    updateGraph();
}


PyramidCustomNode::PyramidCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(false,false,false,true,parent,handle) {

  _w = new PyramidCustomWidget(this);

  initShaderSource();
  _w->updateWidget();
}

PyramidCustomNode::~PyramidCustomNode() {
}

void PyramidCustomNode::apply() {
  if(nbOutputs()==0) return;
  setOutputParams();

  enableShaders();
  if(_w->mode()==0) 
    finestToCoarsest();
  else 
    coarsestToFinest();
  disableShaders();
}

void PyramidCustomNode::finestToCoarsest() {
  int  w    = (int)(outputTex(0)->w());
  int  h    = (int)(outputTex(0)->h());
  int  nbl  = 1 + (int)floor(log2(max((float)w,(float)h)));

  _fbo.bind();
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->bind();
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
    _fbo.attachTexture(GL_TEXTURE_2D,outputTex(i)->id(),*buffersOfOutputTex(i),0);
  }

  Glutils::setViewport(w,h);

  _p->enable();
  _p->setUniform1i("nbLevels",nbl);
  _p->setUniform1i("numLevel",0);
  _p->setUniform1i("isFinest",1);
  _p->setUniform1i("isCoarsest",0);
  drawOutputs(buffersOfOutputTex(0),nbOutputs(),false,false);
  _p->setUniform1i("isFinest",0);
  _p->disable();

  for(int n=1;n<nbl;++n) {
    _p->enable();
    
    for(unsigned int i=0;i<nbOutputs();++i) {
      outputTex(i)->bind();
      _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,n-1);
      _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL, n-1);
      QString level("prevLevel"+QString::number(i));
      
      _p->setUniformTexture(level,GL_TEXTURE_2D,outputTex(i)->id());
    
    }

    _p->setUniform1i("numLevel",n);
    _p->setUniform1i("isCoarsest",n==(nbl-1));
    _p->disable();
    
    w = max(1,w >> 1);
    h = max(1,h >> 1);
    
    for(unsigned int i=0;i<nbOutputs();++i) {
      outputTex(i)->bind();
      _fbo.attachTexture(GL_TEXTURE_2D,outputTex(i)->id(),*buffersOfOutputTex(i),n);
    }

    Glutils::setViewport(w,h);
    _p->enable();
    drawGenericOutputs();
    _p->disable();
  }
  
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->bind();
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,nbl-1);
  }
  FramebufferObject::unbind();
}

void PyramidCustomNode::coarsestToFinest() {
  int  w    = (int)(outputTex(0)->w());
  int  h    = (int)(outputTex(0)->h());
  int  nbl  = 1 + (int)floor(log2(max((float)w,(float)h)));

  _fbo.bind();
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->bind();
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, nbl-1);
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,nbl-1);
    _fbo.attachTexture(GL_TEXTURE_2D,outputTex(i)->id(),*buffersOfOutputTex(i),nbl-1);
  }

  Glutils::setViewport(1,1);

  _p->enable();
  _p->setUniform1i("nbLevels",nbl);
  _p->setUniform1i("numLevel",(nbl-1));
  _p->setUniform1i("isFinest",0);
  _p->setUniform1i("isCoarsest",1);
  drawOutputs(buffersOfOutputTex(0),nbOutputs(),false,false);
  _p->setUniform1i("isCoarsest",0);
  _p->disable();

  for(int n=(nbl-2);n>=0;--n) {
    _p->enable();
    
    for(unsigned int i=0;i<nbOutputs();++i) {
      outputTex(i)->bind();
      _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,n+1);
      _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL, n+1);
      QString level("prevLevel"+QString::number(i));
      
      _p->setUniformTexture(level,GL_TEXTURE_2D,outputTex(i)->id());
    
    }

    _p->setUniform1i("numLevel",n);
    _p->setUniform1i("isFinest",n==0);
    _p->disable();
    
    outputTex(0)->bind();
    _glf->glGetTexLevelParameteriv(GL_TEXTURE_2D,n,GL_TEXTURE_WIDTH,&w);
    _glf->glGetTexLevelParameteriv(GL_TEXTURE_2D,n,GL_TEXTURE_HEIGHT,&h);

    for(unsigned int i=0;i<nbOutputs();++i) {
      outputTex(i)->bind();
      _fbo.attachTexture(GL_TEXTURE_2D,outputTex(i)->id(),*buffersOfOutputTex(i),n);
    }

    Glutils::setViewport(w,h);
    _p->enable();
    drawGenericOutputs();
    _p->disable();
  }
  
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->bind();
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,nbl-1);
  }
  FramebufferObject::unbind();
}

void PyramidCustomNode::initFBO() {
  NodeTexture2D::initFBO();
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->bind();
    _glf->glGenerateMipmap(GL_TEXTURE_2D);
  }
  _glf->glBindTexture(GL_TEXTURE_2D,0);
}

const QString PyramidCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec2 vertex;\n"
		     "layout(location = 1) in vec2 coord;\n");
}

const QString PyramidCustomNode::defaultVertBody()   {
  return QObject::tr("out vec2 texcoord;\n\n"
		     "void main() {\n"   
		     "\tgl_Position = vec4(vertex,0,1);\n"
		     "\ttexcoord    = coord;\n"
		     "}\n");
}

const QString PyramidCustomNode::constantFragHead()  {
  return QString();
}

const QString PyramidCustomNode::defaultFragBody()   {
  return QObject::tr("in vec2 texcoord;\n"
		     "void main() {\n"
		     "\tif(isFinest)\n"
		     "\t\toutBuffer0 = texture(inBuffer0,texcoord);\n"
		     "\telse {\n"
		     "\t\tvec2 ps = .5*(1./vec2(textureSize(prevLevel0,0)));\n"
		     "\t\tvec4 c1 = texture(prevLevel0,texcoord+ps);\n"
		     "\t\tvec4 c2 = texture(prevLevel0,texcoord-ps);\n"
		     "\t\tvec4 c3 = texture(prevLevel0,texcoord+vec2(ps.x,-ps.y));\n"
		     "\t\tvec4 c4 = texture(prevLevel0,texcoord+vec2(-ps.x,ps.y));\n"
		     "\t\toutBuffer0 = 0.25*(c1+c2+c3+c4);\n"
		     "\t}\n"
		     "}\n");
}

const QString PyramidCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 4) out;\n");
}

const QString PyramidCustomNode::defaultTessCBody()  {
return QObject::tr("void main(void) {\n"
		   "\tgl_TessLevelOuter[0] = 1;\n"
		   "\tgl_TessLevelOuter[1] = 1;\n"
		   "\tgl_TessLevelOuter[2] = 1;\n"
		   "\tgl_TessLevelOuter[3] = 1;\n"
		   "\tgl_TessLevelInner[0] = 1;\n"
		   "\tgl_TessLevelInner[1] = 1;\n"
		   "\tgl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
		   "}\n");
}

const QString PyramidCustomNode::constantTessEHead() {
  return QString();
}

const QString PyramidCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(quads,equal_spacing) in;\n\n"
		     "void main() {\n"
		     "\tvec4 p1 = mix(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_TessCoord.x);\n"
		     "\tvec4 p2 = mix(gl_in[3].gl_Position,gl_in[2].gl_Position,gl_TessCoord.x);\n"
		     "\tgl_Position = mix(p1,p2,gl_TessCoord.y);\n"
		     "}\n");
}

const QString PyramidCustomNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString PyramidCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}");
}

bool PyramidCustomNode::save(SceneSaver  *saver) {
  bool ret = true;

  if(!saver->saveInt("processMode",_w->mode())) ret = false;
  if(!GenericCustomNode::save(saver))           ret = false;

  return ret;
}

bool PyramidCustomNode::load(SceneLoader *loader) {
  bool ret = true;

  int mode = 0;
  if(!loader->loadInt("processMode",mode)) ret = false;
  if(!GenericCustomNode::load(loader))          ret = false;
  
  _w->setMode(mode,false);

  return ret;
}
