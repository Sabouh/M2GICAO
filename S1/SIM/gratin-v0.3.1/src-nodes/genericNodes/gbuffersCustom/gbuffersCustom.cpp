// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "gbuffersCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <iostream>

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "misc/mesh.h"
#include "misc/objLoader.h"

using namespace std;

QDir GbuffersCustomWidget::_currentPath = QDir::currentPath();

GbuffersCustomWidget::GbuffersCustomWidget(GbuffersCustomNode *node)
  : GenericCustomWidget(node),
    _load(new QPushButton("Load...")),
    _default(new QPushButton("Reset cam")) {

  _camWidget = new TrackballCameraWidget(node,"Camera",node->camera());

  // default parameters 
  userLayout()->addWidget(_load);
  userLayout()->addWidget(_camWidget);
  userLayout()->addWidget(_default);

  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));

  addChildWidget(_camWidget);
}

void GbuffersCustomWidget::loadClicked() {
  QString filename = QFileDialog::getOpenFileName(0,"Load object",_currentPath.absolutePath(),"Objects (*.obj);;All (*.*)");

  if(filename.isEmpty()) {
    return;
  }

  QDir d(".");
  _currentPath = d.filePath(filename);

  ((GbuffersCustomNode *)node())->loadObject(filename);
  updateGraph();
}

void GbuffersCustomWidget::defaultClicked() {
  ((GbuffersCustomNode *)node())->initCam();
  updateGraph();
}


GbuffersCustomNode::GbuffersCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(true,false,false,true,parent,handle,true,false,Vector2f(0.0f,0.0f),Vector2f(512.0f,512.0f)),
    _filename(GRATIN_APPLI_DATA_PATH+"/objs/sphere.obj"),
    _camera(new TrackballCamera(Vector2i(512,512),Vector3f(0,0,0),1.0f,0)),
    _vao(NULL),
    _depth(NULL) {

  _w = new GbuffersCustomWidget(this);

  initShaderSource();
  loadObject(_filename);

  _w->updateWidget();
}

GbuffersCustomNode::~GbuffersCustomNode() {
  delete _vao;
  delete _camera;
  delete _depth;
}

void GbuffersCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  setOutputParams();

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
    _glf->glPatchParameteri(GL_PATCH_VERTICES,3);
    _vao->drawElements(GL_PATCHES,_nbElements);
  } else {
    _vao->drawElements(GL_TRIANGLES,_nbElements);
  }

  _vao->unbind();
  FramebufferObject::unbind();

  disableShaders();
  cleanOpenGLState();
}

void GbuffersCustomNode::loadObject(const QString &filename) {
  QString f = filename;
  QDir d(".");
  f = d.absoluteFilePath(f);

  Mesh *mesh = ObjLoader::load(f.toStdString());

  if(!mesh || mesh->nbVertices()==0 || mesh->nbFaces()==0) return;

  _filename = f;

  // init camera 
  _camera->setSceneParams(mesh->barycenter(),mesh->radius());

  // init VAO
  makeCurrent();
  delete _vao;
  _vao = new VertexarrayObject();
  _vao->addAttrib(mesh->nbVertices()*sizeof(Vector3f),mesh->verticesPtr(),3);
  _vao->addAttrib(mesh->nbVertices()*sizeof(Vector3f),mesh->normalsPtr(),3);
  _vao->addAttrib(mesh->nbVertices()*sizeof(Vector3f),mesh->tangentsPtr(),3); 
  _vao->addAttrib(mesh->nbVertices()*sizeof(Vector2f),mesh->uvcoordsPtr(),2);
  _vao->setIndices(mesh->nbFaces()*sizeof(Vector3i),mesh->facesPtr());
  _nbElements = mesh->nbFaces()*3;
  delete mesh;
}

void GbuffersCustomNode::initCam() {
  Vector2i size(outputSize()[0],outputSize()[1]);
  _camera->reinit(size,_camera->sceneCenter(),_camera->sceneRadius());
}

void GbuffersCustomNode::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  if(me->button()==Qt::LeftButton) {
    _camera->initRotation(p);
  } else if(me->button()==Qt::RightButton) {
    _camera->initMoveXY(p);
  } else if(me->button()==Qt::MidButton) {
    _camera->initMoveZ(p);
  }

  GenericCustomNode::mousePressEvent(p,me);
}

void GbuffersCustomNode::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  _camera->move(p);
  GenericCustomNode::mouseMoveEvent(p,me);
}

void GbuffersCustomNode::wheelEvent(const Vector2f &p,QWheelEvent *we) {
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

void GbuffersCustomNode::initFBO() {  
  NodeTexture2D::initFBO();

  _depth = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,outputSize()[0],outputSize()[1],
  					    GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT,GL_FLOAT),
  			      TextureParams(GL_NEAREST,GL_NEAREST));

  _fbo.bind();
  _fbo.attachTexture(GL_TEXTURE_2D,_depth->id(),GL_DEPTH_ATTACHMENT);
  _fbo.isValid();

  FramebufferObject::unbind();
}

void GbuffersCustomNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _depth; _depth = NULL;
}

const QString GbuffersCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec3 inVertex;\n"
		     "layout(location = 1) in vec3 inNormal;\n"
		     "layout(location = 2) in vec3 inTangent;\n"
		     "layout(location = 3) in vec2 inTexcoord;\n");
}

const QString GbuffersCustomNode::defaultVertBody()   {
  return QObject::tr("out vec3  normalV;\n"
		     "out float depthV;\n\n"
		     "void main() {\n"
		     "\tmat4 mdv    = view*model;\n"
		     "\tmat4 mvp    = proj*mdv;\n\n"
		     "\tnormalV     = (mdv*vec4(inNormal,0)).xyz;\n"
		     "\tdepthV      = (clamp(-(mdv*vec4(inVertex,1)).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
		     "\tgl_Position = mvp*vec4(inVertex,1);\n"
		     "}\n");
}

const QString GbuffersCustomNode::constantFragHead()  {
  return QString();
}

const QString GbuffersCustomNode::defaultFragBody()   {
  return QObject::tr("in vec3  normalV;\n"
		     "in float depthV;\n\n"
		     "void main() {\n"
		     "\toutBuffer0 = vec4(normalize(normalV),depthV);\n"
		     "}\n");
}

const QString GbuffersCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 3) out;\n");
}

const QString GbuffersCustomNode::defaultTessCBody()  {
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

const QString GbuffersCustomNode::constantTessEHead() {
  return QString();
}

const QString GbuffersCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(triangles,equal_spacing,ccw) in;\n\n"
  		     "void main() {\n"
  		     "\tvec4 p1 = gl_TessCoord.x*gl_in[0].gl_Position;\n"
  		     "\tvec4 p2 = gl_TessCoord.y*gl_in[1].gl_Position;\n"
  		     "\tvec4 p3 = gl_TessCoord.z*gl_in[2].gl_Position;\n"
  		     "\tgl_Position = p1+p2+p3;\n"
  		     "}\n");
}

const QString GbuffersCustomNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString GbuffersCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}\n");
}

bool GbuffersCustomNode::save(SceneSaver  *saver) {
  bool ret = true;

  // obj file and camera 
  QString name = saver->absoluteToRelativePath(_filename);
  if(!saver->saveString("filename",name))      ret = false;
  if(!saver->saveIOData("camera",_camera))     ret = false;
  if(!GenericCustomNode::save(saver))          ret = false;

  return ret;
}

bool GbuffersCustomNode::load(SceneLoader *loader) {
  bool ret = true;

  _filename = GRATIN_APPLI_DATA_PATH+"/objs/sphere.obj";
  QString name;
  if(!loader->loadString("filename",name)) ret = false;
  
  if(ret) {
    _filename = loader->relativeToAbsolutePath(name);
  }
  
  if(!loader->loadIOData("camera",_camera))     ret = false;
  loadObject(_filename);  
  if(!GenericCustomNode::load(loader))          ret = false;

  return ret;
}
