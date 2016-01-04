// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgLoader.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "core/textureIO.h"
#include "imgLoaderWidget.h"

using namespace std;

ImgLoaderNode::ImgLoaderNode(PbGraph *parent,NodeHandle *handle)
: NodeTexture2D(parent,handle),
  _p(":/imgLoader.vert",":/imgLoader.frag"),
  _filename(GRATIN_APPLI_DATA_PATH+"/imgs/default-tex.jpg"),
  _tex(Glutils::qimageToTexture(_filename)),
  _w(new ImgLoaderWidget(this)) {
  _p.addUniform("img");

}

ImgLoaderNode::~ImgLoaderNode() {
  delete _tex;
}

void ImgLoaderNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,_tex->id());
  drawOutputs(buffersOfOutputTex(0));
  _p.disable();
}

void ImgLoaderNode::loadImage(const QString &filename,const Vector2i &size,bool keepSize) {
  makeCurrent();
  TextureIO loader;
  FloatTexture2D *t = loader.load(filename);
  const Vector2f origSize = outputSize();//(_tex->w(),_tex->h());

  if(t) {
    delete _tex;
    _tex = t;
    _p.setUniformTexture("img",GL_TEXTURE_2D,_tex->id());
    QDir d(".");
    _filename = d.absoluteFilePath(filename);
    if(size==Vector2i(0,0)) 
      if(keepSize)
	setOutputSize(origSize);
      else
	setOutputSize(Vector2f(t->w(),t->h()));
    else
      setOutputSize(Vector2f((float)size[0],(float)size[1]));

    if(!keepSize)
      ((ImgLoaderWidget *)widget())->imgSizeChanged(Vector2i(t->w(),t->h()),size);
  }
}

bool ImgLoaderNode::save(SceneSaver  *saver) {
  ImgLoaderWidget *w = (ImgLoaderWidget *)widget();

  QString name = saver->absoluteToRelativePath(_filename);

  // cout << __FILE__ << " --> saving..." << endl;
  // cout << "   the relative directory is : " << saver->savingPath().toStdString() << endl;
  // cout << "   the original filename is  : " << _filename.toStdString() << endl;
  // cout << "   the new relative name is  : " << name.toStdString() << endl;

  return (saver->saveString("filename",name) && 
	  saver->saveVector2i("size",w->scale()->val())); 
}

bool ImgLoaderNode::load(SceneLoader *loader) {
  _filename = GRATIN_APPLI_DATA_PATH+"/imgs/default-tex.jpg";
  Vector2i size(0,0);
  
  bool ret = true;
  QString name;
  if(!loader->loadString("filename",name)) ret = false;

  if(ret) {
    _filename = loader->relativeToAbsolutePath(name);
    
    // cout << __FILE__ << " --> loading..." << endl;
    // cout << "   the relative directory is : " << loader->loadingPath().toStdString() << endl;
    // cout << "   the old relative name is  : " << name.toStdString() << endl;
    // cout << "   the original filename is  : " << _filename.toStdString() << endl;
  } 

  if(!loader->loadVector2i("size",size)) ret = false;

  loadImage(_filename,size);
  return ret;
}

void ImgLoaderNode::setImgSize(const Vector2i &size) {
  setOutputSize(Vector2f((float)size[0],(float)size[1]));
}

void ImgLoaderNode::reload() {
  _p.reload();
}

