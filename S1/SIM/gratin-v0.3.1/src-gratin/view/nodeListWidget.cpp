// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/nodeListWidget.h"
#include "view/graphWidget.h"

#include <QStringList>

using namespace std;

NodeListWidget *NodeListWidget::_instance = NULL;

NodeListWidget::NodeListWidget(GraphWidget *graphWidget,QWidget *parent)
  : QDockWidget("Nodes",parent),
    _graphWidget(graphWidget),
    _tree(new QTreeWidget()) {

  setWidget(_tree);
  setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
  connect(_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(addNodeClicked(QTreeWidgetItem *,int)));
}

NodeListWidget::~NodeListWidget() {
  cleanTree();
}

void NodeListWidget::setNodeList(const vector<NodeHandleInterface *> nodeHandles) {
  cleanTree();
  _nodeHandles = nodeHandles;
  makeTree();
}


void NodeListWidget::cleanTree() {
  _tree->clear();
}

void NodeListWidget::makeTree() {

  for(unsigned int i=0;i<_nodeHandles.size();++i) {
    addNode(_nodeHandles[i]);
  }

  _tree->setHeaderLabels(QStringList() << "Name" << "Description");
}

void NodeListWidget::addNode(NodeHandleInterface *handle) {
  // get name and path
  QString name = handle->name();
  QString path = handle->path();
  QString desc = handle->desc();
  
  // remove all spaces
  name.remove(QChar(' '));
  path.remove(QChar(' '));
  
  // check slash at the end of the path
  if(path.at(path.size()-1)!=QChar('/'))
    path = path.append("/");
  
  // build the whole path+name
  QString fullname = path+name;
  
  QStringList t = fullname.split("/");
  makeTreeRec(_tree,t,handle);
}

void NodeListWidget::addNodeList(NodeHandleInterface *handle) {
  addNode(handle);
  _nodeHandles.push_back(handle);
}

void NodeListWidget::delNodeList(NodeHandleInterface *handle) {
  if(!handle) return;

  bool found = false;
  for(unsigned int i=0;i<_nodeHandles.size();++i) {
    if(_nodeHandles[i]->stringId()==handle->stringId() && 
       _nodeHandles[i]->version()==handle->version()) {
      _nodeHandles.erase(_nodeHandles.begin()+i);
      found = true;
      break;
    }
  }

  if(!found) return;

  QList<QTreeWidgetItem *> items = _tree->findItems(handle->name(),Qt::MatchFixedString | Qt::MatchRecursive);
  NodeHandleWidgetItem *item = NULL;

  for(int i=0;i<items.size();++i) {
    item = static_cast<NodeHandleWidgetItem *>(items[i]);
    
    if(item->childCount()==0 && 
       item->handle() && 
       item->handle()->stringId()==handle->stringId() && 
       item->handle()->version()==handle->version()) 
      break;
    
    item = NULL;
  }
  
  if(!item) return;

  deleteItem(item);
}

void NodeListWidget::deleteItem(QTreeWidgetItem *item) {  
  // special case of top level items
  if(_tree->topLevelItem(_tree->indexOfTopLevelItem(item))==item) {
    QTreeWidgetItem *itemToDelete = _tree->takeTopLevelItem(_tree->indexOfTopLevelItem(item));
    delete itemToDelete;
    return;
  }

  // children
  QTreeWidgetItem *parent = item->parent();
  const int i = parent->indexOfChild(item);
  QTreeWidgetItem *itemToDelete = parent->takeChild(i);
  delete itemToDelete;

  // if the item has no brothers no need to keep its parent
  if(parent->childCount()==0) {
    deleteItem(parent);
  }
}

void NodeListWidget::makeTreeRec(QTreeWidget *tree,QStringList &path,NodeHandleInterface *handle,QTreeWidgetItem *parent,bool stop) {
  if(path.empty())
    return;

  NodeHandleWidgetItem *item;

  QList<QTreeWidgetItem *> items = QList<QTreeWidgetItem *>();
  if(!stop) items = tree->findItems(path[0],Qt::MatchExactly | Qt::MatchRecursive,0);

  if(!items.empty()) {
    item = static_cast<NodeHandleWidgetItem *>(items[0]);
  } else {
    stop = true;
    item = new NodeHandleWidgetItem();
    item->setText(0,path[0]);

    if(parent) {
      parent->insertChild(parent->childCount(),item);
    } else {
      tree->addTopLevelItem(item);
    }
  }

  path.removeFirst();

  if(path.empty()) {
    item->setNodeHandle(handle);
    item->setText(1,handle->desc());
  }

  makeTreeRec(tree,path,handle,item,stop);
}

void NodeListWidget::addNodeClicked(QTreeWidgetItem *item,int) {
  
  
  NodeHandleWidgetItem *nitem = static_cast<NodeHandleWidgetItem *>(item);
  if(!nitem || !nitem->handle()) return;

  _graphWidget->addNode(nitem->handle()->name(),nitem->handle()->path());
}

