// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_WIDGET
#define NODE_WIDGET

#include <QWidget>
#include <assert.h>

#include "misc/extinclude.h"
#include "core/curve.h"
#include "core/sceneIOTags.h"

class NodeInterface;

class LIBRARY_EXPORT NodeWidget : public QWidget, public IOData {
 public:
 NodeWidget(NodeInterface *node,QWidget *parent=NULL);
  
 virtual ~NodeWidget();
  
 inline virtual bool keyframed() const {return false;}
 inline virtual void computeNewFrameValues();
 inline virtual bool currentValuesHaveChanged();
 inline virtual bool saveUserDefinedWidgets(SceneSaver  *) {return true;}
 inline virtual bool loadUserDefinedWidgets(SceneLoader *) {return true;}
 
 const Vector2i nodeIds() const;
 const QString nodeName() const;
 const QString nodeNameId() const;
 const QStringList nodeNamesId() const;

 // update graph only if parameters have changed 
 inline void updateGraphCond();
 
 // update the graph starting from this node 
 void updateGraph();
 
 // load and save 
 virtual bool save(SceneSaver  *saver );
 virtual bool load(SceneLoader *loader);
 
 inline const std::vector<NodeWidget *> &childrenWidgets() const {return _childrenWidgets;}
 inline NodeInterface *node() {return _node;}

 protected:
 void addChildWidget(NodeWidget *widget);
 void delChildWidget(NodeWidget *widget);
 
 private:
 NodeInterface *_node;
 std::vector<NodeWidget *> _keyframedWidgets;
 std::vector<NodeWidget *> _childrenWidgets;
};

inline void NodeWidget::computeNewFrameValues() {
  for(unsigned int i=0;i<_keyframedWidgets.size();++i) {
    _keyframedWidgets[i]->computeNewFrameValues();
  }
}
//#include <iostream>
inline bool NodeWidget::currentValuesHaveChanged() {
  //std::cout << "widget " << nodeName().toStdString() << std::endl;
  bool changed = false;
  for(unsigned int i=0;i<_keyframedWidgets.size();++i) {
    //std::cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << std::endl;
    //std::cout << "           -> testing " << _keyframedWidgets[i]->nodeName().toStdString() << std::endl;
    if(_keyframedWidgets[i]->currentValuesHaveChanged())
      changed = true;
  }

  return changed;
}

inline void NodeWidget::updateGraphCond() {
  computeNewFrameValues();
  if(currentValuesHaveChanged()) 
    updateGraph();
}

#endif // NODE_WIDGET
