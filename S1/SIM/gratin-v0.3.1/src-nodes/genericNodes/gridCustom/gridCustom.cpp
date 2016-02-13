// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "gridCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "misc/mesh.h"
#include "misc/objLoader.h"

using namespace std;

GridCustomWidget::GridCustomWidget(GridCustomNode *node)
  : GenericCustomWidget(node),
    _gridDepth(new IntSliderWidget(node,"grid depth",1,11,8)),
    _default(new QPushButton("Reset cam")) {

  _camWidget = new TrackballCameraWidget(node,"Camera",node->camera());

  // default parameters 
  userLayout()->addWidget(_gridDepth);
  userLayout()->addWidget(_camWidget);
  userLayout()->addWidget(_default);

  connect(_gridDepth,SIGNAL(valChanged()),this,SLOT(gridDepthChanged()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));

  addChildWidget(_camWidget);
  addChildWidget(_gridDepth);
}

void GridCustomWidget::gridDepthChanged() {
  ((GridCustomNode *)node())->initGrid();
  updateGraph();
}

void GridCustomWidget::defaultClicked() {
  ((GridCustomNode *)node())->initCam();
  updateGraph();
}


GridCustomNode::GridCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(true,false,false,true,parent,handle,true,false,Vector2f(0.0f,0.0f),Vector2f(512.0f,512.0f)),
    _camera(new TrackballCamera(Vector2i(512,512),Vector3f(0,0,0),1.0f,0)),
    _vao(NULL),
    _depth(NULL) {

  _w = new GridCustomWidget(this);

  initShaderSource();
  initGrid();

  _w->updateWidget();
}

GridCustomNode::~GridCustomNode() {
  delete _vao;
  delete _camera;
  delete _depth;
}

void GridCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  initOpenGLState();

  enableShaders();
  _p->setUniformMatrix4fv("model",(GLfloat *)_camera->modelMatrix().data());
  _p->setUniformMatrix4fv("view",(GLfloat *)_camera->viewMatrix().data());
  _p->setUniformMatrix4fv("proj",(GLfloat *)_camera->projMatrix().data());
  _p->setUniform1f("zmin",_camera->zmin());
  _p->setUniform1f("zmax",_camera->zmax());

  _vao->bind();

  if(useTesselation()) {
    _glf->glPatchParameteri(GL_PATCH_VERTICES,4);
    _vao->drawElements(GL_PATCHES,_nbElements);
  } else {
    _vao->drawElements(GL_TRIANGLES,_nbElements);
  }

  _vao->unbind();
  FramebufferObject::unbind();

  disableShaders();
  cleanOpenGLState();
}

void GridCustomNode::settingsChanged() {
  initGrid();
  GenericCustomNode::settingsChanged();
}


void GridCustomNode::initCam() {
  Vector2i size(outputSize()[0],outputSize()[1]);
  _camera->reinit(size,_camera->sceneCenter(),_camera->sceneRadius());
}

void GridCustomNode::initGrid() {
  const unsigned int samples = (1 << _w->gridDepth()->val());    
  const float size = 1.0;
  const float step = size/((float)samples-1);
  const Vector3f start = Vector3f(-0.5f,-0.5f,0.0f)*size;

  if(useTesselation()) {
    // quad mesh creation 
    vector<Vector3f> vertices;
    vector<Vector2f> uvs;
    vector<Vector4i> indices;
    
    for(unsigned int i=0;i<samples;++i) {
      for(unsigned int j=0;j<samples;++j) {
	const Vector3f current = start + Vector3f((float)j,(float)i,0.0f)*step;
	vertices.push_back(current);
	uvs.push_back(Vector2f((float)j,(float)i)/(float)samples);
	
	if(i>0 && j>0) {
	  unsigned int i1 = i*samples+j;
	  unsigned int i2 = (i-1)*samples+j;
	  unsigned int i3 = (i-1)*samples+j-1;
	  unsigned int i4 = i*samples+j-1;
	  
	  indices.push_back(Vector4i(i1,i2,i3,i4));
	}
      }
    }
    
    delete _vao;
    
    _vao = new VertexarrayObject();
    
    unsigned int nbVert = static_cast<unsigned int>(vertices.size());
    unsigned int nbInd  = static_cast<unsigned int>(indices.size());
    
    _vao->addAttrib(nbVert*sizeof(Vector3f),vertices[0].data(),3);
    _vao->addAttrib(nbVert*sizeof(Vector2f),uvs[0].data(),2);
    _vao->setIndices(nbInd*sizeof(Vector4i),indices[0].data());
    
    _nbElements = nbInd*4;
  } else {
    // triangle mesh creation 
    vector<Vector3f> vertices;
    vector<Vector2f> uvs;
    vector<Vector3i> indices;
    
    for(unsigned int i=0;i<samples;++i) {
      for(unsigned int j=0;j<samples;++j) {
	const Vector3f current = start + Vector3f((float)j,(float)i,0.0f)*step;
	vertices.push_back(current);
	uvs.push_back(Vector2f((float)j,(float)i)/(float)samples);
	
	if(i>0 && j>0) {
	  unsigned int i1 = i*samples+j;
	  unsigned int i2 = (i-1)*samples+j;
	  unsigned int i3 = (i-1)*samples+j-1;
	  unsigned int i4 = i*samples+j-1;
	  
	  indices.push_back(Vector3i(i1,i2,i3));
	  indices.push_back(Vector3i(i1,i3,i4));
	}
      }
    }
    
    delete _vao;
    
    _vao = new VertexarrayObject();
    
    unsigned int nbVert = static_cast<unsigned int>(vertices.size());
    unsigned int nbInd  = static_cast<unsigned int>(indices.size());
    
    _vao->addAttrib(nbVert*sizeof(Vector3f),vertices[0].data(),3);
    _vao->addAttrib(nbVert*sizeof(Vector2f),uvs[0].data(),2);
    _vao->setIndices(nbInd*sizeof(Vector3i),indices[0].data());
    
    _nbElements = nbInd*3;
  }
}

void GridCustomNode::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  GenericCustomNode::mousePressEvent(p,me);

  if(me->button()==Qt::LeftButton) {
    _camera->initRotation(p);
  } else if(me->button()==Qt::RightButton) {
    _camera->initMoveXY(p);
  } else if(me->button()==Qt::MidButton) {
    _camera->initMoveZ(p);
  }
  update();
}

void GridCustomNode::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  GenericCustomNode::mouseMoveEvent(p,me);

  _camera->move(p);
  update();
}

void GridCustomNode::wheelEvent(const Vector2f &p,QWheelEvent *we) {
  GenericCustomNode::wheelEvent(p,we);

  const Vector2f v(0.0f,60.0f);

  _camera->initMoveZ(p);
  if(we->delta()>0) {
    _camera->move(p+v);
  } else {
    _camera->move(p-v);
  }
  update();
}

void GridCustomNode::initFBO() {  
  NodeTexture2D::initFBO();

  _depth = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,outputSize()[0],outputSize()[1],
  					    GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT,GL_FLOAT),
  			      TextureParams(GL_NEAREST,GL_NEAREST));

  _fbo.bind();
  _fbo.attachTexture(GL_TEXTURE_2D,_depth->id(),GL_DEPTH_ATTACHMENT);
  _fbo.isValid();

  FramebufferObject::unbind();
  setOutputParams();
}

void GridCustomNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _depth; _depth = NULL;
}

const QString GridCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec3 inVertex;\n"
		     "layout(location = 1) in vec2 inTexcoord;\n");
}

const QString GridCustomNode::defaultVertBody()   {
  return QObject::tr("out vec3  normalV;\n"
		     "out float depthV;\n\n"
		     "void main() {\n"
		     "\tmat4 mdv    = view*model;\n"
		     "\tmat4 mvp    = proj*mdv;\n\n"
		     "\tnormalV     = (mdv*vec4(0,0,1,0)).xyz;\n"
		     "\tdepthV      = (clamp(-(mdv*vec4(inVertex,1)).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
		     "\tgl_Position = mvp*vec4(inVertex,1);\n"
		     "}\n");
}

const QString GridCustomNode::constantFragHead()  {
  return QString();
}

const QString GridCustomNode::defaultFragBody()   {
  return QObject::tr("in vec3  normalV;\n"
		     "in float depthV;\n\n"
		     "void main() {\n"
		     "\toutBuffer0 = vec4(normalize(normalV),depthV);\n"
		     "}\n");
}

const QString GridCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 4) out;\n");
}

const QString GridCustomNode::defaultTessCBody()  {
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

const QString GridCustomNode::constantTessEHead() {
  return QString();
}

const QString GridCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(quads,equal_spacing) in;\n\n"
		     "void main() {\n"
		     "\tvec4 p1 = mix(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_TessCoord.x);\n"
		     "\tvec4 p2 = mix(gl_in[3].gl_Position,gl_in[2].gl_Position,gl_TessCoord.x);\n"
		     "\tgl_Position = mix(p1,p2,gl_TessCoord.y);\n"
		     "}\n");
}

const QString GridCustomNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString GridCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}");
}

bool GridCustomNode::save(SceneSaver  *saver) {
  bool ret = true;

  // obj file and camera 
  if(!saver->saveIOData("camera",_camera))     ret = false;
  if(!GenericCustomNode::save(saver))          ret = false;

  return ret;
}

bool GridCustomNode::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadIOData("camera",_camera))     ret = false;
  initGrid();
  if(!GenericCustomNode::load(loader))          ret = false;
  return ret;
}
