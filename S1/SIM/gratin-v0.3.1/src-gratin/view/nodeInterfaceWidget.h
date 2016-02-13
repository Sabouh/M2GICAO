// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_INTERFACE_WIDGET_H
#define NODE_INTERFACE_WIDGET_H

#include <QDockWidget>
#include <QToolBox>
#include <Eigen/Dense>

#include "core/sceneIOTags.h"
#include "view/nodeWidget.h"
#include "view/nodeInterfaceContainer.h"

using namespace Eigen;

class GraphWidget;

class NodeInterfaceWidget : public QDockWidget, public IOData {
  Q_OBJECT

public:
  // this widget is a singleton
  static NodeInterfaceWidget *instance(GraphWidget *graphWidget=NULL,QWidget *parent=NULL) {
    if(!_instance) {
      _instance = new NodeInterfaceWidget(graphWidget,parent);
    }
    return _instance;
  } 

  ~NodeInterfaceWidget();

  void highlightWidget(NodeWidget *widget);
  void removeWidget(NodeWidget *widget);
  void updateIds();
  
  bool save(SceneSaver  *) {return true;}
  bool load(SceneLoader *) {return true;}

 protected:
  void keyPressEvent(QKeyEvent *event);

 private:
  NodeInterfaceWidget(GraphWidget *graphWidget,QWidget *parent=0);
  static NodeInterfaceWidget *_instance;

  int  indexOfWidget(NodeWidget *widget);
  bool isExternalWidget(NodeWidget *widget);

  GraphWidget *_graphWidget;
  //QToolBox    *_widgets;
  NodeInterfaceContainer *_widgets;
};

#endif // NODE_INTERFACE_WIDGET_H
