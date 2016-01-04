// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGRESIZE_WIDGET
#define IMGRESIZE_WIDGET

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"

class ImgResizeWidget : public NodeWidget {
  Q_OBJECT

    public:
  ImgResizeWidget(NodeInterface *node);

  void imgSizeChanged(const Vector2i &orig,const Vector2i &scale=Vector2i(0,0));

  inline Vector2iSpinWidget *scale() {return _scaleSize;}

  public slots:
  void defaultClicked();
  void scaleSizeChanged();

 private:
  QPushButton        *_default;
  QGroupBox          *_scaleBox;
  QCheckBox          *_aspectRatio;
  Vector2iSpinWidget *_scaleSize;
  Vector2i            _origImageSize;
  Vector2i            _currentScale;
};

#endif // IMGRESIZE_WIDGET
