// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgResize.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "core/textureIO.h"
#include "imgResizeWidget.h"

using namespace std;

ImgResizeNode::ImgResizeNode(PbGraph *parent,NodeHandle *handle)
: NodeTexture2D(parent,handle),
  _p(":/imgResize.vert",":/imgResize.frag"),
  _w(new ImgResizeWidget(this)),
  _sizeInitialized(false) {
  _p.addUniform("img");
}

void ImgResizeNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  drawOutputs(buffersOfOutputTex(0));
  _p.disable();
}

void ImgResizeNode::init() {
  ImgResizeWidget *w  = (ImgResizeWidget *)widget();

  FloatTexture2D *t = inputTex(0);
  if(!_sizeInitialized) {
    setOutputSize(Vector2f(t->w(),t->h()));
    w->imgSizeChanged(Vector2i(t->w(),t->h()),Vector2i(0,0));
  } else {
    w->imgSizeChanged(Vector2i(t->w(),t->h()),w->scale()->val());
  }
  _sizeInitialized = true;
}

bool ImgResizeNode::save(SceneSaver  *saver) {
  ImgResizeWidget *w = (ImgResizeWidget *)widget();
  return saver->saveVector2i("size",w->scale()->val()); 
}

bool ImgResizeNode::load(SceneLoader *loader) {
  _sizeInitialized = true;
  Vector2i size(0,0);
  bool ret = loader->loadVector2i("size",size);
  setImgSize(size);
  return ret;
}

void ImgResizeNode::setImgSize(const Vector2i &size) {
  setOutputSize(Vector2f((float)size[0],(float)size[1]));
}

void ImgResizeNode::reload() {
  _p.reload();
}
