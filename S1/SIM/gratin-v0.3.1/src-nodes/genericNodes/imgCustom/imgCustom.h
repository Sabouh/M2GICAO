// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCUSTOM_H
#define IMGCUSTOM_H

#include <QObject>
#include "genericCustomNode.h"
#include "genericCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"

class ImgCustomNode;

class ImgCustomWidget : public GenericCustomWidget {
  Q_OBJECT
    
    public:
  ImgCustomWidget(ImgCustomNode *node);
};


class ImgCustomNode : public GenericCustomNode {
 public:
  ImgCustomNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~ImgCustomNode();

  void apply ();

  inline NodeWidget *widget() {return _w;}

  virtual const QString constantVertHead();
  virtual const QString defaultVertBody(); 
  virtual const QString constantFragHead();
  virtual const QString defaultFragBody(); 
  virtual const QString constantTessCHead();
  virtual const QString defaultTessCBody(); 
  virtual const QString constantTessEHead();
  virtual const QString defaultTessEBody(); 
  virtual const QString constantGeomHead(); 
  virtual const QString defaultGeomBody(); 

 private:
  ImgCustomWidget *_w;

};

class ImgCustomHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.ImgCustom")

 public:
  const QString     stringId() const {return "imgGenericId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "genericImage";}
  const QString     path    () const {return "Generic/";}
  const QString     desc    () const {return "Generic node for image processing";}
  const QString     help    () const {
    return tr("This node was designed to create custom shaders\n"
	      "The settings widget allows to modify input, output parameters\n"
	      "as well as the output image sizes.\n\n"
	      "WARNING1: you will be able to modify the number of inputs/outputs\n"
	      "only if the node is entirely disconnected.\n"
	      "WARNING2: input/output names will not be modified if a custom node is created\n"
	      "from this node because these names should be edited directly in the setting widget\n"
	      "WARNING3: modifying the settings of a node will remove all previously\n"
	      "defined keyframes\n\n" 
	      "* Special data allows to modify the output image (using a multiple of\n"
	      "the input size, plus an offset vector\n"
	      "Check use mouse/keyboard if you want to interact with the mouse\n\n"
	      "* Input data allows to add/remove/displace input data (only if disconnected)\n\n"
	      "* Output data allows to add/remove/displace output data (only if disconnected)\n\n"
	      "* Param data allows to add/remove parameters that will automatically be \n"
	      "included in your shader.\n"
	      "For each of them you may choose a proper name, type, and min/max/default values.\n"
	      "Check keyframes if you want your parameter to be accessible\n"
	      " through the animation widget\n\n"
	      "In the node interface itself:\n"
	      "Source tab contains the head and body of your GLSL source (only the body is editable)\n"
	      "Params tab contains the defined widgets according to your setting choices\n"
	      "Log tab contains compilation errors\n\n"
	      "Once your settings and source completed, click on apply to see the result\n"
	      );
  }
  
  const QStringList inputNames () const {return QStringList() << "inBuffer0";}
  const QStringList outputNames() const {return QStringList() << "outBuffer0";}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new ImgCustomNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new ImgCustomNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};


#endif // IMGCUSTOM_H
