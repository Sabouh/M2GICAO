// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODEGROUPED_H
#define NODEGROUPED_H

#include "core/pbgraph.h"
#include "core/nodeManager.h"
#include "core/nodeInterface.h"
#include "view/nodeWidget.h"
#include <QTabWidget>

class NodeGroupedWidget : public NodeWidget {
 public:
  NodeGroupedWidget(NodeInterface *node);
  ~NodeGroupedWidget();

  inline bool keyframed() const {return _keyframed;}
  void addTab(NodeWidget *page,const QString &label);
  void clear();

 private:
  QTabWidget *_t;
  bool _keyframed;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class NodeGrouped : public PbGraph, public NodeInterface {
  friend class PbGraph;
  friend class NodeManager;

public:
  NodeGrouped(PbGraph *parent=NULL,
	      const QString &name=QString("grouped"),
	      const QString &path=QString("custom/"),
	      const QString &desc=QString("grouped node"),
	      const QString &help=QString("grouped node"),
	      const QStringList &inputNames=QStringList(),
	      const QStringList &outputNames=QStringList());
  virtual ~NodeGrouped();

  static const QString GROUPED_NODE_ID;  // stringId for grouped nodes 
  static const unsigned int GROUPED_NODE_VERSION; // version for grouped nodes 

  // **************************************
  // ****** PART SPECIFIC TO PBGRAPH ******
  // **************************************
 public:
  virtual bool addNode         (NodeInterface *node);
  virtual bool removeNode      (NodeInterface *node,bool clean=true);
  virtual bool makeConnection  (NodeInterface *src,IONodeID outputId,
				NodeInterface *dst,IONodeID inputId);
  virtual bool removeConnection(NodeInterface *src,IONodeID outputId,
				NodeInterface *dst,IONodeID inputId);

  // remove all nodeGrouped functionnalities in the nodeGrouped class
  //virtual NodeGrouped *connexPartToNodeGrouped(NodeInterface *) {return NULL;}
  //virtual NodeGrouped *nodeGroupedToConnexPart(NodeGrouped *)       {return NULL;}

  // remove all nodeGrouped functionnalities in the nodeGrouped class
  virtual NodeGrouped *groupNodes(const std::vector<NodeInterface *> &) {return NULL;} 
  virtual NodeGrouped *ungroupNode(NodeGrouped *)                       {return NULL;}

  // send events between nodes 
  inline virtual void sendEventFrom (NodeInterface *node,const NodeEvent &event);
  inline virtual void sendEventAll(const NodeEvent &event);

  // applying funtions 
  inline virtual void applyFrom (NodeInterface *node);
  inline virtual void applyAll();
  inline virtual void applyAllWithCond(const NodeCondition &condition);

  // reloading
  inline virtual void reloadAll();
  inline virtual void refreshView();
  virtual const QStringList graphNames();

  virtual const std::vector<std::pair<NodeInterface *,Vector2u> > inputNodeData(NodeID nodeId);





  // *********************************************
  // ****** PART SPECIFIC TO NODE INTERFACE ******
  // *********************************************

 public:
  //inline virtual unsigned int nbInputs()  const;
  //inline virtual unsigned int nbOutputs() const;
  inline virtual void apply ();
  inline virtual void reload();
  inline virtual void init  ();
  inline virtual void clean ();
  inline virtual NodeWidget *widget();
  inline virtual void keyPressEvent    (QKeyEvent *          );
  inline virtual void mousePressEvent  (const Vector2f &,QMouseEvent *);
  inline virtual void mouseMoveEvent   (const Vector2f &,QMouseEvent *);
  inline virtual void mouseReleaseEvent(const Vector2f &,QMouseEvent *);
  inline virtual void wheelEvent       (const Vector2f &,QWheelEvent *);
  inline virtual void receiveEvent(const NodeEvent &event);
  inline virtual bool satisfy(const NodeCondition &condition);
  /* inline virtual const QString     name(); */
  /* inline virtual const QString     path(); */
  /* inline virtual const QString     desc(); */
  /* inline virtual const QString     help(); */
  /* inline virtual const QStringList inputDesc(); */
  /* inline virtual const QStringList outputDesc(); */
  //inline virtual bool isReady() const {return _ready;}
  inline virtual bool checkDataType(NodeInterface *inputNode,
  				    unsigned int outputId,unsigned int inputId);
  //inline virtual bool checkInputData(const std::vector<Data *> &data) const;
  //inline virtual void draw(const Matrix4f &mvp=Matrix4f::Identity());
  inline virtual void drawOutputContent(const Matrix4f &mvp=Matrix4f::Identity(),
  					const Vector2f &position=Vector2f(0,0),unsigned int i=0);
  inline virtual const Vector2f outputContentSize(unsigned int i);
  inline virtual Data *input (unsigned int i) const;
  inline virtual Data *output(unsigned int i) const;
  inline virtual const std::vector<Data *> &inputs ();
  inline virtual const std::vector<Data *> &outputs();

  //inline virtual bool isGrouped() const {return true;}

  //virtual void initDefaultPositions();
  virtual void updatePositions();
  virtual void initEmptyData(const Vector2f &pos=Vector2f(0,0));
  virtual void cleanData();
  virtual void cleanEverything();
  virtual void setInputData(const std::vector<std::pair<NodeInterface *,
			    Vector2u> > &dinputs);
  //virtual void setInputData(const std::vector<Data *> &newData);
  virtual void reloadInputData();
  virtual void initOutputData();
  virtual void cleanOutputData();

  virtual bool save(SceneSaver  *);
  virtual bool load(SceneLoader *);


  // **************************************
  // ****** PART SPECIFIC TO NODEGROUPED ******
  // **************************************
 public:
  void lock();   // prepare the node once all the nodes/connections have been added
  void unlock(); // allow the user to add nodes and make connections inside the group
  
  // connections between nodes inside the group
  void getConnections(std::vector<std::pair<NodeInterface *,NodeInterface *> > &connectedNodes,
		      std::vector<Vector2u> &connectedOutputsInputs) const; 
  
  int inputNumber (NodeInterface *node,unsigned int input);
  int outputNumber(NodeInterface *node,unsigned int output);
  std::pair<NodeInterface *,unsigned int> nodeFromInputNumber(unsigned int number);
  std::pair<NodeInterface *,unsigned int> nodeFromOutputNumber(unsigned int number);

 private:
  enum{IO_NODEGROUPED=0,IO_USER_DATA=1};

  void initConnections();
  void clearConnections();
  void initInputConnections();
  void initOutputConnections();
  void initHandleIO();

  void initWidget();
  void clearWidget();

  inline void setIoMode(int mode=0) {_mode = mode;}

  bool         _locked;
  //unsigned int _nbInputs;
  //unsigned int _nbOutputs;
  unsigned int _mode;

  std::vector<NodeInterface *> _inodes;
  std::vector<NodeInterface *> _onodes;
  std::vector<unsigned int>    _iconnect;
  std::vector<unsigned int>    _oconnect;

  NodeGroupedWidget  *_w;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

/* inline unsigned int NodeGrouped::nbInputs()  const { */
/*   return _nbInputs; */
/* } */

/* inline unsigned int NodeGrouped::nbOutputs() const { */
/*   return _nbOutputs; */
/* } */

inline Data *NodeGrouped::input(unsigned int i) const {
  return _inodes[i]->input(_iconnect[i]);
}

inline Data *NodeGrouped::output(unsigned int i) const {
  return _onodes[i]->output(_oconnect[i]);
}

inline const std::vector<Data *> &NodeGrouped::inputs () {
  _inputData = std::vector<Data *>();
  for(unsigned int i=0;i<nbInputs();++i) {
    _inputData.push_back(input(i));
  }
  return NodeInterface::inputs();
}

inline const std::vector<Data *> &NodeGrouped::outputs() {
  _outputData = std::vector<Data *>();
  for(unsigned int i=0;i<nbOutputs();++i) {
    _outputData.push_back(output(i));
  }
  return NodeInterface::outputs();
}

inline void NodeGrouped::apply () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->apply();
  }
}

inline void NodeGrouped::reload() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->reload();
  }
}

inline void NodeGrouped::init  () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->init();
  }
}

inline void NodeGrouped::clean () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->clean();
  }
}

inline NodeWidget *NodeGrouped::widget() {
  return _w;
}

inline void NodeGrouped::keyPressEvent(QKeyEvent *ke) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->keyPressEvent(ke);
  }
}

inline void NodeGrouped::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mousePressEvent(p,me);
  }
}

inline void NodeGrouped::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mouseMoveEvent(p,me);
  }
}

inline void NodeGrouped::mouseReleaseEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mouseReleaseEvent(p,me);
  }
}

inline void NodeGrouped::wheelEvent(const Vector2f &p,QWheelEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->wheelEvent(p,me);
  }
}

inline void NodeGrouped::receiveEvent(const NodeEvent &event) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->receiveEvent(event);
  }

  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    updatePositions();
  }
}

inline bool NodeGrouped::satisfy(const NodeCondition &condition) {
  bool s = false;
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->satisfy(condition))
      s = true;
  }
  return s;
}

/* inline const QString NodeGrouped::name() { */
/*   return _handle ? _handle->name() : QString("Group"); */
/* } */

/* inline const QString NodeGrouped::path() { */
/*   return _handle ? _handle->path() : QString("Groups/"); */
/* } */

/* inline const QString NodeGrouped::desc() { */
/*   return _handle ? _handle->desc() : QString("Group"); */
/* } */

/* inline const QString NodeGrouped::help() { */
/*   return _handle ? _handle->help() : QString("No help defined for this node"); */
/* } */

/* inline const QStringList NodeGrouped::inputDesc() { */
/*   if(_handle) return _handle->inputDesc(); */

/*   QStringList l; */
/*   for(unsigned int i=0;i<_nbInputs;++i) { */
/*     const QStringList tmp = _inodes[i]->inputDesc(); */
    
/*     /\* // TODO: ugly *\/ */
/*     /\* if(_iconnect[i]<tmp.size()) *\/ */
/*     /\*   l.push_back(tmp[_iconnect[i]]); *\/ */
/*     /\* else  *\/ */
/*     /\*   l.push_back("inputName"); *\/ */

/*     l.push_back(tmp[_iconnect[i]]); */
/*   } */
/*   return l; */
/* } */

/* inline const QStringList NodeGrouped::outputDesc() { */
/*   if(_handle) return _handle->outputDesc(); */

/*   QStringList l; */
/*   for(unsigned int i=0;i<_nbOutputs;++i) { */
/*     const QStringList tmp = _onodes[i]->outputDesc(); */
/*     l.push_back(tmp[_oconnect[i]]); */
/*   } */
/*   return l; */
/* } */

//inline bool NodeGrouped::isReady() const {return _ready;}
inline bool NodeGrouped::checkDataType(NodeInterface *inputNode,
			  unsigned int outputId,unsigned int inputId) {
  return _inodes[inputId]->checkDataType(inputNode,outputId,_iconnect[inputId]);
}

/* inline bool NodeGrouped::checkInputData(const std::vector<Data *> &data) const { */
/*   return false; */
/* } */

/* inline void NodeGrouped::draw(const Matrix4f &mvp) { */
/*   NodeInterface::draw(mvp); */

/*   if(!isReady()) return; */

/*   for(unsigned int i=0;i<nbOutputs();++i) { */
/*     const Vector2f o = (outputSize(i)-outputContentSize(i))/2.0f; */
/*     drawOutputContent(mvp,_pos+outputPos(i)+o,i); */
/*   } */
/* } */

inline void NodeGrouped::drawOutputContent(const Matrix4f &mvp,const Vector2f &position,unsigned int i) {
  _onodes[i]->drawOutputContent(mvp,position,_oconnect[i]);
}

inline const Vector2f NodeGrouped::outputContentSize(unsigned int i) {
  return _onodes[i]->outputContentSize(_oconnect[i]);
}

inline void NodeGrouped::sendEventFrom(NodeInterface *,const NodeEvent &event) {
  _parent->sendEventFrom(this,event);
}

inline void NodeGrouped::sendEventAll(const NodeEvent &event) {
  _parent->sendEventAll(event);
}

inline void NodeGrouped::applyFrom(NodeInterface *) {
  _parent->applyFrom(this);
}

inline void NodeGrouped::applyAll() {
  _parent->applyAll();
}

inline void NodeGrouped::applyAllWithCond(const NodeCondition &condition) {
  _parent->applyAllWithCond(condition);
}

inline void NodeGrouped::reloadAll() {
  _parent->reloadAll();
}

inline void NodeGrouped::refreshView() {
  _parent->refreshView();
}


#endif // NODEGROUPED_H
