// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/nodeInterface.h"
#include "core/pbgraph.h"
#include "core/nodeHandle.h"
#include "misc/glutils.h"

using namespace std;

VertexarrayObject *NodeInterface::_rectangleVao  = NULL;
VertexarrayObject *NodeInterface::_unitSquareVao = NULL;
GPUProgram        *NodeInterface::_pconnect      = NULL;
GPUProgram        *NodeInterface::_pstyle        = NULL;
TextDrawer        *NodeInterface::_textDrawer    = NULL;
int                NodeInterface::_nbInstances   = 0;

void NodeInterface::initGPUData() {
  _pconnect      = new GPUProgram(":/point.vert",":/point.frag");
  _pstyle        = new GPUProgram(":/node-style.vert",":/node-style.frag");
  _textDrawer    = new TextDrawer();
  _rectangleVao  = new VertexarrayObject();
  _unitSquareVao = new VertexarrayObject();

  _pconnect->addUniform("mvp");
  _pconnect->addUniform("size");
  _pconnect->addUniform("color");
  _pstyle->addUniform("mvp");
  _pstyle->addUniform("backgroundcol");
  _pstyle->addUniform("linecol");
  _pstyle->addUniform("selected");
  _pstyle->addUniform("roundness");

  vector<Vector2f> boxesPts(6);
  vector<Vector2f> boxesCos(6);

  boxesPts[0]  = Vector2f(-1,-1);
  boxesPts[1]  = Vector2f(1,-1);
  boxesPts[2]  = Vector2f(1,1);
  boxesPts[3]  = Vector2f(1,1);
  boxesPts[4]  = Vector2f(-1,1);
  boxesPts[5]  = Vector2f(-1,-1);
  
  boxesCos[0]  = Vector2f(0.0f,0.0f);
  boxesCos[1]  = Vector2f(1.0f,0.0f);
  boxesCos[2]  = Vector2f(1.0f,1.0f);
  boxesCos[3]  = Vector2f(1.0f,1.0f);
  boxesCos[4]  = Vector2f(0.0f,1.0f);
  boxesCos[5]  = Vector2f(0.0f,0.0f);

  // the rectangle points will be the only thing that might change at every frame
  _rectangleVao->addAttrib(6*sizeof(Vector2f) ,boxesPts[0].data(),2,0,GL_DYNAMIC_DRAW);
  _rectangleVao->addAttrib(6*sizeof(Vector2f) ,boxesCos[0].data(),2,0,GL_STATIC_DRAW);
  _unitSquareVao->addAttrib(6*sizeof(Vector2f),boxesPts[0].data(),2,0,GL_STATIC_DRAW);
  _unitSquareVao->addAttrib(6*sizeof(Vector2f),boxesCos[0].data(),2,0,GL_STATIC_DRAW);
}

void NodeInterface::clearGPUData() {
  delete _rectangleVao;     _rectangleVao     = NULL;
  delete _unitSquareVao;    _unitSquareVao    = NULL;
  delete _pconnect;         _pconnect         = NULL;
  delete _pstyle;           _pstyle           = NULL;
  delete _textDrawer;       _textDrawer       = NULL;
}

NodeInterface::NodeInterface(PbGraph *parent,NodeHandle *handle)
  : _nodeId(0),
    _graphId(0),
    _parent(parent),
    _handle(handle),
    _ready(false),
    _state(NO_CHANGE),
    _pos(Vector2f(0,0)),
    _size(50.0f,250.0f),
    _selected(false) {

  if(_nbInstances<=0) {
    initGPUData();
    _nbInstances = 0;
  }
  
  _drawVao = new VertexarrayObject();
  _drawVao->addAttrib(2*sizeof(Vector2f),NULL,2,0,GL_DYNAMIC_DRAW);
  _drawVao->addAttrib(2*sizeof(Vector2f),NULL,2,0,GL_DYNAMIC_DRAW);
  initDrawingBuffers();

  _nbInstances++;
}

NodeInterface::~NodeInterface() {
  _nbInstances--;
  if(_nbInstances<=0) {
    clearGPUData();
  }
  delete _drawVao;
  delete _handle;
}

void NodeInterface::initDrawingBuffers() {
  const unsigned int nb = nbInputs()+nbOutputs()+nbOutputs()*6+6;
  if(nb==0) return;

  std::vector<Vector2f> coords(nb,Vector2f(0.0f,0.0f));
  unsigned int j = nbInputs()+nbOutputs();

  for(unsigned int i=0;i<(nbOutputs()+1);++i) {
    coords[j++]  = Vector2f(0.0f,0.0f);
    coords[j++]  = Vector2f(1.0f,0.0f);
    coords[j++]  = Vector2f(1.0f,1.0f);
    coords[j++]  = Vector2f(1.0f,1.0f);
    coords[j++]  = Vector2f(0.0f,1.0f);
    coords[j++]  = Vector2f(0.0f,0.0f);
  }

  _drawVao->setAttrib(0,nb*sizeof(Vector2f),NULL,2,0,GL_DYNAMIC_DRAW);
  _drawVao->setAttrib(1,nb*sizeof(Vector2f),coords[0].data(),2,0,GL_DYNAMIC_DRAW);
}


// the vector is composed of <NodeInterface *,<outputId,inputId>>
void NodeInterface::setInputData(const vector<pair<NodeInterface *,Vector2u> > &dinputs) {

  // place the data inside an ordered array
  std::vector<Data *> newData(nbInputs(),NULL);
  for(unsigned int i=0;i<dinputs.size();++i) {
      unsigned int inputId  = (dinputs[i].second)[1];
      unsigned int outputId = (dinputs[i].second)[0];

      NodeInterface *node   = dinputs[i].first;
      newData[inputId] = node->output(outputId);
  }

  //setInputData(newData);

  int state = NO_CHANGE;

  // place the data inside an ordered array
  std::vector<Data *> currentData = inputs();
  const unsigned int inputDataSize = static_cast<unsigned int>(currentData.size());

  // old/new state
  bool currentState = _ready;
  bool newState     = checkInputData(newData);

  // check if the state of the node changes or not
  if     ( currentState &&  newState) state = READY_TO_READY;
  else if(!currentState && !newState) state = NOT_READY_TO_NOT_READY;
  else if( currentState && !newState) state = READY_TO_NOT_READY;
  else if(!currentState &&  newState) state = NOT_READY_TO_READY;

  if(currentData.size()==newData.size() && (state==READY_TO_READY || state==NOT_READY_TO_NOT_READY)) {
      // check if everything is the same
      bool same = true;
      for(unsigned int i=0;i<newData.size();++i) {
	if(!newData[i] || !(newData[i]->sameAs(currentData[i]))) {
	  same = false;
	  break;
	}
      }
      if(same) state = NO_CHANGE;
  }

  _ready = newState;

  if(newData.size()!=inputDataSize)
    initEmptyData();

  // change the data
  for(unsigned int i=0;i<nbInputs();++i) {
    if(newData[i]) input(i)->copy(newData[i]);
  }

  // update outputs
  if(state==READY_TO_NOT_READY || state==READY_TO_READY)
      cleanOutputData();
  if(state==READY_TO_READY || state==NOT_READY_TO_READY)
      initOutputData();

  _state = state;

  initDrawingBuffers();
}

bool NodeInterface::isPartlyConnected() {
  return parentGraph() ? parentGraph()->isNodePartlyConnected(this) : false;
}

void NodeInterface::initDefaultPositions() {
  const Vector2f defsize(512,512);

  for(unsigned int i=0;i<nbOutputs();++i) {
    output(i)->setSize(defsize);
  }

  updatePositions();
}

void NodeInterface::updatePositions() {
  // compute the total size (max(x), sum(y))
  Vector2f total(0.0f,0.0f);
  for(unsigned int i=0;i<nbOutputs();++i) {
    const Vector2f s = outputContentSize(i);
    total[0] = max(total[0],s[0]);
    total[1] = total[1]+s[1];
  }
  total[1] = total[1]/(float)nbOutputs();

  // compute the new size for each outputs
  Vector2f offsetContent(total/10.0f);
  total = Vector2f(0.0f,0.0f);
  for(unsigned int i=0;i<nbOutputs();++i) {
    output(i)->setSize(outputContentSize(i)+offsetContent);
    total += outputSize(i);
  }


  // compute the new size for the node and update output positions
  total = total/(float)nbOutputs();
  Vector2f offsetOutput(total/10.0f);
  Vector2f currentPos(Vector2f(offsetOutput[0],offsetOutput[1]/2.0f));
  _size[0] = total[0]+offsetOutput[0]*2.0f;  

  for(int i=(int)nbOutputs()-1;i>=0;--i) {
    output(i)->setPos(currentPos);
    currentPos[1] += outputSize(i)[1]+offsetOutput[1];
  }
  currentPos[1] += offsetOutput[1]/2.0f;
  _size[1] = currentPos[1];
}

void NodeInterface::reloadInputData() {
  vector<pair<NodeInterface *,Vector2u> > inputs = _parent->inputNodeData(nodeId());
  for(unsigned int i=0;i<inputs.size();++i) {
    unsigned int inputId  = (inputs[i].second)[1];
    unsigned int outputId = (inputs[i].second)[0];
    NodeInterface *node   = inputs[i].first;
    input(inputId)->copy(node->output(outputId));
  }
}

void NodeInterface::initEmptyData(const Vector2f &pos) {
  cleanData();
  _pos = pos;

  _inputData  = vector<Data *>(nbInputs());
  _outputData = vector<Data *>(nbOutputs());

  for(unsigned int i=0;i<nbInputs();++i) {
    _inputData[i] = new Data();
  }

  for(unsigned int i=0;i<nbOutputs();++i) {
    _outputData[i] = new Data();
  }

  initDefaultPositions();
}

void NodeInterface::cleanData() {
  for(unsigned int i=0;i<_inputData.size();++i) {
      delete _inputData[i];
      _inputData[i] = NULL;
  }

  for(unsigned int i=0;i<_outputData.size();++i) {
    delete _outputData[i];
    _outputData[i] = NULL;
  }
  
  _outputData.clear();
  _inputData.clear();
}

void NodeInterface::cleanEverything() {
  cleanData();

  delete widget();
}

void NodeInterface::sendEvent(const NodeEvent &event) {
  _parent->sendEventFrom(this,event);
}

const QString NodeInterface::stringIdVersion() const { return _handle->stringIdVersion(); }
const QString NodeInterface::stringId       () const { return _handle->stringId(); }
const QString NodeInterface::name           () const { return _handle->name(); }
const QString NodeInterface::path           () const { return _handle->path(); }
const QString NodeInterface::desc           () const { return _handle->desc(); }
const QString NodeInterface::help           () const { return _handle->help(); }
const QStringList NodeInterface::inputNames () const { return _handle->inputNames(); }
const QStringList NodeInterface::outputNames() const { return _handle->outputNames(); }
unsigned int NodeInterface::nbInputs        () const { return _handle->nbInputs();}
unsigned int NodeInterface::nbOutputs       () const { return _handle->nbOutputs();}
unsigned int NodeInterface::version         () const { return _handle->version();}

void NodeInterface::setStringId   (const QString &stringId       ) { _handle->setStringId(stringId);       }
void NodeInterface::setVersion    (unsigned int version          ) { _handle->setVersion(version);         }
void NodeInterface::setName       (const QString &name           ) { _handle->setName(name);               }
void NodeInterface::setPath       (const QString &path           ) { _handle->setPath(path);               }
void NodeInterface::setDesc       (const QString &desc           ) { _handle->setDesc(desc);               }
void NodeInterface::setHelp       (const QString &help           ) { _handle->setHelp(help);               }
void NodeInterface::setInputNames (const QStringList &inputNames ) { _handle->setInputNames(inputNames);   }
void NodeInterface::setOutputNames(const QStringList &outputNames) { _handle->setOutputNames(outputNames); }


void NodeInterface::update() {
  _parent->applyFrom(this);
  _parent->refreshView();
}

const QStringList NodeInterface::namesId() {
  QStringList l = _parent->graphNames();
  l << nameId();
  return l;
}



DefaultNode::DefaultNode(PbGraph *parent,const QString &name,const QString &path,
			 const QStringList &inames,const QStringList &onames)
  : NodeInterface(parent,new NodeHandle("default",0,name,path,"","",inames,onames)) {
}
