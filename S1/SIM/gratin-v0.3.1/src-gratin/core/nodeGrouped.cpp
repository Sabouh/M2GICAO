// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "nodeGrouped.h"
#include "core/nodeManager.h"

#include <QVBoxLayout>
using namespace std;

const QString NodeGrouped::GROUPED_NODE_ID  = "_GRATIN_GROUPED_NODES_ID_";
const unsigned int NodeGrouped::GROUPED_NODE_VERSION  = 1;


NodeGroupedWidget::NodeGroupedWidget(NodeInterface *node) 
  : NodeWidget(node),
    _t(new QTabWidget()),
    _keyframed(false) {
  
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(_t);
  setLayout(l);
}

NodeGroupedWidget::~NodeGroupedWidget() {
  _t->setParent(0);
}

void NodeGroupedWidget::addTab(NodeWidget *page,const QString &label) {
  if(!page) return;

  _t->addTab((QWidget *)page,label);

  if(page->keyframed()) 
    _keyframed = true;

  addChildWidget(page);

}

void NodeGroupedWidget::clear() {
  _t->clear();
}



NodeGrouped::NodeGrouped(PbGraph *parent,
			 const QString &name,
			 const QString &path,
			 const QString &desc,
			 const QString &help,
			 const QStringList &inputNames,
			 const QStringList &outputNames)
  : PbGraph(),
    NodeInterface(parent,new NodeHandle(GROUPED_NODE_ID,
					GROUPED_NODE_VERSION,
					name,
					path,
					desc,
					help,
					inputNames,
					outputNames)),
    _locked(false),
    //_nbInputs(0),
    //_nbOutputs(0),
    _mode(0),
    _w(new NodeGroupedWidget(this)) {
}

NodeGrouped::~NodeGrouped() {

}

bool NodeGrouped::addNode(NodeInterface *node) {
  if(_locked) return false;
  return PbGraph::addNode(node);
}

bool NodeGrouped::removeNode(NodeInterface *node,bool clean) {
  if(_locked) return false;
  return PbGraph::removeNode(node,clean);
}

bool NodeGrouped::makeConnection(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId) {
  if(_locked) return false;
  return PbGraph::makeConnection(src,outputId,dst,inputId);
}

bool NodeGrouped::removeConnection(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId) {
  if(_locked) return false;
  return PbGraph::removeConnection(src,outputId,dst,inputId);
}

const vector<pair<NodeInterface *,Vector2u> > NodeGrouped::inputNodeData(NodeID nodeId) {
  vector<pair<NodeInterface *,Vector2u> > idataG = PbGraph::inputNodeData(nodeId); // nodeGrouped connections
  vector<pair<NodeInterface *,Vector2u> > idataP = _parent->inputNodeData(_nodeId);// parent connections

  for(unsigned int i=0;i<_inodes.size();++i) {
    // look for this node in the input node list
    if(_inodes[i]->nodeId()!=nodeId) continue;

    // internal input connection for this node 
    const unsigned int input = _iconnect[i];
    
    // look if one of the external output connection corresponds... 
    for(unsigned int j=0;j<idataP.size();++j) {
      const Vector2u c1 = idataP[j].second;
      if(c1[1]==i) { // ... to the current external  input connection 
	// create the mapping (with the internal node input connection)
	const Vector2u c2(c1[0],input); 
	idataG.push_back(pair<NodeInterface *,Vector2u>(idataP[j].first,c2));
	break;
      }
    }
  }
  
  return idataG;
}

void NodeGrouped::lock() {
  _locked=true;
  initConnections();
  initWidget();
  updatePositions();
}

void NodeGrouped::unlock() {
  _locked=false;
  clearConnections();
  clearWidget();
}

// void NodeGrouped::initDefaultPositions() {

// }

void NodeGrouped::updatePositions() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->updatePositions();
  }
  
  NodeInterface::updatePositions();
}

void NodeGrouped::initEmptyData(const Vector2f &pos) {
  cleanData();
  _pos = pos;

  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->initEmptyData(node(i)->pos());
  }

  initDefaultPositions();
}

void NodeGrouped::cleanData() {
  _outputData.clear();
  _inputData.clear();
}

void NodeGrouped::cleanEverything() {

  unlock();
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->cleanEverything();
    delete node(i);
  }

  cleanData();
  delete _w;_w = NULL;
}

void NodeGrouped::setInputData(const vector<pair<NodeInterface *,Vector2u> > &) {
  // prepareForProcessing(false,false,false);
  // _state = nbNodes()>0 ? node(0)->state() : NO_CHANGE;

  prepareForProcessing(false,false,true);
  _state = NO_CHANGE;

  _ready = true;

  //cout << __FILE__ << " - " << __LINE__ << " - group : state = " << _state << ", nb nodes = " << nbNodes() << endl;

  for(unsigned int i=0;i<nbNodes();++i) {
    //cout << __FILE__ << " - " << __LINE__ << " - group : node state = " << node(i)->state() << endl;
    if(!node(i)->isReady()) {
      _ready = false;
      //return;
    }
  }

  //if(isReady())
  updatePositions();

}

void NodeGrouped::reloadInputData() {
  NodeInterface::reloadInputData();
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->reloadInputData();
  }
}

void NodeGrouped::initOutputData() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->initOutputData();
  }

  updatePositions();
}

void NodeGrouped::cleanOutputData() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->cleanOutputData();
  }
}

bool NodeGrouped::save(SceneSaver  *saver) {
  bool ret = true;
  //if(!saver->saveUint("nbInputs",_nbInputs))   ret = false;
  //if(!saver->saveUint("nbOutputs",_nbOutputs)) ret = false;
  if(!PbGraph::save(saver))                    ret = false;
  return ret;
}

bool NodeGrouped::load(SceneLoader *loader) {
  bool ret = true;

  if(_mode==IO_NODEGROUPED) {

    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;

    unlock();

    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;
    //if(!loader->loadUint("nbInputs",_nbInputs))   ret = false;
    //if(!loader->loadUint("nbOutputs",_nbOutputs)) ret = false;
    if(!loadNodeParams(loader))                   ret = false;
    if(!loadNodeConnections(loader))              ret = false;
    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;

    lock();
    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;

  } else {
    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;

    prepareForProcessing(false,false);
    if(!loadNodeData(loader)) ret = false;
    //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << endl;
  }
  
  return ret;
}

void NodeGrouped::initWidget() {
  _w->clear();
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->widget()) {
      _w->addTab(node(i)->widget(),node(i)->nameId());
    }
  }
}

void NodeGrouped::clearWidget() {
  _w->clear();
}

void NodeGrouped::initConnections() {
  clearConnections();
  initInputConnections();
  initOutputConnections();
  initHandleIO();
  initDrawingBuffers();
}

void NodeGrouped::initInputConnections() {
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->nbInputs()==0) continue;
    
    vector<bool> connected(node(i)->nbInputs(),false);
    
    for(unsigned int j=0;j<nbNodes();++j) {
      for(unsigned int k=0;k<_adjacencyMatrix[j][i].size();++k) {
	connected[_adjacencyMatrix[j][i][k][1]] = true;
      }
    }

    for(unsigned int j=0;j<node(i)->nbInputs();++j) {
      if(!connected[j]) {
	_inodes.push_back(node(i));
	_iconnect.push_back(j);
      }
    }
  }

  //_nbInputs = _inodes.size();
}

void NodeGrouped::initOutputConnections() {
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->nbOutputs()==0) continue;
    
    vector<bool> connected(node(i)->nbOutputs(),false);
    
    for(unsigned int j=0;j<nbNodes();++j) {
      for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {
	connected[_adjacencyMatrix[i][j][k][0]] = true;
      }
    }

    for(unsigned int j=0;j<node(i)->nbOutputs();++j) {
      if(!connected[j]) {
	_onodes.push_back(node(i));
	_oconnect.push_back(j);
      }
    }
  }

  //_nbOutputs = _onodes.size();
}

void NodeGrouped::initHandleIO() {
  QStringList li,lo;
  for(unsigned int i=0;i<_inodes.size();++i) {
    const QStringList n = _inodes[i]->inputNames();
    li.push_back(n[_iconnect[i]]);
  }

  for(unsigned int i=0;i<_onodes.size();++i) {
    const QStringList n = _onodes[i]->outputNames();
    lo.push_back(n[_oconnect[i]]);
  }

  setInputNames(li);
  setOutputNames(lo);
}

void NodeGrouped::clearConnections() {
  _inodes.clear();
  _onodes.clear();
  _iconnect.clear();
  _oconnect.clear();
}

void NodeGrouped::getConnections(vector<pair<NodeInterface *,NodeInterface *> > &connectedNodes,
                             vector<Vector2u> &connectedOutputsInputs) const {
  connectedNodes.clear();
  connectedOutputsInputs.clear();
  for(NodeID i=0;i<nbNodes();++i) {
      for(unsigned int j=0;j<nbNodes();++j) {
          for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {
              connectedNodes.push_back(pair<NodeInterface *,NodeInterface *>(_topologicalList[i],_topologicalList[j]));
              connectedOutputsInputs.push_back(_adjacencyMatrix[i][j][k]);
          }
      }
  }
}

const QStringList NodeGrouped::graphNames() {
  QStringList l = _parent->graphNames();
  l << nameId();
  return l;
}

int NodeGrouped::inputNumber(NodeInterface *node,unsigned int input) {
  for(unsigned int i=0;i<_inodes.size();++i) {
    if(_inodes[i]==node && _iconnect[i]==input) {
      return i;
    }
  }
  return -1;
}

int NodeGrouped::outputNumber(NodeInterface *node,unsigned int output) {
  for(unsigned int i=0;i<_onodes.size();++i) {
    if(_onodes[i]==node && _oconnect[i]==output) {
      return i;
    }
  }
  return -1;
}

std::pair<NodeInterface *,unsigned int> NodeGrouped::nodeFromInputNumber(unsigned int number) {
  if(number<_inodes.size()) {
    return std::pair<NodeInterface *,unsigned int>(_inodes[number],_iconnect[number]);
  }
  return std::pair<NodeInterface *,unsigned int>(NULL,0);
}

std::pair<NodeInterface *,unsigned int> NodeGrouped::nodeFromOutputNumber(unsigned int number) {
  if(number<_onodes.size()) {
    return std::pair<NodeInterface *,unsigned int>(_onodes[number],_oconnect[number]);
  }
  return std::pair<NodeInterface *,unsigned int>(NULL,0);
}
