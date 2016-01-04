// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgSpritesCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"

using namespace std;

ImgSpritesCustomWidget::ImgSpritesCustomWidget(ImgSpritesCustomNode *node)
  : GenericCustomWidget(node) {

}

ImgSpritesCustomNode::ImgSpritesCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(true,false,false,true,parent,handle,false,true),
    _vao(NULL),
    _nbElements(0),
    _sw(1),
    _sh(1) {

  _w = new ImgSpritesCustomWidget(this);

  initShaderSource();
  initSprites();

  _w->updateWidget();
}

ImgSpritesCustomNode::~ImgSpritesCustomNode() {
  delete _vao;
}

void ImgSpritesCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  setOutputParams();

  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  initOpenGLState();
  enableShaders();

  _vao->bind();
  if(useTesselation()) {
    _glf->glPatchParameteri(GL_PATCH_VERTICES,1);
    _vao->drawArrays(GL_PATCHES,0,_nbElements);
  } else {
    _vao->drawArrays(GL_POINTS,0,_nbElements);
  }
  _vao->unbind();

  FramebufferObject::unbind();
  disableShaders();
  cleanOpenGLState();
}

void ImgSpritesCustomNode::initSprites() {
  vector<Vector2f> vertices;
  unsigned int w = _sw;
  unsigned int h = _sh;
  
  for(unsigned int i=0;i<h;++i) {
    for(unsigned int j=0;j<w;++j) {
      const float x = 2.0f*(((float)j+0.5f)/(float)w)-1.0f;
      const float y = 2.0f*(((float)i+0.5f)/(float)h)-1.0f;
      
      vertices.push_back(Vector2f(x,y));
    }
  }

  delete _vao;  
  _vao = new VertexarrayObject();
    
  unsigned int nbVert = static_cast<unsigned int>(vertices.size());

  _vao->addAttrib(nbVert*sizeof(Vector2f),vertices[0].data(),2);
  _nbElements = nbVert; 
}

void ImgSpritesCustomNode::initFBO() {
  NodeTexture2D::initFBO();

  if(nbOutputs()>0) {
    _sw = outputTex(0)->w();
    _sh = outputTex(0)->h();
  } else {
    _sw = _sh = 1;
  }
  initSprites();
}

void ImgSpritesCustomNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _vao; _vao = NULL;
}

const QString ImgSpritesCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec2 vertex;\n");
}

const QString ImgSpritesCustomNode::defaultVertBody()   {
  return QObject::tr("out vec4 color;\n\n"   
		     "void main() {\n"   
		     "\tgl_Position  = vec4(vertex,0,1);\n"
		     "\tgl_PointSize = 1.0;\n"
		     "\tcolor = texture(inBuffer0,vertex*0.5+vec2(0.5));\n"
		     "}\n");
}

const QString ImgSpritesCustomNode::constantFragHead()  {
  return QString();
}

const QString ImgSpritesCustomNode::defaultFragBody()   {
  return QObject::tr("in vec4 color;\n\n"   
		     "void main() {\n"
		     "\toutBuffer0 = color;\n"
		     "}\n");
}

const QString ImgSpritesCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 1) out;\n");
}

const QString ImgSpritesCustomNode::defaultTessCBody()  {
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

const QString ImgSpritesCustomNode::constantTessEHead() {
  return QString();
}

const QString ImgSpritesCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(quads,equal_spacing) in;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position;\n"
		     "}\n");
}

const QString ImgSpritesCustomNode::constantGeomHead()  {
  return QObject::tr("layout(points) in;\n");
}

const QString ImgSpritesCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(points, max_vertices = 1) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position;\n"
		     "\tEmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}");
}
