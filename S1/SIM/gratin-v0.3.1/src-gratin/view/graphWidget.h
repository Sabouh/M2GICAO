// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GRAPH_WIDGET_H
#define GRAPH_WIDGET_H

#include "misc/glwindow.h"
#include "misc/extinclude.h"
#include "misc/vertexarrayObject.h"
#include "view/interactionWidget.h"
#include "view/nodeInterfaceWidget.h"
#include "view/nodeListWidget.h"
#include "view/animationWidget.h"
#include "view/logWidget.h"
#include "core/sceneIOTags.h"
#include "core/nodeManager.h"
#include "core/pbgraph.h"
#include "core/animation.h"

#include <QWindow>
#include <QString>
#include <QImage>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSurfaceFormat>
#include <QScreen>

class GraphWidget : public GlWindow, public IOData {
  Q_OBJECT

 public:
  enum {NO_MOTION,OFFSET_MOTION,SCALING_MOTION,CONNECT_MOTION,SELECT_MOTION,MULTIPLE_SELECT_MOTION};
  enum {NOTHING_HAS_BEEN_CLICKED,A_NODE_HAS_BEEN_CLICKED,AN_OUTPUT_HAS_BEEN_CLICKED};

  GraphWidget(const QSurfaceFormat &format,QScreen *screen=NULL);
  ~GraphWidget();

  // widgets associated to the graph
  void setInteractionWidget(GlInteractionWidget *interactionWidget);
  void setDockInteractionWidget(QDockWidget *dockInteractionWidget);
  void setNodeInterfaceWidget(NodeInterfaceWidget *nodeInterfaceWidget);
  void setNodeListWidget(NodeListWidget *nodeListWidget);
  void setAnimationWidget(AnimationWidget *animationWidget);

  // common operations 
  void addNode(const QString &name,const QString &path);
  void group();
  void ungroup();
  void addNodeToList();
  void manageNodePaths();
  void saveNodeContent();
  void saveAnimNodeContent();
  void reloadShaders();
  void applyNodes();
  void selectUnselectAll();
  void clear();
  void copy();
  void paste();
  void zoom(int type);
  void viewAll();
  void render();

  // animation tools 
  inline Animation *animation();
  inline void animationSettingsChanged();
  inline void animationFrameChanged();
  inline void nodeCurveChanged(NodeInterface *node);
  inline bool graphIsEmpty() const;

  // load and save 
  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  // remove this node from all the interaction widgets
  void removeFromInteractionWidgets(NodeInterface *node);

  public slots:
  void updateGL();
  void updateGLForce();

  protected slots:
  void resizeGL();

 protected :
  void initializeGL();
  void exposeEvent(QExposeEvent *ev);

  virtual void mousePressEvent(QMouseEvent *me);
  virtual void mouseMoveEvent(QMouseEvent *me);
  virtual void mouseReleaseEvent(QMouseEvent *me);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void keyPressEvent(QKeyEvent *ke);
  virtual void keyReleaseEvent(QKeyEvent *ke);
  virtual void wheelEvent(QWheelEvent *we);

 private:
  inline const Vector2f remapCoord(int xin,int yin);
  inline bool outputNodeClicked(const Vector2f &coord);
  inline bool inputNodeClicked(const Vector2f &coord);
  inline bool nodeClicked(const Vector2f &coord);
  inline bool inputConnected(const Vector2i &inNode);
  inline bool clickedNodeSelected();
  inline bool everythingIsSelected();
  inline bool nodeSelected(unsigned int i);
  inline void selectAll();
  inline void unselectAll();
  inline void selectClickedNode();
  inline void selectNodeOutput();
  inline void unselectNodeOutputs();
  inline void unselectClickedNode();
  inline void selectSelection();
  inline void unselectSelection();
  inline void moveSelection(const Vector2f &offset);
  inline void removeSelection();

  void removeNode(NodeInterface *node);
  void drawConnection(const Matrix4f &mvp);
  void drawSelection(const Matrix4f &mvp);
  void drawGraphConnections(const Matrix4f &mvp);
  void graphStateChanged();
  void updateConnectionBuffer();
  void updateSelectionBuffer();
  void updateGraphBuffer();
  void setGraphBuffer();

    // widgets
  NodeInterfaceWidget *_nodeInterfaceWidget;
  NodeListWidget      *_nodeListWidget;
  GlInteractionWidget *_interactionWidget;
  AnimationWidget     *_animationWidget;
  QDockWidget         *_dockInteractionWidget;
  // data
  NodeManager *_nodeManager;
  PbGraph     *_pbGraph;
  Animation   *_animation;

  float        _scale;
  int          _interaction;
  int          _clickState;
  Vector2f     _mouseCoord;
  Vector2f     _mouseTmp;
  Vector2f     _offset;
  bool         _ctrl;
  bool         _shift;
  bool         _onClick;
  //bool         _glConnected;
  Vector2i     _outNodeSelection;
  Vector2i     _inNodeSelection;

  // gpu data
  GPUProgram         *_lineShader; 
  GPUProgram         *_pointShader; 
  VertexarrayObject  *_interactVao;
  VertexarrayObject  *_connectionVao;
  unsigned int        _nbConnections;

  // IO data 
  SceneSaver  *_copier;
  SceneLoader *_paster;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline const Vector2f GraphWidget::remapCoord(int xin,int yin) {
  return Vector2f(_scale*(xin-width()/2.0f)+_offset[0],
		  _scale*(yin-height()/2.0f)+_offset[1]);
}

inline void GraphWidget::unselectAll() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    _pbGraph->node(i)->setSelected(false);
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      _pbGraph->node(i)->output(j)->setSelected(false);
    }
  }
}

inline void GraphWidget::selectAll() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    _pbGraph->node(i)->setSelected(true);
  }
}

inline bool GraphWidget::nodeSelected(unsigned int i) {
  if(i>=_pbGraph->nbNodes()) return false;
  return _pbGraph->node(i)->selected();
}

inline bool GraphWidget::outputNodeClicked(const Vector2f &coord) {
  const float size = 15.0f*_scale;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      const Vector2f dcoord = coord-_pbGraph->node(i)->outputNodeCoord(j);

      if(dcoord.norm()<size) {
	_outNodeSelection = Vector2i(i,j);
	return true;
      }
    }
  }
  
  return false;
}

inline bool GraphWidget::inputNodeClicked(const Vector2f &coord) {
  const float size = 15.0f*_scale;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    for(unsigned int j=0;j<_pbGraph->node(i)->nbInputs();++j) {
      const Vector2f dcoord = coord-_pbGraph->node(i)->inputNodeCoord(j);

      if(dcoord.norm()<size) {
	_inNodeSelection = Vector2i(i,j);
	return true;
      }
    }
  }
  
  return false;
}

inline bool GraphWidget::inputConnected(const Vector2i &inNode) {
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();
  const unsigned int inode  = (unsigned int)inNode[0];
  const unsigned int iinput = (unsigned int)inNode[1];
  
  for(unsigned int i=0;i<m.size();++i) {
    for(unsigned int k=0;k<m[i][inode].size();++k) {
      const Vector2u c = m[i][inode][k];
      
      if(c[1]==iinput) {
	_outNodeSelection = Vector2i(i,c[0]);
	return true;
      }
    }
  }
  
  return false;
}

inline bool GraphWidget::nodeClicked(const Vector2f &coord) {
  for(int i=(int)_pbGraph->nbNodes()-1;i>=0;--i) {

    // check for the point inside a node 
    if(_pbGraph->node(i)->pointInNode(coord)) {
      // get node index 
      _outNodeSelection = Vector2i(i,-1);
      for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
	
	// check for the point inside the output of a node 
	if(_pbGraph->node(i)->pointInOutput(coord,j)) {
	  _outNodeSelection[1] = j;
	}
      }
      return true;
    } 
  }
  
  return false;
}

inline bool GraphWidget::everythingIsSelected() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(!_pbGraph->node(i)->selected()) 
      return false;
  }
  return true;
}

inline bool GraphWidget::clickedNodeSelected() {
  return _pbGraph->node(_outNodeSelection[0])->selected();
}

inline void GraphWidget::selectClickedNode() {
  _pbGraph->node(_outNodeSelection[0])->setSelected(true);
}

inline void GraphWidget::unselectClickedNode() {
  _pbGraph->node(_outNodeSelection[0])->setSelected(false);
}

inline void GraphWidget::selectNodeOutput() {
  if(_outNodeSelection[1]>=0)
    _pbGraph->node(_outNodeSelection[0])->output(_outNodeSelection[1])->setSelected(true);
}

inline void GraphWidget::unselectNodeOutputs() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      _pbGraph->node(i)->output(j)->setSelected(false);
    }
  }
}


inline void GraphWidget::moveSelection(const Vector2f &offset) {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    
    if(_pbGraph->node(i)->selected()) 
      _pbGraph->node(i)->offsetPos(offset);
    
  }
}

inline void GraphWidget::selectSelection() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {

    if(_pbGraph->node(i)->nodeInSquare(_mouseCoord,_mouseTmp)) 
      _pbGraph->node(i)->setSelected(true);

  }
}

inline void GraphWidget::unselectSelection() {
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    
    if(_pbGraph->node(i)->nodeInSquare(_mouseCoord,_mouseTmp)) 
      _pbGraph->node(i)->setSelected(false);
    
  }
}

inline void GraphWidget::removeSelection() {
  // first copy nodes to avoid side effects 
  std::vector<NodeInterface *> nodes;
  
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    nodes.push_back(_pbGraph->node(i));
  }
  
  for(unsigned int i=0;i<nodes.size();++i) {
    if(nodes[i]->selected()) 
      removeNode(nodes[i]);

  }
}

inline Animation *GraphWidget::animation() {
  return _animation;
}

inline void GraphWidget::animationSettingsChanged() {
  _animationWidget->animationSettingsChanged();
}

inline void GraphWidget::animationFrameChanged() {
  _pbGraph->sendEventAll(NodeEvent(NodeEvent::FRAME_HAS_CHANGED));
  _pbGraph->applyAllWithCond(NodeCondition(NodeCondition::PARAMETERS_CHANGED));
  LogWidget::instance()->setText("Current frame: "+QString::number(_animation->currentFrame()));

  updateGL();
}

/* inline void GraphWidget::nodeCurveChanged(unsigned int id) { */
/*   _pbGraph->sendEventFrom(_pbGraph->node(id),NodeEvent(NodeEvent::FRAME_HAS_CHANGED)); */
/*   _pbGraph->applyFromWithCond(_pbGraph->node(id),NodeCondition(NodeCondition::PARAMETERS_CHANGED)); */
/*   updateGL(); */
/* } */

inline void GraphWidget::nodeCurveChanged(NodeInterface *node) {
  NodeInterface *n = _pbGraph->rootNode(node);
  _pbGraph->sendEventFrom(n,NodeEvent(NodeEvent::FRAME_HAS_CHANGED));
  _pbGraph->applyFromWithCond(n,NodeCondition(NodeCondition::PARAMETERS_CHANGED));
  updateGL();
}

inline bool GraphWidget::graphIsEmpty() const {
  return _pbGraph->nbNodes()==0;
}

#endif // GRAPH_WIDGET_H
