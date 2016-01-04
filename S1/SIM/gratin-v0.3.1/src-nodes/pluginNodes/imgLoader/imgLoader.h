// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGLOADER_H
#define IMGLOADER_H

#include <QObject>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "misc/gpuProgram.h"

class ImgLoaderNode : public NodeTexture2D {
 public:
  // from nodeInterface
  ImgLoaderNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~ImgLoaderNode();

  void apply ();
  void reload();

  inline NodeWidget *widget() {return _w;}
  
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);
 
 public:
  // from ImageLoaderNode
  void loadImage(const QString &filename,const Vector2i &size=Vector2i(0,0),bool keepSize=false);
  void setImgSize(const Vector2i &size);
  inline FloatTexture2D *tex() {return _tex;}

 private:
  GPUProgram      _p;
  QString         _filename;
  FloatTexture2D *_tex;
  NodeWidget     *_w;
};

class ImgLoaderHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.ImgLoader")

 public:
  const QString     stringId() const {return "imgLoaderId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "imageLoader";}
  const QString     path    () const {return "Imaging/";}
  const QString     desc    () const {return "Image loader";}
  const QString     help    () const {return tr("Load an image.",
						"Use the interface to load, resize and reset image parameters.");}
  
  const QStringList inputNames () const {return QStringList();}
  const QStringList outputNames() const {return (QStringList() << "image");}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new ImgLoaderNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new ImgLoaderNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};



#endif // IMGLOADER_H
