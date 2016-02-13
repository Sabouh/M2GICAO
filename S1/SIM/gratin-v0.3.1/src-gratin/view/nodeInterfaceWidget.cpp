// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/nodeInterfaceWidget.h"
#include "view/graphWidget.h"

#include <QScrollArea>

using namespace std;

NodeInterfaceWidget *NodeInterfaceWidget::_instance = NULL;

NodeInterfaceWidget::NodeInterfaceWidget(GraphWidget *graphWidget,QWidget *parent)
  : QDockWidget("Node parameters",parent),
    _graphWidget(graphWidget) {

  QScrollArea *scrollbar = new QScrollArea();

  scrollbar->setWidgetResizable(true);

  //_widgets = new QToolBox();
  _widgets = new NodeInterfaceContainer();

  scrollbar->setWidget(_widgets);
  setWidget(scrollbar);
}

NodeInterfaceWidget::~NodeInterfaceWidget() {
  // remove all widgets, without destroying them
  while(_widgets->count()>0) {
    removeWidget((NodeWidget *)_widgets->widget(0));
  }
}

void NodeInterfaceWidget::removeWidget(NodeWidget *widget) {
  // no widget for this node... 
  if(!widget) return;

 
  if(isExternalWidget(widget)) {
    // the node might be shown in the external window
    _widgets->removeExternalWidget();
  } else {
    // remove only if this node is in the list
    int index = indexOfWidget(widget);
    if(index>=0) {
      _widgets->removeItem(index);
    }
  }
}

void NodeInterfaceWidget::highlightWidget(NodeWidget *widget) {
  // no widget for this node... 
  if(!widget) return;
  
  if(isExternalWidget(widget)) {
    // the node might be shown in the external window
    _widgets->showExternalWidget();
  } else {
    // if the widget does not exists, add it
    int index = indexOfWidget(widget);
    if(index<0) {
      index = _widgets->addItem(widget,widget->nodeNameId());
    }
    
    // highlight 
    _widgets->setCurrentIndex(index);
  }
}

void NodeInterfaceWidget::updateIds() {
  for(int i=0;i<_widgets->count();++i) {
    NodeWidget *w = (NodeWidget *)(_widgets->widget(i));
    _widgets->setItemText(i,w->nodeNameId());
  }

  NodeWidget *w = (NodeWidget *)(_widgets->externalWidget());
  if(w) {
    _widgets->setExternalItemText(w->nodeNameId());
  }
}

int NodeInterfaceWidget::indexOfWidget(NodeWidget *widget) {
  const Vector2i ids = widget->nodeIds();

  // compare node ids to be sure that this is the same node 
  for(int i=0;i<_widgets->count();++i) {
    NodeWidget *w = (NodeWidget *)(_widgets->widget(i));
    if(w->nodeIds()==ids) {
      return i;
    }
  }

  return -1;
}

bool NodeInterfaceWidget::isExternalWidget(NodeWidget *widget) {
  NodeWidget *w = (NodeWidget *)(_widgets->externalWidget());
   if(!w || !widget) return false;

   return w->nodeIds()==widget->nodeIds();
}

void NodeInterfaceWidget::keyPressEvent(QKeyEvent *event) {
  if(event->key()==Qt::Key_Delete) {
    removeWidget((NodeWidget *)(_widgets->currentWidget()));
  } else {
    QDockWidget::keyPressEvent(event);
  }
}

