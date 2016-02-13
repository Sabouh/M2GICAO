// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_LIST_WIDGET_H
#define NODE_LIST_WIDGET_H

#include "core/nodeHandle.h"
#include "core/sceneIOTags.h"

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

class GraphWidget;

/* class FastListWidget : public QWidget { */
/*   Q_OBJECT */

/*     public: */
/*   ~FastListWidget(); */

/*   // this widget is a singleton */
/*   static FastListWidget *instance() { */
/*     if(!_instance) { */
/*       _instance = new FastListWidget(); */
/*     } */
/*     return _instance; */
/*   }  */

/*   private slots: */
/*   void addNode(int); */

/*  private: */
/*   FastListWidget(); */
/*   static FastListWidget *_instance; */

/*   std::vector<std::pair<QString,QString> > _nodes; */
/*   QLineEdit *_search; */
/*   QComboBox *_results; */
/* }; */

class NodeHandleWidgetItem : public QTreeWidgetItem {
 public:
 NodeHandleWidgetItem() : QTreeWidgetItem(), _handle(NULL) {}
  
  inline void setNodeHandle(NodeHandleInterface *handle) { _handle=handle; }
  inline NodeHandleInterface *handle() {return _handle;}

 private:
  NodeHandleInterface *_handle;
};

class NodeListWidget : public QDockWidget, public IOData {
  Q_OBJECT

 //friend class FastListWidget;
public:
  ~NodeListWidget();

  // this widget is a singleton
  static NodeListWidget *instance(GraphWidget *graphWidget=NULL,QWidget *parent=NULL) {
    if(!_instance) {
      _instance = new NodeListWidget(graphWidget,parent);
    }
    return _instance;
  } 

  void setNodeList(const std::vector<NodeHandleInterface *> nodeHandles);
  void addNodeList(NodeHandleInterface *handle);
  void delNodeList(NodeHandleInterface *handle);

  // load and save 
  bool save(SceneSaver  *) {return true;}
  bool load(SceneLoader *) {return true;}

public slots:
  void addNodeClicked(QTreeWidgetItem *,int);

private:
  NodeListWidget(GraphWidget *graphWidget,QWidget *parent=0);
  static NodeListWidget *_instance;

  GraphWidget *_graphWidget;
  QTreeWidget *_tree;
  std::vector<NodeHandleInterface *> _nodeHandles;

  void addNode(NodeHandleInterface *handle);
  void cleanTree();
  void makeTree();
  void makeTreeRec(QTreeWidget *tree,QStringList &path,NodeHandleInterface *handle,QTreeWidgetItem *parent=NULL,bool stop=false);
  void deleteItem(QTreeWidgetItem *item);
};

#endif // NODE_LIST_WIDGET_H
