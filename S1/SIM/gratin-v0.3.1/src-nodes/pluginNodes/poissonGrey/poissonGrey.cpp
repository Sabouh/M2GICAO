// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "poissonGrey.h"
#include "misc/glutils.h"

using namespace std;

PoissonGreyNode::PoissonGreyNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _w(new PoissonGreyWidget(this)),
    _p(QString(plugDir()+"/textureNodes/poissonGrey/shaders/").toStdString()),
    _initialized(false),
    _debugMode(SOLUTION),
    _debugLevel(0),
    _debugProgram(":/psbw.vert",":/test.frag") {

  _debugProgram.addUniform("t");
}

void PoissonGreyNode::init() {
  _initialized = false;
}

void PoissonGreyNode::apply() {

  if(!_initialized) {
    // we need to reinitialize 
    _p.initialize(inputTex(0),outputTex(0));
    _p.update(true);
    _initialized = true;
  }    

  
  //_p.update(true);
  for(int i=0;i<_w->nbCycles()->val();++i) {
    _p.update(false);
  }  

  //test();
}

void PoissonGreyNode::test() {
  FloatTexture2D *t = NULL;
  switch(_debugMode) {
  case RESIDUAL: t = _p.getResidual(_debugLevel);break;
  case TMP: t = _p.getTmp(_debugLevel);break;
  case SOLUTION: t = _p.getSolution(_debugLevel);break;
  default:break;
  }

  if(!t) return;

  t->bind();
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); 

  _debugProgram.enable();
  _debugProgram.setUniformTexture("t",GL_TEXTURE_2D,t->id());
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  drawOutputs(buffersOfOutputTex(0),1);
  _debugProgram.disable();
}

void PoissonGreyNode::keyPressEvent(QKeyEvent *ke) {
  if(ke->key()==Qt::Key_R) {
    _debugMode = RESIDUAL;
  } else if(ke->key()==Qt::Key_T) {
    _debugMode = TMP;
  } else if(ke->key()==Qt::Key_S) {
    _debugMode = SOLUTION;
  } else if(ke->key()==Qt::Key_L) {
    _debugLevel = min(_debugLevel+1,(int)_p.nbLevels()-1);
  } else if(ke->key()==Qt::Key_M) {
    _debugLevel = max(_debugLevel-1,0);
  } 

  //cout << "MODE: " << _debugMode << " -- LEVEL: " << _debugLevel << endl;
  update();
}

void PoissonGreyNode::clean() {
  _initialized = false;
}

void PoissonGreyNode::reload() {
  _initialized = false;
  _p.reloadShaders();
}

void PoissonGreyNode::receiveEvent(const NodeEvent &event) {
  NodeTexture2D::receiveEvent(event);
  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    _initialized = false;
  } 
}
