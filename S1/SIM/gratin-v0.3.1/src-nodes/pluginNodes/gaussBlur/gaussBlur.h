// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GAUSSBLUR_H
#define GAUSSBLUR_H

#include <QVBoxLayout>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"

// **** a widget containing a keyframed slider ****
class GaussBlurWidget : public NodeWidget {
 public:
 GaussBlurWidget(NodeInterface *node) :
  NodeWidget(node),_halfsize(new IntSliderWidget(node,"size",0,500,0)) {
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(_halfsize);
    setLayout(l);
    addChildWidget(_halfsize);
  }
  
  inline IntSliderWidget *halfsize() const {return _halfsize;}

 private:
  IntSliderWidget *_halfsize;
};




// **** the node itself, containing the shader and the widget ****
class GaussBlurNode : public NodeTexture2D {
 public:
  GaussBlurNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();

  inline NodeWidget  *widget() {return _w;} 

 protected:
  // need 1 tmp texture (because of the 2 passes)
  inline unsigned int nbTmps()  const {return 1; } 

 private:
  GPUProgram       _p;
  GaussBlurWidget *_w;
};




// **** the node handle, containing information displayed in the interface ****
class GaussBlurHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
    Q_INTERFACES(NodeHandleInterface)
    Q_PLUGIN_METADATA(IID "Gratin.GaussBlur")

    public:
  const QString     stringId() const {return "imgGaussianBlurId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "gaussianBlur";}
  const QString     path    () const {return "Imaging/";}
  const QString     desc    () const {return "Gaussian blur";}
  const QString     help    () const {return tr("The amount of blur is controled by the kernel size.\n"
						"Increase this size to increase the blur");}
  
  const QStringList inputNames () const {return QStringList() << "image"; }
  const QStringList outputNames() const {return (QStringList() << "filtering");}
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new GaussBlurNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new GaussBlurNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};

#endif // GAUSSBLUR_H
