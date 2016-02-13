// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgCropWidget.h"
#include "imgCrop.h"
#include "core/textureIO.h"
#include <QVBoxLayout>

#define MAX_TEX_SIZE 8192

ImgCropWidget::ImgCropWidget(NodeInterface *node)
  : NodeWidget(node) {
  
  _origImageSize = ((ImgCropNode *)node)->currentSize();
  _currentSize   = _origImageSize;

  Vector2i maxSize(max(_origImageSize[0],MAX_TEX_SIZE),max(_origImageSize[1],MAX_TEX_SIZE));
  Vector2i minSize(1,1);

  _default  = new QPushButton("Reset");
  _cropBox  = new QGroupBox("Crop");
  _cropSize = new Vector2iSpinWidget(node,"Size",minSize,maxSize,_origImageSize,false);
  _cropPos  = new Vector2iSpinWidget(node,"Position",-maxSize,maxSize,Vector2i(0,0),true);

  QVBoxLayout *l;

  l = new QVBoxLayout();
  l->addWidget(_cropPos);
  l->addWidget(_cropSize);
  _cropBox->setLayout(l);

  l = new QVBoxLayout();
  l->addWidget(_cropBox);
  l->addWidget(_default);
  l->addStretch(1);
  setLayout(l);

  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));
  connect(_cropSize,SIGNAL(valChanged()),this,SLOT(cropSizeChanged()));

  addChildWidget(_cropSize);
  addChildWidget(_cropPos);
}

void ImgCropWidget::defaultClicked() {
  _cropSize->setVal(_origImageSize,false);
  _cropPos->setVal(Vector2i(0,0),false);
  updateGraph();
}

void ImgCropWidget::cropSizeChanged() {
  _currentSize = _cropSize->val();
  ((ImgCropNode *)node())->setImgSize(_currentSize);
}

void ImgCropWidget::imgSizeChanged(const Vector2i &orig,const Vector2i &dim) {
  _origImageSize = orig;
  const Vector2i tmp  = dim==Vector2i(0,0) ? orig : dim;
  _cropSize->setVal(tmp,false);
}
