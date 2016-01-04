// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "gaussBlur.h"

GaussBlurNode::GaussBlurNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _p(":/gaussBlur.vert",":/gaussBlur.frag"),
    _w(new GaussBlurWidget(this)) {

    _p.addUniform("img");
    _p.addUniform("direction");
    _p.addUniform("halfsize");
}

void GaussBlurNode::apply() {
  // set viewport 
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  // first pass 
  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  _p.setUniform1i("halfsize",_w->halfsize()->val());
  _p.setUniform1i("direction",0);
  drawOutputs(buffersOfTmpTex(0),1,true,false);

  // second pass 
  _p.setUniformTexture("img",GL_TEXTURE_2D,tmpTex(0)->id());
  _p.setUniform1i("direction",1);
  drawOutputs(buffersOfOutputTex(0),1,false,true);
  _p.disable();
}
