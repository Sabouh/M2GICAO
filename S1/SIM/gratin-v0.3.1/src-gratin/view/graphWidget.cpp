// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/graphWidget.h"
#include "view/customNodeWidget.h"
#include "core/nodeInterface.h"
#include "core/nodeHandle.h"
#include "core/nodeGrouped.h"
#include "misc/glutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <QDebug>
#include <QMessageBox>

using namespace std;

GraphWidget::GraphWidget(const QSurfaceFormat &format,QScreen *screen)
  : GlWindow(screen),
    _nodeInterfaceWidget(NULL),
    _nodeListWidget(NULL),
    _interactionWidget(NULL),
    _animationWidget(NULL),
    _nodeManager(NodeManager::instance()),
    _pbGraph(new PbGraph()),
    _animation(new Animation()),
    _scale(2.0f),
    _interaction(NO_MOTION),
    _clickState(NOTHING_HAS_BEEN_CLICKED),
    _mouseCoord(0,0),
    _mouseTmp(0,0),
    _offset(0.0f,0.0f),
    _ctrl(false),
    _shift(false),
    _onClick(false),
    _outNodeSelection(-1,-1),
    _inNodeSelection(-1,-1),
    _lineShader(NULL),    
    _pointShader(NULL),
    _copier(NULL),
    _paster(NULL){

  setlocale(LC_ALL,"C");
  srand(time(NULL));

  setSurfaceType(QSurface::OpenGLSurface);
  setFormat(format);
  create();

  GlContext::initOpenGLContext(this);
  initializeGL();

  connect(this,&GraphWidget::widthChanged,this,&GraphWidget::resizeGL);
  connect(this,&GraphWidget::heightChanged,this,&GraphWidget::resizeGL);
  connect(_pbGraph,&PbGraph::nodeContentChanged,this,&GraphWidget::updateGL);
}

GraphWidget::~GraphWidget() {
  delete _pbGraph;
  delete _nodeManager;
  delete _lineShader;
  delete _pointShader;
  delete _interactVao;
  delete _connectionVao;
  delete _animation;
  delete _copier;
  delete _paster;
  GlContext::cleanOpenGLContext();
}


void GraphWidget::render() {
  if(!isExposed()) return;

  _glcontext->makeCurrent(this); 

  //_glf->glClearColor(0.86f,0.86f,0.90f,1.0f);
  _glf->glClearColor(0.9f,0.9f,0.9f,1.0f);
   
  _glf->glClear(GL_COLOR_BUFFER_BIT);
  _glf->glEnable(GL_BLEND); 
  _glf->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  const Matrix4f mvp = Glutils::screenMode(width(),height(),_offset,_scale);

  // draw graph connections
  drawGraphConnections(mvp);

  // draw graph nodes 
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    _pbGraph->node(i)->draw(mvp);
  }

  // draw motion connection
  switch(_interaction) {
  case CONNECT_MOTION: 
    // the user is currently moving the output connection of a node 
    drawConnection(mvp);
    break;
  case MULTIPLE_SELECT_MOTION: 
    // the user is currently selecting a group of elements
    drawSelection(mvp);
    break;
  default:
    break;
  }

  _glf->glDisable(GL_BLEND); 

  _glcontext->swapBuffers(this);

  // update interaction widget
  _interactionWidget->updateGL();
}

void GraphWidget::updateGL() {
    renderLater();
}

void GraphWidget::updateGLForce() {
    render();
}

void GraphWidget::mousePressEvent(QMouseEvent *me) {
  _mouseCoord = remapCoord(me->x(),height()-me->y());

  if(me->button()==Qt::MidButton) {
    // zoom in and out
    _interaction = SCALING_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());
  } else if(me->button()==Qt::RightButton) {
    // translation
    _interaction = OFFSET_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());
  } else if(me->button()==Qt::LeftButton) {

    // first find what is clicked
    if(inputNodeClicked(_mouseCoord) && inputConnected(_inNodeSelection)) {
      NodeInterface *src = _pbGraph->node(_outNodeSelection[0]);
      NodeInterface *dst = _pbGraph->node(_inNodeSelection[0]);
      if(_pbGraph->removeConnection(src,_outNodeSelection[1],dst,_inNodeSelection[1])) {
	_outNodeSelection[0] = src->nodeId();
	_inNodeSelection[0] = dst->nodeId();
	graphStateChanged();
	LogWidget::instance()->setText("Connection between "+src->nameId()+" and "+dst->nameId()+" removed from the graph");
      }
      _clickState = AN_OUTPUT_HAS_BEEN_CLICKED;
      _interaction = CONNECT_MOTION;
      _mouseTmp = remapCoord(me->x(),height()-me->y());
    } else if(outputNodeClicked(_mouseCoord)) {
      _clickState = AN_OUTPUT_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    } else if(nodeClicked(_mouseCoord)) {
      _mouseTmp   = Vector2f(me->x(),height()-me->y());
      _clickState = A_NODE_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    } else {
      _mouseTmp   = Vector2f(me->x(),height()-me->y());
      _clickState = NOTHING_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    }
  }

  _onClick = true;
  updateGL();
}

void GraphWidget::mouseMoveEvent(QMouseEvent *me) {

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
  } else if(_interaction==NO_MOTION && _clickState==AN_OUTPUT_HAS_BEEN_CLICKED) {
    // the user will make a new connection
    _interaction = CONNECT_MOTION;
  } else if(_interaction==NO_MOTION && _clickState==A_NODE_HAS_BEEN_CLICKED) {
    
    if(clickedNodeSelected()) {
      // the node is already selected, we move everything
      _interaction = SELECT_MOTION;
    } else {
      // the node is not selected. Unselect everything and select this node
      unselectAll();
      selectClickedNode();
      _interaction = SELECT_MOTION;
    }

  } else if(_interaction==NO_MOTION && _clickState==NOTHING_HAS_BEEN_CLICKED && _onClick) {
    // create a rectangle selection
    _interaction = MULTIPLE_SELECT_MOTION;
  }


  if(_interaction==CONNECT_MOTION) {
    // the output position has been clicked and the user makes a new connection
    _mouseTmp = remapCoord(me->x(),height()-me->y());
  } else if(_interaction==SELECT_MOTION) {
    // the user clicked on a node and displace it (and possibly all the other selected nodes)
    moveSelection(Vector2f(me->x()-_mouseTmp[0],(height()-me->y())-_mouseTmp[1])*_scale);
    _mouseTmp = Vector2f(me->x(),height()-me->y());
  } else if(_interaction==MULTIPLE_SELECT_MOTION) {
    _mouseTmp = remapCoord(me->x(),height()-me->y());
  }

  if(_onClick) {
    updateGL();
  }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *me) {

  if(_interaction==CONNECT_MOTION) {
    // check if the final coord corresponds to an input and connect
    _mouseTmp = remapCoord(me->x(),height()-me->y());

    if(inputNodeClicked(_mouseTmp)) {
      NodeInterface *src = _pbGraph->node(_outNodeSelection[0]);
      NodeInterface *dst = _pbGraph->node(_inNodeSelection[0]);
      if(_pbGraph->makeConnection(src,_outNodeSelection[1],dst,_inNodeSelection[1])) {
	graphStateChanged();
	_pbGraph->applyFrom(dst);
	LogWidget::instance()->setText("Connection between "+src->nameId()+" and "+dst->nameId()+" added to the graph");
      } else
        LogWidget::instance()->setText("Connection not allowed!");
    } 

  } else if(_interaction==MULTIPLE_SELECT_MOTION) {
    unselectNodeOutputs();

    // select / unselect the rectangle selection
    if(_ctrl) selectSelection();
    else if(_shift) unselectSelection();
    else {unselectAll(); selectSelection();}
  } else if(_interaction==NO_MOTION && _clickState==A_NODE_HAS_BEEN_CLICKED) {
    unselectNodeOutputs();

    // select / unselect the selection
    if(_ctrl) selectClickedNode();
    else if(_shift) unselectClickedNode();
    else {
      unselectAll(); 
      selectClickedNode();
      //TODO: test if an output has been clicked here
      selectNodeOutput();
    }
  } else if(_interaction==NO_MOTION && _clickState==NOTHING_HAS_BEEN_CLICKED) {
    // single click on background: unselected everything
    unselectAll();
  } 

  // back to initial state
  _interaction = NO_MOTION;
  _clickState  = NOTHING_HAS_BEEN_CLICKED;

  _onClick = false;
  updateGL();
}


void GraphWidget::mouseDoubleClickEvent(QMouseEvent *me) {
  if(_interaction==NO_MOTION && me->button()==Qt::LeftButton && _pbGraph->nbNodes()>(unsigned int)_outNodeSelection[0]) {
    // highlight the corresponding widget 
    NodeInterface *n = _pbGraph->node(_outNodeSelection[0]);
    NodeWidget *w = n->widget();
    _nodeInterfaceWidget->highlightWidget(w);
    if(w) {
      _nodeInterfaceWidget->show();
      LogWidget::instance()->setText("Widget of node "+n->nameId());
    } else
      LogWidget::instance()->setText("Node "+n->nameId()+" -- no widget");

    mousePressEvent(me);  
  } // else if(me->button()==Qt::RightButton) {

  // }
}

void GraphWidget::wheelEvent(QWheelEvent *we) {
  if(we->delta()>0 && _scale<100.0f) {
    _scale*=1.1f;
  } else if(we->delta()<0 && _scale>0.02f) {
    _scale*=0.9f;
  }
  updateGL();
}


void GraphWidget::keyPressEvent(QKeyEvent *ke) {
  if(ke->modifiers() & Qt::ControlModifier) {
    // control is pressed
    _ctrl = true;
  } 

  if(ke->modifiers() & Qt::ShiftModifier) {
    // shift is pressed
    _shift = true;
  } 

  if(ke->key()==Qt::Key_A && _ctrl) {
    // select/unselect everything
    if(everythingIsSelected()) {
      unselectAll();
    } else {
      selectAll();
    }
  } else if(ke->key()==Qt::Key_Escape) {
    unselectAll();
  } else if(ke->key()==Qt::Key_Delete) {
    removeSelection();
  } else if(ke->key()==Qt::Key_Space) {

    if(_outNodeSelection[0]<0 || _outNodeSelection[1]<0) return;

    NodeInterface *node = _pbGraph->node(_outNodeSelection[0]);
    unsigned int outId = _outNodeSelection[1];

    if(_interactionWidget->containNode(node,outId)) {
      _interactionWidget->removeNode(node,outId);
      LogWidget::instance()->setText("Removing output "+QString::number(outId)+" of node "+node->nameId()+" from the interaction widget");
    } else {
      _interactionWidget->addNode(node,outId);
      LogWidget::instance()->setText("Adding output "+QString::number(outId)+" of node "+node->nameId()+" to the interaction widget");
      _dockInteractionWidget->show();
    }
  }

  updateGL();
}

void GraphWidget::keyReleaseEvent(QKeyEvent *ke) {
  if(!(ke->modifiers() & Qt::ControlModifier)) {
    _ctrl = false;
  } 

  if(!(ke->modifiers() & Qt::ShiftModifier)) {
    _shift = false;
  } 

  updateGL();
}

void GraphWidget::zoom(int type) {
  switch(type) {
  case 0: _scale = 1.0f; break;
  case 1: _scale = _scale*0.5f;break;
  case -1: _scale = _scale*2.0f;break;
  }
  updateGL();
}

void GraphWidget::drawConnection(const Matrix4f &mvp) {
  updateConnectionBuffer();

  const float size = 15.0f;
  const Vector3f pColor(0.7f,0.3f,0.3f);
  const Vector3f lColor(0.3f,0.3f,0.8f);

  _interactVao->bind();

  _lineShader->enable();
  _lineShader->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _lineShader->setUniform3fv("color",(GLfloat *)lColor.data());
  _interactVao->drawArrays(GL_LINES,0,2);
  _lineShader->disable();
  
  _pointShader->enable();
  _pointShader->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pointShader->setUniform3fv("color",(GLfloat *)pColor.data());
  _pointShader->setUniform1f("size",size);
  _interactVao->drawArrays(GL_POINTS,1,1);
  _pointShader->disable();

  _interactVao->unbind();
}

void GraphWidget::drawGraphConnections(const Matrix4f &mvp) {
  updateGraphBuffer();
  if(_nbConnections==0) return;

  const Vector3f lColor(0.3f,0.3f,0.8f);

  _connectionVao->bind();
  _lineShader->enable();

  _lineShader->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _lineShader->setUniform3fv("color",(GLfloat *)lColor.data());  
  _connectionVao->drawArrays(GL_LINES,0,_nbConnections);

  _lineShader->disable();
  _connectionVao->unbind();
}

void GraphWidget::drawSelection(const Matrix4f &mvp) {
  updateSelectionBuffer();

  const Vector3f color(0.7f,0.3f,0.3f);

  _interactVao->bind();

  _lineShader->enable();

  _lineShader->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _lineShader->setUniform3fv("color",(GLfloat *)color.data());
  _interactVao->drawArrays(GL_LINE_LOOP,2,4);

  _lineShader->disable();
  _interactVao->unbind();
}


void GraphWidget::initializeGL() {
  // make current context active 
  _glcontext->makeCurrent(this);

  // default opengl values 
  _glf->glDisable(GL_DEPTH_TEST);
  _glf->glEnable(GL_LINE_SMOOTH); 
  _glf->glEnable(GL_PROGRAM_POINT_SIZE);
  _glf->glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  _glf->glClearColor(0.78f,0.84f,0.87f,1.0f);
  _glf->glClear(GL_COLOR_BUFFER_BIT);

  // init vaos 
  vector<Vector2f> pts(6,Vector2f(0.0f,0.0f));
  _interactVao   = new VertexarrayObject();
  _connectionVao = new VertexarrayObject();
  _interactVao->addAttrib(6*sizeof(Vector2f),(const void *)pts[0].data(),2,0,GL_DYNAMIC_DRAW);
  _connectionVao->addAttrib(2*sizeof(Vector2f),NULL,2,0,GL_DYNAMIC_DRAW);
  _nbConnections = 0;

  // init shader 
  _lineShader = new GPUProgram(":/line.vert",":/line.frag");
  _lineShader->addUniform("mvp");
  _lineShader->addUniform("color");

  _pointShader = new GPUProgram(":/point.vert",":/point.frag");
  _pointShader->addUniform("mvp");
  _pointShader->addUniform("size");
  _pointShader->addUniform("color");
}

void GraphWidget::updateConnectionBuffer() {
 vector<Vector2f> pts(2);
  pts[0] = _pbGraph->node(_outNodeSelection[0])->outputNodeCoord(_outNodeSelection[1]);
  pts[1] = _mouseTmp;

  _interactVao->updateAttribArray(0,0,2*sizeof(Vector2f),(const void *)pts[0].data());
}

void GraphWidget::updateSelectionBuffer() {
  vector<Vector2f> pts(4);
  pts[0] = _mouseCoord;
  pts[1] = Vector2f(_mouseTmp[0],_mouseCoord[1]);
  pts[2] = _mouseTmp;
  pts[3] = Vector2f(_mouseCoord[0],_mouseTmp[1]);

  _interactVao->updateAttribArray(0,2*sizeof(Vector2f),4*sizeof(Vector2f),(const void *)pts[0].data());
}

void GraphWidget::updateGraphBuffer() {
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();

  vector<Vector2f> pts;
  
  for(unsigned int i=0;i<m.size();++i) {
    for(unsigned int j=i+1;j<m[i].size();++j) {
      for(unsigned int k=0;k<m[i][j].size();++k) {
	const Vector2u oi = m[i][j][k];
	NodeInterface *src = _pbGraph->node(i);
	NodeInterface *dst = _pbGraph->node(j);

	pts.push_back(src->outputNodeCoord(oi[0]));
	pts.push_back(dst->inputNodeCoord(oi[1]));
      }
    }
  }

  if(!pts.empty())
    _connectionVao->updateAttribArray(0,0,pts.size()*sizeof(Vector2f),(const void *)pts[0].data());
}

void GraphWidget::setGraphBuffer() {
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();

  vector<Vector2f> pts;
  
  for(unsigned int i=0;i<m.size();++i) {
    for(unsigned int j=i+1;j<m[i].size();++j) {
      for(unsigned int k=0;k<m[i][j].size();++k) {
	const Vector2u oi = m[i][j][k];
	NodeInterface *src = _pbGraph->node(i);
	NodeInterface *dst = _pbGraph->node(j);

	pts.push_back(src->outputNodeCoord(oi[0]));
	pts.push_back(dst->inputNodeCoord(oi[1]));
      }
    }
  }

  if(!pts.empty())
    _connectionVao->setAttrib(0,pts.size()*sizeof(Vector2f),(const void *)pts[0].data(),2,0,GL_DYNAMIC_DRAW);
  
  _nbConnections = static_cast<unsigned int>(pts.size());
}

void GraphWidget::resizeGL() {
  _glcontext->makeCurrent(this);
  _glf->glViewport(0,0,width(),height());

  if(isExposed()) {
    updateGL();
  }
}

void GraphWidget::setNodeListWidget(NodeListWidget *nodeListWidget) {
  _nodeListWidget = nodeListWidget;
  if(!_nodeListWidget) return;

  _nodeListWidget->setNodeList(_nodeManager->nodeHandles());
}

void GraphWidget::setNodeInterfaceWidget(NodeInterfaceWidget *nodeInterfaceWidget) {
  _nodeInterfaceWidget = nodeInterfaceWidget;
}

void GraphWidget::setInteractionWidget(GlInteractionWidget *interactionWidget) {
  _interactionWidget = interactionWidget;
}

void GraphWidget::setDockInteractionWidget(QDockWidget *dockInteractionWidget) {
  _dockInteractionWidget = dockInteractionWidget;
}


void GraphWidget::setAnimationWidget(AnimationWidget *animationWidget) {
  _animationWidget = animationWidget;
}

void GraphWidget::reloadShaders() {
  _pbGraph->reloadAll();
  _pbGraph->applyAll();
  updateGL();
  LogWidget::instance()->setText("Graph reloaded");
}

void GraphWidget::applyNodes() {
  _pbGraph->applyAll();
  updateGL();
}

void GraphWidget::addNode(const QString &name,const QString &path) {
  NodeInterface *node = _nodeManager->createInstance(_pbGraph,name,path);
  
  if(!node) return;
  //cout << __FILE__ << " - " << __FUNCTION__ << " creating " << path.toStdString() << name.toStdString() << endl;

  node->initEmptyData(_offset);
  
  if(!(_pbGraph->addNode(node))) {
    delete node;
    return;
  }

  if(_nodeManager->isCustomNode(node->name(),node->path()) ||
     _nodeManager->isGroupedNode(node->name(),node->path())) {
    _nodeManager->loadCustomNodeData(node);
  }

  LogWidget::instance()->setText(name+" added to the graph");


  _pbGraph->applyFrom(node);

  graphStateChanged();
  updateGL();
}

void GraphWidget::removeNode(NodeInterface *node) {
  removeFromInteractionWidgets(node);
  _pbGraph->removeNode(node);
  LogWidget::instance()->setText(node->name()+" removed from the graph");
  node->cleanEverything();
  delete node;
  graphStateChanged();
  updateGL();
}

void GraphWidget::graphStateChanged() {
  // change node name ids in the associated widgets 
  _nodeInterfaceWidget->updateIds();
  _animationWidget->updateIds();
  setGraphBuffer();
}

void GraphWidget::clear() {
  selectAll();
  removeSelection();
}

void GraphWidget::selectUnselectAll() {
  if(everythingIsSelected()) 
    unselectAll();
  else 
    selectAll();
  updateGL();
}

void GraphWidget::copy() {
  delete _copier; _copier = NULL;
  bool ret = true;

  // get selected nodes
  Vector2f meanPos(0,0); 
  vector<unsigned int> selectedNodes;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      selectedNodes.push_back(i);
    }
  }

  // copy connections between these nodes 
  const vector<vector<vector<Vector2u> > > adjacency = _pbGraph->adjacencyMatrix();
  vector<vector<vector<Vector2u> > > selectedConnections;
  if(selectedNodes.size()>0)
    selectedConnections = vector<vector<vector<Vector2u> > >(selectedNodes.size());

  for(unsigned int i=0;i<selectedNodes.size();++i) {
    meanPos = meanPos+_pbGraph->node(selectedNodes[i])->pos()/(float)selectedNodes.size();
    selectedConnections[i] = vector<vector<Vector2u> >(selectedNodes.size());
    for(unsigned int j=i+1;j<selectedNodes.size();++j) {
      const unsigned int index1 = selectedNodes[i];
      const unsigned int index2 = selectedNodes[j];
      selectedConnections[i][j] = adjacency[index1][index2];
    }
  }

  // init copier 
  _copier = new SceneSaver();
  _copier->init("selection-copy");
  
  // copy nodes information
  _copier->saveUint("nbNodes",static_cast<unsigned int>(selectedNodes.size()));
  _copier->saveVector2f("meanPos",meanPos);

  for(unsigned int i=0;i<selectedNodes.size();++i) {
    NodeManager::instance()->saveNode(_copier,_pbGraph->node(selectedNodes[i]),_pbGraph,ret,i);
  }
  
  // copy node connections 
  for(unsigned int i=0;i<selectedNodes.size();++i) {
    for(unsigned int j=0;j<selectedNodes.size();++j) {
      _copier->saveUint(Tags::attribId("nbConnections",i,j),static_cast<unsigned int>(selectedConnections[i][j].size()));
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	_copier->saveVector2u(Tags::attribId("connection",i,j,k),selectedConnections[i][j][k]);
      }
    }
  }

  // copy user-defined nodes parameters 
  for(unsigned int i=0;i<selectedNodes.size();++i) {
    NodeManager::instance()->saveNodeData(_copier,_pbGraph->node(selectedNodes[i]),ret,i);
  }

  LogWidget::instance()->setText("Selection copied");
}

void GraphWidget::paste() {
  if(!_copier) return;
  bool ret = true;

  delete _paster; _paster = NULL;
  _paster = new SceneLoader();
  _paster->readFrom(_copier);
  _paster->init("selection-copy");
  
  unsigned int nb = 0;
  Vector2f meanPos(0,0);

  // get nb nodes and create necessary arrays
  _paster->loadUint("nbNodes",nb);
  _paster->loadVector2f("meanPos",meanPos);

  vector<NodeInterface *> selectedNodes;
  vector<vector<vector<Vector2u> > > selectedConnections;
  if(nb>0) {
    selectedNodes = vector<NodeInterface *>(nb,NULL);
    selectedConnections = vector<vector<vector<Vector2u> > >(nb);
    for(unsigned int i=0;i<selectedNodes.size();++i) {
      selectedConnections[i] = vector<vector<Vector2u> >(nb); 
    }
  }

  // create nodes 
  for(unsigned int i=0;i<nb;++i) {
    selectedNodes[i] = NodeManager::instance()->loadNode(_paster,_pbGraph,ret,i);
    if(!selectedNodes[i]) {
      // an error occurred... cancel everything
      for(int j=(int)i-1;j>=0;--j) {
	delete selectedNodes[j];
	LogWidget::instance()->setText("Unable to paste selection...");
	return;
      }
    }
    selectedNodes[i]->initEmptyData(selectedNodes[i]->pos()-meanPos+_offset);
  }

  // get connections 
  for(unsigned int i=0;i<nb;++i) {
    for(unsigned int j=0;j<nb;++j) {
      unsigned int nbConnections=0;
      _paster->loadUint(Tags::attribId("nbConnections",i,j),nbConnections);
      if(nbConnections>0) selectedConnections[i][j]=vector<Vector2u>(nbConnections);
   
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	_paster->loadVector2u(Tags::attribId("connection",i,j,k),selectedConnections[i][j][k]);
      }
    }
  }

  // insert nodes in the current graph 
  for(unsigned int i=0;i<nb;++i) {
    cout << "adding " << selectedNodes[i]->nameId().toStdString() << " to the graph" << endl;
    _pbGraph->addNode(selectedNodes[i]);
  }

  // insert connections
  for(unsigned int i=0;i<nb;++i) {
    for(unsigned int j=i+1;j<nb;++j) {
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	cout << "making connection" << endl;
	NodeInterface *src = selectedNodes[i];
	NodeInterface *dst = selectedNodes[j];
	unsigned int output = selectedConnections[i][j][k][0];
	unsigned int input  = selectedConnections[i][j][k][1];
	_pbGraph->makeConnection(src,output,dst,input);
      }
    }
  }

  // load user-defined data 
  for(unsigned int i=0;i<nb;++i) {
    NodeManager::instance()->loadNodeData(_paster,selectedNodes[i],ret,i);
  }

  // apply all new nodes 
  for(unsigned int i=0;i<nb;++i) {
    if(selectedNodes[i]->isReady()) {
      selectedNodes[i]->apply();
    }
  }

  graphStateChanged();
  LogWidget::instance()->setText("Selection pasted");
  updateGL();
}

void GraphWidget::group() {

  vector<NodeInterface *> nodes;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      nodes.push_back(_pbGraph->node(i));
    }
  }

  // test: at least 2 nodes   
  if(nodes.size()<2) {
    LogWidget::instance()->setText("Please, select at least two nodes for grouping");
    return;
  }

  // test: need connex graphs selected
  const PbGraph::PerNodeTL tl = _pbGraph->perNodeTL();
  for(unsigned int i=0;i<nodes.size();++i) {
    bool unconnectedFound = false;
    unsigned int id = nodes[i]->nodeId();
    for(unsigned int j=0;j<tl[id].size();++j) {
      if(unconnectedFound && nodeSelected(tl[id][j])) {
	LogWidget::instance()->setText("Please, select a connex graph for grouping");
	return;
      }

      if(!nodeSelected(tl[id][j])) {
	unconnectedFound = true;
      }
    }
  }

  // detach all nodes from the widgets 
  for(unsigned int i=0;i<nodes.size();++i) {
    _animationWidget->delParams(nodes[i]->widget());
    _nodeInterfaceWidget->removeWidget(nodes[i]->widget());
    _interactionWidget->removeNode(nodes[i]);
  }

  // need to remember connections...
  vector<pair<NodeInterface *,NodeInterface *> > inPrevConnectedNodes;
  vector<Vector2u>                               inPrevConnectedOutIn;
  vector<pair<NodeInterface *,NodeInterface *> > outPrevConnectedNodes;
  vector<Vector2u >                              outPrevConnectedOutIn;
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();

  // for each selected node 
  for(unsigned int i=0;i<nodes.size();++i) {

    // for each other nodes 
    for(unsigned int j=0;j<_pbGraph->nbNodes();++j) {
      // don't care about connections between selected nodes 
      if(nodeSelected(j)) continue; 

      // previous input connections 
      for(unsigned int k=0;k<m[j][nodes[i]->nodeId()].size();++k) {
	inPrevConnectedNodes.push_back(pair<NodeInterface *,NodeInterface *>(_pbGraph->node(j),nodes[i]));
	inPrevConnectedOutIn.push_back(m[j][nodes[i]->nodeId()][k]);
      }

      // previous output connections
      for(unsigned int k=0;k<m[nodes[i]->nodeId()][j].size();++k) {
	outPrevConnectedNodes.push_back(pair<NodeInterface *,NodeInterface *>(nodes[i],_pbGraph->node(j)));
	outPrevConnectedOutIn.push_back(m[nodes[i]->nodeId()][j][k]);
      }
    }
  }

  // remove in connections 
  for(unsigned int i=0;i<inPrevConnectedNodes.size();++i) {
    _pbGraph->removeConnection(inPrevConnectedNodes[i].first,inPrevConnectedOutIn[i][0],
			       inPrevConnectedNodes[i].second,inPrevConnectedOutIn[i][1]);
  }

  // remove out connections 
  for(unsigned int i=0;i<outPrevConnectedNodes.size();++i) {
    _pbGraph->removeConnection(outPrevConnectedNodes[i].first,outPrevConnectedOutIn[i][0],
			       outPrevConnectedNodes[i].second,outPrevConnectedOutIn[i][1]);
  }

  // group creation 
  NodeGrouped *nodeGrouped = _pbGraph->groupNodes(nodes);
  if(!nodeGrouped) {
    LogWidget::instance()->setText("Group creation failed");
    return;
  }
  unselectAll();
  
  // recreate input connections between the group and the external nodes 
  for(unsigned int i=0;i<inPrevConnectedNodes.size();++i) {
    int n = nodeGrouped->inputNumber(inPrevConnectedNodes[i].second,inPrevConnectedOutIn[i][1]);
    if(n>=0) {
      _pbGraph->makeConnection(inPrevConnectedNodes[i].first,inPrevConnectedOutIn[i][0],
			       (NodeInterface *)nodeGrouped,(unsigned int)n);
    }
  }

  // recreate output connections between the group and the external nodes 
  for(unsigned int i=0;i<outPrevConnectedNodes.size();++i) {
    int n = nodeGrouped->outputNumber(outPrevConnectedNodes[i].first,outPrevConnectedOutIn[i][0]);
    if(n>=0) {
      _pbGraph->makeConnection((NodeInterface *)nodeGrouped,(unsigned int)n,
			       outPrevConnectedNodes[i].second,outPrevConnectedOutIn[i][1]);
    }
  }

  // update 
  _pbGraph->applyFrom(nodeGrouped);
  graphStateChanged();
  updateGL();

  LogWidget::instance()->setText("Group created");
}

void GraphWidget::ungroup() {
  NodeInterface *node = NULL;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      if(!node)
	node = _pbGraph->node(i);
      else {
	LogWidget::instance()->setText("Please, select only one single node");
	return;
      }
    } 
  }
  
  // check if one node is selected
  if(!node) {
    LogWidget::instance()->setText("Nothing selected");
    return;
  }
  
  // check if the node is a nodeGrouped
  if(!_nodeManager->isGroupedNode(node)) {
    LogWidget::instance()->setText("Node not grouped");
    return;
  }
  
  NodeGrouped *ng = (NodeGrouped *)node;

  // need to remember connections...
  vector<pair<NodeInterface *,NodeInterface *> > inPrevConnectedNodes;
  vector<Vector2u>                               inPrevConnectedOutIn;
  vector<pair<NodeInterface *,NodeInterface *> > outPrevConnectedNodes;
  vector<Vector2u >                              outPrevConnectedOutIn;
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();

  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    // remove / remember input connections
    for(unsigned int k=0;k<m[i][ng->nodeId()].size();++k) {
      Vector2u c = m[i][ng->nodeId()][k];
      std::pair<NodeInterface *,unsigned int> n = ng->nodeFromInputNumber(c[1]);

      if(n.first) {
	inPrevConnectedNodes.push_back(std::pair<NodeInterface *,NodeInterface *>(_pbGraph->node(i),n.first));
	inPrevConnectedOutIn.push_back(Vector2u(c[0],n.second));
      }
      
      _pbGraph->removeConnection(_pbGraph->node(i),c[0],ng,c[1]);
    }

    // remove / remember output connections
    for(unsigned int k=0;k<m[ng->nodeId()][i].size();++k) {
      Vector2u c = m[ng->nodeId()][i][k];
      std::pair<NodeInterface *,unsigned int> n = ng->nodeFromOutputNumber(c[0]);
      
      if(n.first) {
	outPrevConnectedNodes.push_back(std::pair<NodeInterface *,NodeInterface *>(n.first,_pbGraph->node(i)));
	outPrevConnectedOutIn.push_back(Vector2u(n.second,c[1]));
      }

      _pbGraph->removeConnection(ng,c[0],_pbGraph->node(i),c[1]);
    }
  }

  // ungroup 
  _animationWidget->delParams(ng->widget());
  _nodeInterfaceWidget->removeWidget(ng->widget());
  _interactionWidget->removeNode(ng);
  NodeGrouped *nodeGrouped = _pbGraph->ungroupNode((NodeGrouped *)node);
  if(!nodeGrouped) {
    LogWidget::instance()->setText("Ungroup operation failed");
    return;
  } 
  nodeGrouped->cleanEverything();
  delete nodeGrouped;
  unselectAll();

  // recreate input connections between the group and the external nodes 
  for(unsigned int i=0;i<inPrevConnectedNodes.size();++i) {
    _pbGraph->makeConnection(inPrevConnectedNodes[i].first,inPrevConnectedOutIn[i][0],
			     inPrevConnectedNodes[i].second,inPrevConnectedOutIn[i][1]);
  }

  // recreate output connections between the group and the external nodes 
  for(unsigned int i=0;i<outPrevConnectedNodes.size();++i) {
    _pbGraph->makeConnection(outPrevConnectedNodes[i].first,outPrevConnectedOutIn[i][0],
			     outPrevConnectedNodes[i].second,outPrevConnectedOutIn[i][1]);
  }


  // update 
  _pbGraph->applyAll();
  graphStateChanged();
  updateGL();
  LogWidget::instance()->setText("Node ungrouped");
}


void GraphWidget::addNodeToList() {
  NodeInterface *node = NULL;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      if(!node)
	node = _pbGraph->node(i);
      else {
	LogWidget::instance()->setText("Please, select only one single node");
	return;
      }
    } 
  }
  
  // check if one node is selected
  if(!node) {
    return;
  }

  CustomNodeWidget w(node,this);
  w.exec();
}

void GraphWidget::manageNodePaths() {
  ManagePathsWidget w;
  w.exec();
}

void GraphWidget::saveNodeContent() {
  NodeInterface *node = NULL;
  unsigned int output = 0;
  
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(!_pbGraph->node(i)) continue;
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      if(!_pbGraph->node(i)->output(j)) continue;
      
      if(_pbGraph->node(i)->output(j)->selected()) {
  	if(!node) {
  	  node = _pbGraph->node(i);
  	  output = j;
  	}else {
  	  LogWidget::instance()->setText("Please, select only one single node output");
  	  return;
  	}
      
      }
    }
  }

    // check if one node is selected
    if(!node) {
      return;
    }
    
    QString filename = node->prepareToExportOutputContent(output);

    if(filename.isNull() || filename.isEmpty()) return;

    QFile f(filename);
    if(f.exists()) {
      QString text = filename+" already exist! Do you want to replace it?";
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(NULL,tr("File exists"),text,
  				 QMessageBox::Save | QMessageBox::Cancel);
      if(ret==QMessageBox::Cancel)
  	return;
    }
    
    if(node->exportOutputContent(filename,output))
      LogWidget::instance()->setText(filename+" saved!");
    else
      LogWidget::instance()->setText(filename+" failed");
}

void GraphWidget::saveAnimNodeContent() {
  NodeInterface *node = NULL;
  unsigned int output = 0;
  
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(!_pbGraph->node(i)) continue;
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      if(!_pbGraph->node(i)->output(j)) continue;
      
      if(_pbGraph->node(i)->output(j)->selected()) {
  	if(!node) {
  	  node = _pbGraph->node(i);
  	  output = j;
  	}else {
  	  LogWidget::instance()->setText("Please, select only one single node output");
  	  return;
  	}
      
      }
    }
  }

    // check if one node is selected
    if(!node) {
      return;
    }
    
    QString filename = node->prepareToExportOutputContent(output);

    if(filename.isNull() || filename.isEmpty()) return;

    unsigned int firstFrame = _animation->firstFrame();
    unsigned int lastFrame = _animation->lastFrame();
    unsigned int zeros = (unsigned int)ceil(log10((double)lastFrame));

    QStringList p = filename.split("/");
    QString name = p[p.size()-1];
    QString path;
    for(int i=0;i<p.size()-1;++i) {
      path = path + p[i] + "/";
    }

    char f[1024];
    unsigned int nbExistingFiles = 0;
    for(unsigned int i=firstFrame;i<=lastFrame;++i) {
      sprintf(f,"%s%0*d-%s", path.toStdString().c_str(),zeros, i,name.toStdString().c_str());
      QFile file(f);
      if(file.exists()) {
	nbExistingFiles++;
      }
    }

    if(nbExistingFiles>0) {
      QString text = " Do you want to replace " + QString::number(nbExistingFiles) + " files?";
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(NULL,tr("File exists"),text,
    				 QMessageBox::Save | QMessageBox::Cancel);
      if(ret==QMessageBox::Cancel)
    	return;
    }
    
    for(unsigned int i=firstFrame;i<=lastFrame;++i) {
      sprintf(f,"%s%0*d-%s", path.toStdString().c_str(),zeros, i,name.toStdString().c_str());
      _animation->setCurrentFrame(i);
      _animationWidget->animationFrameChanged();
      if(node->exportOutputContent(f,output))
        LogWidget::instance()->setText(QString(f)+" saved!");
      else
        LogWidget::instance()->setText(QString(f)+" failed!");
      
      _animationWidget->repaint();
      _interactionWidget->updateGL();
      _nodeListWidget->repaint();
      _nodeInterfaceWidget->repaint();
      updateGL();
    }
}


bool GraphWidget::save(SceneSaver  *saver) {
  bool ret = true;
  if(!saver->saveIOData  ("graph"    ,_pbGraph  )) ret = false;
  if(!saver->saveFloat   ("scale"    ,_scale    )) ret = false;
  if(!saver->saveVector2f("offset"   ,_offset   )) ret = false;
  if(!saver->saveIOData  ("animation",_animation)) ret = false;
  return ret;
}

bool GraphWidget::load(SceneLoader *loader) {
  bool ret = true;

  _interaction      = NO_MOTION;
  _clickState       = NOTHING_HAS_BEEN_CLICKED;
  _mouseCoord       = Vector2f(0,0);
  _mouseTmp         = Vector2f(0,0);
  _inNodeSelection  = Vector2i(-1,-1);
  _outNodeSelection = Vector2i(-1,-1);
  _ctrl             = false;
  _shift            = false;

  if(!loader->loadIOData  ("graph"    ,_pbGraph  )) ret = false;
  if(!loader->loadFloat   ("scale"    ,_scale    )) ret = false;
  if(!loader->loadVector2f("offset"   ,_offset   )) ret = false;
  if(!loader->loadIOData  ("animation",_animation)) ret = false;

  animationSettingsChanged();
  graphStateChanged();

  return ret;
}

void GraphWidget::removeFromInteractionWidgets(NodeInterface *node) {
  if(!node) return;
  _animationWidget->delParams(node->widget());
  _nodeInterfaceWidget->removeWidget(node->widget());
  _interactionWidget->removeNode(node);
}

void GraphWidget::viewAll() {
  if(_pbGraph->nbNodes()==0) return;

  Vector2f minpos = _pbGraph->node(0)->pos();
  Vector2f maxpos = _pbGraph->node(0)->pos()+_pbGraph->node(0)->size();

  for(unsigned int i=1;i<_pbGraph->nbNodes();++i) {
    const Vector2f tmpmin = _pbGraph->node(i)->pos();
    const Vector2f tmpmax = _pbGraph->node(i)->pos()+_pbGraph->node(i)->size();

    minpos[0] = min(minpos[0],tmpmin[0]);
    minpos[1] = min(minpos[1],tmpmin[1]);
    maxpos[0] = max(maxpos[0],tmpmax[0]);
    maxpos[1] = max(maxpos[1],tmpmax[1]);
  }
  
  Vector2f fullsize = maxpos-minpos+(maxpos-minpos)/20.0f;
  fullsize[0] = fabs(fullsize[0]);
  fullsize[1] = fabs(fullsize[1]);

  Vector2f scales(fullsize[0]/(float)width(),fullsize[1]/(float)height());

  _scale  = max(scales[0],scales[1]);
  _offset = minpos;

  Vector2f p1 = remapCoord(width()/2,height()/2);
  Vector2f p2 = remapCoord(0,0);
  Vector2f p3 = remapCoord(width(),height());
  Vector2f p4 = ((p3-p2)-fullsize)/2.0f;

  _offset = _offset+(p1-p2-p4);
  updateGL();
}

void GraphWidget::exposeEvent(QExposeEvent *ev) {
  QWindow::exposeEvent(ev);
  updateGL();
}
