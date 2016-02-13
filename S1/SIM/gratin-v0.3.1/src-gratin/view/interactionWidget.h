// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INTERACTION_WIDGET_H
#define INTERACTION_WIDGET_H

#include "misc/glwindow.h"
#include "misc/extinclude.h"
#include "core/sceneIOTags.h"
#include "core/nodeInterface.h"

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QWindow>
#include <QSurfaceFormat>
#include <QScreen>

class GraphWidget;

class GlInteractionWidget : public GlWindow, public IOData {
  Q_OBJECT

 public:
  enum {NO_MOTION=0,OFFSET_MOTION=1,SCALING_MOTION=2};

  GlInteractionWidget(const QSurfaceFormat &format,QScreen *screen=NULL);
  ~GlInteractionWidget();

  bool containNode(NodeInterface *node,unsigned int output);
  void addNode    (NodeInterface *node,unsigned int output);
  void removeNode (NodeInterface *node,unsigned int output);
  void removeNode (NodeInterface *node);
  void zoom(int type);
  void viewAll();
  void render();

  // load and save 
  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  public slots:
  void updateGL();

  protected slots:
  void resizeGL();

 protected :
  void initializeGL();
  void exposeEvent(QExposeEvent *ev);

 protected :
  virtual void mousePressEvent(QMouseEvent *me);
  virtual void mouseMoveEvent(QMouseEvent *me);
  virtual void mouseReleaseEvent(QMouseEvent *me);
  virtual void wheelEvent(QWheelEvent *we);
  virtual void keyPressEvent(QKeyEvent *ke);
  virtual void keyReleaseEvent(QKeyEvent *ke);

 private:
  int contain(NodeInterface *node,unsigned int output);
  inline const Vector2f remapCoord(int xin,int yin);
  inline int currentSelection(const Vector2f &pos);
  inline void swapNodes(int id1,int id2);

  std::vector<NodeInterface *> _nodes;
  std::vector<unsigned int   > _outputs;
  std::vector<Vector2f       > _positions;

  float       _scale;
  int         _interaction;
  Vector2f    _mouseCoord;
  Vector2f    _mouseTmp;
  Vector2f    _offset;
  bool        _onClick;
  int         _currentNode;
  int         _tmpNode;
  TextDrawer *_textDrawer;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline const Vector2f GlInteractionWidget::remapCoord(int xin,int yin) {
  return Vector2f(_scale*(xin-width()/2.0f)+_offset[0],
		  _scale*(yin-height()/2.0f)+_offset[1]);
}

inline int GlInteractionWidget::currentSelection(const Vector2f &pos) {
  for(unsigned int i=0;i<_nodes.size();++i) {
      const Vector2f s = _nodes[i]->outputContentSize(_outputs[i]);
      if(pos[0]>=_positions[i][0] &&
         pos[0]<=_positions[i][0]+s[0] &&
         pos[1]>=_positions[i][1] &&
         pos[1]<=_positions[i][1]+s[1]) {
        return i;
      }
  }
  return -1;
}

inline void GlInteractionWidget::swapNodes(int id1,int id2) {

  NodeInterface *tmpNode = _nodes    [id1];
  unsigned int tmpOutput = _outputs  [id1];
  Vector2f tmpPosition   = _positions[id1];
  
  _nodes    [id1] = _nodes[id2];
  _outputs  [id1] = _outputs[id2];
  _positions[id1] = _positions[id2];
  
  _nodes    [id2] = tmpNode;
  _outputs  [id2] = tmpOutput;
  _positions[id2] = tmpPosition;
}

#endif // INTERACTION_WIDGET_H
