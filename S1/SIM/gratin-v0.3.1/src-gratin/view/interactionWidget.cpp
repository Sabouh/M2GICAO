// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/interactionWidget.h"
#include "view/graphWidget.h"
#include "view/logWidget.h"
#include "misc/glutils.h"

using namespace std;

GlInteractionWidget::GlInteractionWidget(const QSurfaceFormat &format,QScreen *screen)
  : GlWindow(screen),
    _scale(1.0f),
    _interaction(NO_MOTION),
    _mouseCoord(Vector2f(0,0)),
    _mouseTmp(Vector2f(0,0)),
    _offset(Vector2f(0.0,0.0)),
    _onClick(false),
    _currentNode(-1),
    _tmpNode(-1),
    _textDrawer(NULL) {

  setSurfaceType(QSurface::OpenGLSurface);
  setFormat(format);
  create();

  GlContext::initOpenGLContext(this);
  initializeGL();

  connect(this,&GlInteractionWidget::widthChanged,this,&GlInteractionWidget::resizeGL);
  connect(this,&GlInteractionWidget::heightChanged,this,&GlInteractionWidget::resizeGL);
}

GlInteractionWidget::~GlInteractionWidget() {
  _nodes.clear();
  _outputs.clear();
  delete _textDrawer;
}

void GlInteractionWidget::render() {
  if(!isExposed()) return;

  _glcontext->makeCurrent(this);
  
  const Vector4f background(0.9f,0.9f,0.9f,1.0f);
  //const Vector4f background(1.0f,1.0f,1.0f,1.0f);
  const Vector4f tbackground(0.85f,0.85f,0.85f,1.0f);
  const Vector4f sbackground(0.8f,0.8f,0.5f,0.7f);
  
  _glf->glClearColor(background[0],background[1],background[2],background[3]);
  _glf->glClear(GL_COLOR_BUFFER_BIT);
  
  const Matrix4f mvp = Glutils::screenMode(width(),height(),_offset,_scale);
  
  for(unsigned int i=0;i<_nodes.size();++i) {
    const float x = i==0 ? 0.0f : _positions[i-1][0]+_nodes[i-1]->outputContentSize(_outputs[i-1])[0]+10;
    _positions[i] = Vector2f(x,0.0f);
    
    if(_nodes[i]->isReady()) {
      const Vector4f co = (int)i==_currentNode ? sbackground : tbackground;
      const Vector2f os = _nodes[i]->outputContentSize(_outputs[i]);
      const float     h = os[0]/10.0f;
      _nodes[i]->drawOutputContent(mvp,_positions[i],_outputs[i]);
      _textDrawer->draw(_nodes[i]->nameId()+" -> "+QString::number(_outputs[i]),
			_positions[i]-Vector2f(0,h),
			Vector2f(os[0],h),mvp,Vector4f(0,0,0,1),co);
    }
  }
  _glcontext->swapBuffers(this);
}

void GlInteractionWidget::mousePressEvent(QMouseEvent *me) {
  _onClick = true;
  _mouseCoord = remapCoord(me->x(),height()-me->y());

  _tmpNode = currentSelection(_mouseCoord);
  
  if(_tmpNode>=0 && _tmpNode==_currentNode) {
    _nodes[_currentNode]->mousePressEvent(_mouseCoord-_positions[_currentNode],me);
    LogWidget::instance()->setText("Interacting with node "+_nodes[_currentNode]->nameId());
    return;
  }

  if(me->button()==Qt::MidButton) {
    // zoom in and out
    _interaction = SCALING_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());

  } else if(me->button()==Qt::RightButton) {
    // translation
    _interaction = OFFSET_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());

  } else {
    _interaction = NO_MOTION;
  }

  updateGL();
}

void GlInteractionWidget::mouseMoveEvent(QMouseEvent *me) {
  if(!_onClick) return;

  if(_tmpNode>=0 && _tmpNode==_currentNode) {
    _mouseTmp = remapCoord(me->x(),height()-me->y());
    _nodes[_currentNode]->mouseMoveEvent(_mouseTmp-_positions[_currentNode],me);
   
    return;
  }

  if(_interaction==SCALING_MOTION) {
    // zoom in/out
    if(((height()-me->y())-_mouseTmp[1])>0 && _scale<100.0f) {
      _scale*=1.05f;
    } else if(((height()-me->y())-_mouseTmp[1])<0 && _scale>0.02f) {
      _scale*=0.95f;
    }
    _mouseTmp = Vector2f(me->x(),height()-me->y());

  } else if(_interaction==OFFSET_MOTION) {

    // background translations
    _offset  -= Vector2f(me->x()-_mouseTmp[0],(height()-me->y())-_mouseTmp[1])*_scale;
    _mouseTmp = Vector2f(me->x(),height()-me->y());
  }

  updateGL();
}

void GlInteractionWidget::mouseReleaseEvent(QMouseEvent *me) {

  if(_tmpNode>=0 && _tmpNode==_currentNode) {
    _mouseTmp = remapCoord(me->x(),height()-me->y());
    _nodes[_currentNode]->mouseReleaseEvent(_mouseTmp-_positions[_currentNode],me);
  }

  if(me->button()==Qt::LeftButton && currentSelection(remapCoord(me->x(),height()-me->y()))==_tmpNode) {
    _currentNode = _tmpNode;
  }

  // update interaction mode
  _interaction = NO_MOTION;

  updateGL();
  _onClick = false;
}

void GlInteractionWidget::keyPressEvent(QKeyEvent *ke) {
  if(ke->key()==Qt::Key_Escape) {
    _currentNode = _tmpNode = -1;
  } else if(_currentNode>=0 && _currentNode<(int)_nodes.size()-1 && 
	    ke->key()==Qt::Key_Right && (ke->modifiers() & Qt::ControlModifier)) {
    // swap current node with right neighbor
    swapNodes(_currentNode,_currentNode+1);
    _currentNode = _currentNode+1;
  } else if(_currentNode>=1 && ke->key()==Qt::Key_Left && (ke->modifiers() & Qt::ControlModifier)) {
    // swap current node with right neighbor
    swapNodes(_currentNode,_currentNode-1);
    _currentNode = _currentNode-1;
  } else if(ke->key()==Qt::Key_Delete) {

    if(_currentNode>=0 && _currentNode<=(int)_nodes.size()-1)
      removeNode(_nodes[_currentNode],_outputs[_currentNode]);

  } else if(_currentNode>=0) {
    _nodes[_currentNode]->keyPressEvent(ke);
    LogWidget::instance()->setText("Interacting with node "+_nodes[_currentNode]->nameId());
  }

  updateGL();
}

void GlInteractionWidget::keyReleaseEvent(QKeyEvent *) {
  updateGL();
}

void GlInteractionWidget::wheelEvent(QWheelEvent *we) {

  _mouseCoord = remapCoord(we->x(),height()-we->y());
  _tmpNode = currentSelection(_mouseCoord);

  if(_tmpNode>=0 && _tmpNode==_currentNode) {
    _nodes[_currentNode]->wheelEvent(_mouseCoord-_positions[_currentNode],we);
    LogWidget::instance()->setText("Interacting with node "+_nodes[_currentNode]->nameId());
    return;
  }

  if(we->delta()>0 && _scale<100.0f) {
    _scale*=1.1f;
  } else if(we->delta()<0 && _scale>0.02f) {
    _scale*=0.9f;
  }
  updateGL();
}

void GlInteractionWidget::updateGL() {
  renderLater();
}

void GlInteractionWidget::initializeGL() {
  _glcontext->makeCurrent(this);
  _glf->glDisable(GL_DEPTH_TEST);
  _glf->glClear(GL_COLOR_BUFFER_BIT);
  _textDrawer = new TextDrawer();
}

void GlInteractionWidget::resizeGL() {
  _glcontext->makeCurrent(this);
  _glf->glViewport(0,0,width(),height());

  if(isExposed()) {
    updateGL();
  }
}

void GlInteractionWidget::addNode(NodeInterface *node,unsigned int output) {
  if(contain(node,output)<0) {
    _nodes.push_back(node);
    _outputs.push_back(output);
    _positions.push_back(Vector2f(0,0));
  }
}

void GlInteractionWidget::removeNode(NodeInterface *node,unsigned int output) {
  int index = contain(node,output);
  if(index<0) return;

  if(_currentNode==index) {
    _currentNode = -1;
  } else if(_currentNode>index) {
    _currentNode = _currentNode-1;
  }

  _nodes.erase(_nodes.begin()+index);
  _outputs.erase(_outputs.begin()+index);
  _positions.erase(_positions.begin()+index);
}

void GlInteractionWidget::removeNode(NodeInterface *node) {
  for(unsigned int i=0;i<node->nbOutputs();++i) {
    removeNode(node,i);
  }
}

int GlInteractionWidget::contain(NodeInterface *node,unsigned int output) {
  for(unsigned int i=0;i<_nodes.size();++i) {
    if(_nodes[i]==node && _outputs[i]==output) {
      return i;
    }
  }
  return -1;
}

bool GlInteractionWidget::containNode(NodeInterface *node,unsigned int output) {
  return contain(node,output)>=0;
}

bool GlInteractionWidget::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveFloat   ("scale",_scale))   ret = false;
  if(!saver->saveVector2f("offset",_offset)) ret = false;
  return ret;
}

bool GlInteractionWidget::load(SceneLoader *loader) {
  bool ret = true;

  if(!loader->loadFloat   ("scale",_scale))   ret = false;
  if(!loader->loadVector2f("offset",_offset)) ret = false;

  _interaction = NO_MOTION;
  return ret;
}

void GlInteractionWidget::zoom(int type) {
  switch(type) {
  case 0: _scale = 1.0f; break;
  case 1: _scale = _scale*0.5f;break;
  case -1: _scale = _scale*2.0f;break;
  }
  updateGL();
}

void GlInteractionWidget::viewAll() {
  if(_nodes.empty()) return;

  Vector2f minpos = _positions[0];
  Vector2f maxpos = _positions[0]+_nodes[0]->outputContentSize(_outputs[0]);

  for(unsigned int i=1;i<_nodes.size();++i) {
    const Vector2f tmpmin = _positions[i];
    const Vector2f tmpmax = _positions[i]+_nodes[i]->outputContentSize(_outputs[i]);

    minpos[0] = min(minpos[0],tmpmin[0]);
    minpos[1] = min(minpos[1],tmpmin[1]);
    maxpos[0] = max(maxpos[0],tmpmax[0]);
    maxpos[1] = max(maxpos[1],tmpmax[1]);
  }
  
  Vector2f fullsize = maxpos-minpos+(maxpos-minpos)/20.0f;
  Vector2f scales(fullsize[0]/(float)width(),fullsize[1]/(float)height());

  _scale  = max(scales[0],scales[1]);
  _offset = Vector2f(0.0f,0.0f);

  Vector2f p1 = remapCoord(width()/2,height()/2);
  Vector2f p2 = remapCoord(0,0);
  Vector2f p3 = remapCoord(width(),height());
  Vector2f p4 = ((p3-p2)-fullsize)/2.0f;

  _offset = _offset+(p1-p2-p4);
  updateGL();
}

void GlInteractionWidget::exposeEvent(QExposeEvent *ev) {
  QWindow::exposeEvent(ev);
  updateGL();
}
