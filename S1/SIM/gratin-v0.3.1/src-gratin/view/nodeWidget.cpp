// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/nodeWidget.h"
#include "core/nodeInterface.h"
#include <iostream>

using namespace std;

NodeWidget::NodeWidget(NodeInterface *node,QWidget *parent)
  : QWidget(parent),
    _node(node) {
  assert(_node!=NULL);
}

NodeWidget::~NodeWidget() {
}

void NodeWidget::updateGraph() {
  _node->update();
}

const Vector2i NodeWidget::nodeIds() const {
  return Vector2i((int)_node->nodeId(),(int)_node->graphId());
}

const QString NodeWidget::nodeName() const {
  return _node->name();
}

const QString NodeWidget::nodeNameId() const {
  return _node->nameId();
}

const QStringList NodeWidget::nodeNamesId() const {
  return _node->namesId();
}

void NodeWidget::addChildWidget(NodeWidget *widget) {
  if(!widget) return;

  // do not add if already present
  for(unsigned int i=0;i<_childrenWidgets.size();++i) {
    if(_childrenWidgets[i]==widget) {
      return;
    }
  }

  _childrenWidgets.push_back(widget);
  if(widget->keyframed()) 
    _keyframedWidgets.push_back(widget);
}

void NodeWidget::delChildWidget(NodeWidget *widget) {
  if(!widget) return;

  for(unsigned int i=0;i<_childrenWidgets.size();++i) {
    if(_childrenWidgets[i]==widget) {
      _childrenWidgets.erase(_childrenWidgets.begin()+i);
      break;
    }
  }

  for(unsigned int i=0;i<_keyframedWidgets.size();++i) {
    if(_keyframedWidgets[i]==widget) {
      _keyframedWidgets.erase(_keyframedWidgets.begin()+i);
      break;
    }
  }
}

bool NodeWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveUint("nbChildren",static_cast<unsigned int>(_childrenWidgets.size())))
    ret = false;

  for(unsigned int i=0;i<_childrenWidgets.size();++i) {
    if(!saver->saveIOData(Tags::attribId("childWidget",i),_childrenWidgets[i])) 
      ret = false;
  }
  if(!saveUserDefinedWidgets(saver)) 
    ret = false;
  
  return ret;
}

bool NodeWidget::load(SceneLoader *loader) {
  bool ret = true;
  unsigned int nb = 0;

  if(!loader->loadUint("nbChildren",nb)) ret = false;
  if(nb!=_childrenWidgets.size())        ret = false;

  for(unsigned int i=0;i<nb;++i) {
    if(!loader->loadIOData(Tags::attribId("childWidget",i),_childrenWidgets[i]))
      ret = false;
  }

  if(!loadUserDefinedWidgets(loader)) 
    ret = false;

  return ret;
}
