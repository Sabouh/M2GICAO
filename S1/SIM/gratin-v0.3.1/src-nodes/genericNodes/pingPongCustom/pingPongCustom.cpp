// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "pingPongCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QHBoxLayout>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"

using namespace std;

PingPongCustomWidget::PingPongCustomWidget(PingPongCustomNode *node)
  : GenericCustomWidget(node) {

  _init              = new QPushButton("Init Ping-Pong");
  _applyPingPong     = new QPushButton("Apply Ping-Pong");
  _pingPongNumber    = new QSpinBox();
  _reinitWhenChanged = new QCheckBox("Re-init when a change occurs");
  _updateOnceChanged = new QCheckBox("Apply only once when a change occurs");
  _updateAllChanged  = new QCheckBox("Apply all passes when a change occurs");
  _refreshAllFrames  = new QCheckBox("Refresh in-between passes?");
  _refreshAllFrames->hide(); // temporary? always yes for now to avoid GPU timeouts

  _pingPongNumber->setRange(0,9999);
  _pingPongNumber->setPrefix("Nb passes: ");
  _pingPongNumber->setSuffix(" (total=0)");
  _pingPongNumber->setValue(1);

  setReinitWhenChanged(true);
  setUpdateOnceChanged(false);
  setUpdateAllChanged(false);
  setRefreshAllFrames(true);

  userLayout()->addWidget(_init);
  userLayout()->addWidget(_applyPingPong);
  userLayout()->addWidget(_pingPongNumber);
  userLayout()->addWidget(_reinitWhenChanged);
  userLayout()->addWidget(_updateOnceChanged);
  userLayout()->addWidget(_updateAllChanged);
  userLayout()->addWidget(_refreshAllFrames);
  userLayout()->addStretch(1);

  connect(_init,SIGNAL(clicked()),this,SLOT(initClicked()));
  connect(_applyPingPong,SIGNAL(clicked()),this,SLOT(applyPingPongClicked()));
}

void PingPongCustomWidget::initClicked() {
  PingPongCustomNode *n = (PingPongCustomNode *)node();
  n->setMode(PingPongCustomNode::INIT_MODE);
  n->initPingPong(true);
  n->setMode(PingPongCustomNode::NORMAL_MODE);
}

void PingPongCustomWidget::applyPingPongClicked() {
  PingPongCustomNode *n = (PingPongCustomNode *)node();
  n->setMode(PingPongCustomNode::PP_MODE);
  n->applyPingPong(refreshAllFrames(),true);
  n->setMode(PingPongCustomNode::NORMAL_MODE);
}

PingPongCustomNode::PingPongCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(false,false,false,true,parent,handle),
    _mode(NORMAL_MODE),
    _currentPingPong(0),
    _copyBuffers(NULL),
    _initialized(false) {

  _pingPongFbo[0] = NULL;
  _pingPongFbo[1] = NULL;
  _w = new PingPongCustomWidget(this);

  initShaderSource();

  generateUniforms();
  initHeads();
  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;
  initProg();

  _w->updateWidget();
}

void PingPongCustomNode::settingsChanged() {
  GenericCustomNode::settingsChanged();
  generateUniforms();
  initHeads();
  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;
  initProg();
  _w->updateWidget();
}

PingPongCustomNode::~PingPongCustomNode() {
}

void PingPongCustomNode::init() {
  GenericCustomNode::init();
  initPingPong(true);
  _initialized = false;
}

void PingPongCustomNode::apply() {
  if(!_initialized && isReady()) {
    _initialized = true;
    _mode = PP_MODE;
    initPingPong(false);
    applyPingPong(false,false);
    _w->setPingPongNumberLabel(_currentPingPong);
    //copyResult(1-_currentPingPong%2);
    _mode = NORMAL_MODE;
    return;
  }


  if(_mode==NORMAL_MODE) {
    if(_w->updateOnceChanged()) {
      if(_w->reinitWhenChanged())
	initPingPong(false);
      applyOnce(false);
      
      _w->setPingPongNumberLabel(_currentPingPong);
      copyResult(1-_currentPingPong%2);

    } else if(_w->updateAllChanged()) {
      if(_w->reinitWhenChanged())
	initPingPong(false);
      _mode = PP_MODE;
      applyPingPong(false,false);
      _mode = NORMAL_MODE;
      _w->setPingPongNumberLabel(_currentPingPong);
      //copyResult(1-_currentPingPong%2);
    }
  }
}

void PingPongCustomNode::applyOnce(bool refresh) {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _currentPingPong++;

  const unsigned int currentITex = _currentPingPong%2;
  const unsigned int currentFbo  = 1-(currentITex);
  const unsigned int currentOTex = currentFbo;
  
  // same as setOutputParams (for ping pong tex)
  for(unsigned int i=0;i<nbOutputs();++i) {
    _pingPongTex[currentOTex][i]->setFilterMode(_outFilter);
    _pingPongTex[currentOTex][i]->setWrapMode(_outWrap);
  }
  
  // enable and add ping-pong textures 
  enableShaders();
  for(unsigned int i=0;i<nbOutputs();++i) {
    _p->setUniformTexture(_defautUniformNames[i],GL_TEXTURE_2D,_pingPongTex[currentITex][i]->id());
  }
  _p->setUniform1i(_defautUniformNames[nbOutputs()],_currentPingPong-1);
  
  // same as drawGenericOutputs (for ping-pong tex)
  _pingPongFbo[currentFbo]->bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  if(useTesselation()) {
    _glf->glPatchParameteri(GL_PATCH_VERTICES,4);
    _unitPatchVao->bind();
    _unitPatchVao->drawArrays(GL_PATCHES,0,4);
    _unitPatchVao->unbind(); 
  } else {
    _unitSquareVao->bind();
    _unitSquareVao->drawArrays(GL_TRIANGLES,0,6);
    _unitSquareVao->unbind();
  }
  
  disableShaders();
    
  if(refresh) {
    _w->setPingPongNumberLabel(_currentPingPong);
    copyResult(currentOTex);
    update();
  }
}

void PingPongCustomNode::applyPingPong(bool refreshAll,bool refreshIt) {
  if(!isReady()) return;
  refreshAll = true; // do not consider this variable to avoid GPU timeouts

  unsigned int nb = (unsigned int)_w->pingPongNumber();
  if(_w->reinitWhenChanged()) {
    initPingPong(nb==0 && refreshAll && !refreshIt);
  }

  for(unsigned int i=0;i<nb;++i) {
    applyOnce(refreshAll);
    //_glf->glWaitSync(_glf->glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE,0),GL_SYNC_FLUSH_COMMANDS_BIT,1800);
    QCoreApplication::sendPostedEvents(NULL,0);
  }

  if(!refreshAll && refreshIt) {
    _w->setPingPongNumberLabel(_currentPingPong);
    copyResult(1-_currentPingPong%2);
    update();
  }
}

void PingPongCustomNode::initPingPong(bool refresh) {
  if(!isReady()) return;

  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
 
  _pingPongFbo[0]->bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  initOpenGLState();
  cleanOpenGLState();
  _currentPingPong = 0;
  
  if(refresh) {
    _w->setPingPongNumberLabel(_currentPingPong);
    copyResult(0);
    update();
  }
}

void PingPongCustomNode::generateUniforms() {
  // generate texture names
  _defautUniformNames = QStringList();
  _defautUniformTypes = QStringList();
  for(unsigned int i=0;i<nbOutputs();++i) {
    _defautUniformNames << ("pingpong"+QString::number(i));
    _defautUniformTypes << "sampler2D";
  }
  _defautUniformNames << "numpass";
  _defautUniformTypes << "int";
}

void PingPongCustomNode::initFBO() {
  NodeTexture2D::initFBO();  

  if(nbOutputs()==0) return;
  
  // create textures
  TextureFormat tf = outputTex(0)->format();
  TextureParams tp = outputTex(0)->params();
  unsigned int w = outputTex(0)->w();
  unsigned int h = outputTex(0)->h();
  tf.setWidth(w);
  tf.setHeight(h);

  for(unsigned int i=0;i<nbOutputs();++i) {
    _pingPongTex[0].push_back(new FloatTexture2D(tf,tp));
    _pingPongTex[1].push_back(new FloatTexture2D(tf,tp));
  }

  // create FBOs
  _pingPongFbo[0] = new FramebufferObject();
  _pingPongFbo[1] = new FramebufferObject();

  _pingPongFbo[0]->bind();
  for(unsigned int i=0;i<nbOutputs();++i) {
    FloatTexture2D *tex = _pingPongTex[0][i];
    tex->bind();
    _pingPongFbo[0]->attachTexture(tex->format().target(),tex->id(),GL_COLOR_ATTACHMENT0+i);
  }
  _pingPongFbo[0]->isValid();

  _pingPongFbo[1]->bind();
  for(unsigned int i=0;i<nbOutputs();++i) {
    FloatTexture2D *tex = _pingPongTex[1][i];
    tex->bind();
    _pingPongFbo[1]->attachTexture(tex->format().target(),tex->id(),GL_COLOR_ATTACHMENT0+i);
  }
  _pingPongFbo[1]->isValid();

  generateUniforms();

  // create gpu program
  QStringList names = outputNames();
  QString vs = QObject::tr("#version 410\n"
			   "layout(location = 0) in vec2 vertex;\n"
			   "layout(location = 1) in vec2 coord;\n"
			   "out vec2 texcoord;\n"
			   "void main() {\n"   
			   "\tgl_Position = vec4(vertex,0,1);\n"
			   "\ttexcoord    = coord;\n"
			   "}\n");

  QString fs = QObject::tr("#version 410\n"
			   "in vec2 texcoord;\n");

  for(unsigned int i=0;i<nbOutputs();++i) {
    const QString n = QString::number(i);
    fs = fs+"layout(location = "+n+") out vec4 "+names[i]+";\n";
    fs = fs+"uniform sampler2D "+_defautUniformNames[i]+";\n";
  }

  fs = fs+"void main() {\n";
  for(unsigned int i=0;i<nbOutputs();++i) {
    const QString n = QString::number(i);
    fs = fs+names[i]+" = texture("+_defautUniformNames[i]+",texcoord);\n";
  }

  fs = fs+"}\n";
  _copyBuffers = new GPUProgram(vs,fs,"","","",false);
  for(unsigned int i=0;i<nbOutputs();++i) {
    _copyBuffers->addUniform(_defautUniformNames[i]);
  }
}

void PingPongCustomNode::cleanFBO() {
  NodeTexture2D::cleanFBO();

  delete _pingPongFbo[0]; _pingPongFbo[0] = NULL;
  delete _pingPongFbo[1]; _pingPongFbo[1] = NULL;
  delete _copyBuffers; _copyBuffers = NULL;

  for(unsigned int i=0;i<_pingPongTex[0].size();++i) {
    delete _pingPongTex[0][i];
  }
  
  for(unsigned int i=0;i<_pingPongTex[1].size();++i) {
    delete _pingPongTex[1][i];
  }
  
  _pingPongTex[0].clear();
  _pingPongTex[1].clear();
}

const QString PingPongCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec2 vertex;\n"
		     "layout(location = 1) in vec2 coord;\n");
}

const QString PingPongCustomNode::defaultVertBody()   {
  return QObject::tr("out vec2 texcoord;\n\n"
		     "void main() {\n"   
		     "\tgl_Position = vec4(vertex,0,1);\n"
		     "\ttexcoord    = coord;\n"
		     "}\n");
}

const QString PingPongCustomNode::constantFragHead()  {
  return QString();
}

const QString PingPongCustomNode::defaultFragBody()   {
  return QObject::tr("in vec2 texcoord;\n"
		     "void main() {\n"
		     "\toutBuffer0 = texture(inBuffer0,texcoord);\n"
		     "}\n");
}

const QString PingPongCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 4) out;\n");
}

const QString PingPongCustomNode::defaultTessCBody()  {
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

const QString PingPongCustomNode::constantTessEHead() {
  return QString();
}

const QString PingPongCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(quads,equal_spacing) in;\n\n"
		     "void main() {\n"
		     "\tvec4 p1 = mix(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_TessCoord.x);\n"
		     "\tvec4 p2 = mix(gl_in[3].gl_Position,gl_in[2].gl_Position,gl_TessCoord.x);\n"
		     "\tgl_Position = mix(p1,p2,gl_TessCoord.y);\n"
		     "}\n");
}

const QString PingPongCustomNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString PingPongCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}");
}

bool PingPongCustomNode::save(SceneSaver  *saver) {
  bool ret = true;
  if(!GenericCustomNode::save(saver)) ret = false;
  if(!saver->saveInt("pingPongNumber",_w->pingPongNumber())) ret = false;
  if(!saver->saveBool("pingPongUpdateOnceChanged",_w->updateOnceChanged())) ret = false;
  if(!saver->saveBool("pingPongUpdateAllChanged",_w->updateAllChanged())) ret = false;
  if(!saver->saveBool("pingPongReinitWhenChanged",_w->reinitWhenChanged())) ret = false;
  if(!saver->saveBool("pingPongRefreshAllFrames",_w->refreshAllFrames())) ret = false;
  return ret;
}

bool PingPongCustomNode::load(SceneLoader *loader) {
  bool ret = true;
  int p = 0;
  bool b1=false,b2=false,b3=true,b4=true;

  if(!GenericCustomNode::load(loader)) ret = false;
  if(!loader->loadInt("pingPongNumber",p)) ret = false;
  if(!loader->loadBool("pingPongUpdateOnceChanged",b1)) ret = false;
  if(!loader->loadBool("pingPongUpdateAllChanged",b2)) ret = false;
  if(!loader->loadBool("pingPongReinitWhenChanged",b3)) ret = false;
  if(!loader->loadBool("pingPongRefreshAllFrames",b4)) ret = false;
  
  _w->setPingPongNumber(p);
  _w->setUpdateOnceChanged(b1);
  _w->setUpdateAllChanged(b2);
  _w->setReinitWhenChanged(b3);
  _w->setRefreshAllFrames(b4);
  _w->setPingPongNumberLabel(0);

  generateUniforms();
    
  initHeads();
  
  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;
  initProg();

  if(isReady()) {    
    initPingPong(true);
  }

  return ret;
}
