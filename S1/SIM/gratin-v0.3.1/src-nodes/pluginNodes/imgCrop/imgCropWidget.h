// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCROP_WIDGET
#define IMGCROP_WIDGET

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"

class ImgCropWidget : public NodeWidget {
  Q_OBJECT

    public:
  ImgCropWidget(NodeInterface *node);

  void imgSizeChanged(const Vector2i &orig,const Vector2i &dim=Vector2i(0,0));

  inline Vector2iSpinWidget *cropSize() {return _cropSize;}
  inline Vector2iSpinWidget *cropPos()  {return _cropPos;}

  public slots:
  void defaultClicked();
  void cropSizeChanged();

 private:
  QPushButton         *_default;
  QGroupBox           *_cropBox;
  Vector2iSpinWidget  *_cropPos;
  Vector2iSpinWidget  *_cropSize;
  Vector2i             _origImageSize;
  Vector2i             _currentSize;
};

#endif // IMGCROP_WIDGET
