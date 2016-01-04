// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/textDrawer.h"

GPUProgram        *TextDrawer::_ptext       = NULL;
FloatTexture2D    *TextDrawer::_tfont       = NULL;
VertexarrayObject *TextDrawer::_vao         = NULL;
int                TextDrawer::_nbInstances = 0;
Vector2f           TextDrawer::_cellSize    = Vector2f(0,0);

void TextDrawer::initGPUData() {
  _ptext = new GPUProgram(":/text.vert",":/text.frag");
  _tfont = Glutils::qimageToTexture(":/font.png",true);
  _cellSize = Vector2f(_tfont->w()/_nb,_tfont->h()/_nb);

  _ptext->addUniform("mvp");
  _ptext->addUniform("tcolor");
  _ptext->addUniform("bcolor");
  _ptext->addUniform("font");

  _vao = new VertexarrayObject();
  _vao->addAttrib(2*sizeof(Vector2f),NULL,2,0,GL_STREAM_DRAW);
  _vao->addAttrib(2*sizeof(Vector2f),NULL,2,0,GL_STREAM_DRAW);
}

void TextDrawer::clearGPUData() {
  delete _ptext; _ptext = NULL;
  delete _tfont; _tfont = NULL;
  delete _vao;   _vao   = NULL;
}

TextDrawer::TextDrawer() {
  if(_nbInstances<=0) {
    initGPUData();
    _nbInstances = 0;
  }
  
  _nbInstances++;
}

TextDrawer::~TextDrawer() {
  _nbInstances--;
  if(_nbInstances<=0) {
    clearGPUData();
  }
}
