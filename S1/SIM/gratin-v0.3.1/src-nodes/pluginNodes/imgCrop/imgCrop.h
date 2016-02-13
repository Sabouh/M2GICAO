// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCROP_H
#define IMGCROP_H

#include <QObject>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "misc/gpuProgram.h"

class ImgCropNode : public NodeTexture2D {
 public:
  // from nodeInterface
  ImgCropNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);

  inline bool outputSizeEqualInputSize() const {return false;}

  void apply ();
  void reload();
  void init();

  inline NodeWidget *widget() {return _w;}
  
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);
 
 public:
  // from ImageCropNode
  void setImgSize(const Vector2i &size);
  inline const Vector2i currentSize() const {return Vector2i(outputSize()[0],outputSize()[1]);}

 private:
  GPUProgram  _p;
  NodeWidget *_w;
  bool _sizeInitialized;
};


class ImgCropHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.ImgCrop")

 public:
  const QString     stringId() const {return "imgCropId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "cropImage";}
  const QString     path    () const {return "Imaging/";}
  const QString     desc    () const {return "Crop an input image";}
  const QString     help    () const {return tr("Crop an image. "
						"Use the interface to crop and reset input image parameters.");}
  
  const QStringList inputNames () const {return (QStringList() << "image");}
  const QStringList outputNames() const {return (QStringList() << "cropped");}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new ImgCropNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new ImgCropNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						 inputNames,outputNames));
  }
};

#endif // IMGCROP_H
