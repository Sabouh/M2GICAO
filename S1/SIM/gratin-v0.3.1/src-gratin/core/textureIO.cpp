// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/textureIO.h"
#include "misc/glutils.h"
#include <QFileDialog>
#include <QVBoxLayout>

using namespace std;

#ifdef EXR_SUPPORT

#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <ImfEnvmap.h>
#include <Iex.h>

QStringList TextureIO::_formats = QStringList() << ".png"  << ".jpg"  << ".exr" << ".bmp" << ".tiff";

#else 
QStringList TextureIO::_formats = QStringList() << ".png"  << ".jpg"  << ".bmp" << ".tiff";
#endif // EXR_SUPPORT

QDir TextureIO::_currentPath = QDir::currentPath();//("data/");

TextureIO::TextureIO() 
  : _path(_currentPath.absolutePath()),
    _name("") {

}

TextureIO::~TextureIO() {

}

const QString TextureIO::askForSaveParams() {
  QString format = "Images ( ";
  for(int i=0;i<_formats.size();++i) {
    format = format + "*" + _formats[i] + " ";
  }
  format = format + ");;All (*.*)";
  
  QString filename = QFileDialog::getSaveFileName(0,"Save Image",_currentPath.absolutePath(),format);

  if(filename.isEmpty()) {
    return filename;
  }

  QDir d(".");

  QStringList l = filename.split("/");
  _currentPath = d.filePath(filename);
  _path = _currentPath.absolutePath();
  _name = l[l.size()-1];


  ImageSettings settings(this);
  int ret = settings.exec();
  if(ret==QDialog::Rejected) {
    return QString(); 
  } else {
    _useAlpha = settings.alpha();
    _clampColors = settings.clamp();
  }

  return filename;
}

const QString TextureIO::askForLoadParams() {
  QString format = "Images (";
  for(int i=0;i<_formats.size();++i) {
    format = format + "*" + _formats[i];
    if(i<_formats.size()-1) {
      format = format + " ";
    }
  }
  format = format + ");;All (*.*)";
  
  QString filename = QFileDialog::getOpenFileName(0,"Load Image",_currentPath.absolutePath(),format);

  if(filename.isEmpty()) {
    return filename;
  }

  QDir d(".");
  QStringList l = filename.split("/");
  _currentPath = d.filePath(filename);
  _path = _currentPath.absolutePath();
  _name =  l[l.size()-1];

  return filename;
}

bool TextureIO::save(const QString &filename,FloatTexture2D *tex) {
  const unsigned int w = tex->w();
  const unsigned int h = tex->h();
  const unsigned int p = 4;

  float *img = new float[w*h*p];
  tex->bind();
  _glf->glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&(img[0]));

  if(_clampColors) {
    for(unsigned int i=0;i<w*h*p;++i) {
      img[i] = max(min(img[i],1.0f),0.0f);
    }
  }

  if(!_useAlpha) {
    for(unsigned int y=0;y<h;++y) {
      for(unsigned int x=0;x<w;++x) {
	img[p*w*y+p*x+3] = 1.0f;
      }
    }
  }

#ifdef EXR_SUPPORT 

  QStringList l = filename.split(".");

  if(l[l.size()-1]=="exr" || l[l.size()-1]=="EXR") {
    unsigned int i=0;
    Imf::Rgba *pixels = (Imf::Rgba *)malloc(w*h*sizeof(Imf::Rgba));
    
    for(unsigned int y=0;y<h;++y) {
      for(unsigned int x=0;x<w;++x) {

	pixels[i].r = img[p*w*(h-y-1)+p*x   ];
	pixels[i].g = img[p*w*(h-y-1)+p*x+1];
	pixels[i].b = img[p*w*(h-y-1)+p*x+2];
	pixels[i].a = img[p*w*(h-y-1)+p*x+3];

	++i;
      }
    }
    
    Imf::RgbaOutputFile file(filename.toStdString().c_str(),w,h,Imf::WRITE_RGBA);
    file.setFrameBuffer(pixels,1,w);
    file.writePixels(h);
    
    free(pixels);
    delete [] img;
    return true;
  }

#endif // EXR_SUPPORT

  float *pix = &(img[0]);
  
  QImage image((int)(w),(int)(h),QImage::Format_ARGB32);
  unsigned int k=0;
  for(int i=0;i<image.height();++i) {
    for(int j=0;j<image.width();++j) {
      image.setPixel(j,image.height()-1-i,qRgba(pix[k]*255,pix[k+1]*255,pix[k+2]*255,pix[k+3]*255));
      k += 4;
    }
  }

  delete [] img;   
  return image.save(filename);
}

FloatTexture2D *TextureIO::load(const QString &filename) {
#ifdef EXR_SUPPORT 

  QStringList l = filename.split(".");

  if(l[l.size()-1]=="exr" || l[l.size()-1]=="EXR") {
    // we found an EXR file
    Imf::RgbaInputFile file(filename.toStdString().c_str());
    Imf::Array2D<Imf::Rgba> pixels;

    Imath::Box2i dw = file.dataWindow();
    int width = dw.max.x - dw.min.x + 1;
    int height = dw.max.y - dw.min.y + 1;

    pixels.resizeErase(height,width);
    file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    file.readPixels(dw.min.y,dw.max.y);

    //float img[width][height][4];
    float *img = new float[width*height*4];

    for(int y=0;y<height;++y) {
      for(int x=0;x<width;++x) {
	const Imf::Rgba& src = pixels[y][x];
	const unsigned int index = 4*width*(height-1-y)+4*x;
	img[index+0] = src.r;
	img[index+1] = src.g;
	img[index+2] = src.b;
	img[index+3] = src.a;
      }
    }
    
    FloatTexture2D *r = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,width,height,Glutils::defFormat(),GL_RGBA,GL_FLOAT),TextureParams(Glutils::defFilter(),Glutils::defFilter()),&(img[0]));
    delete [] img;
    return r;
  }
#endif // EXR_SUPPORT 

  return Glutils::qimageToTexture(filename);
}

ImageSettings::ImageSettings(TextureIO *textureIO) 
  : _textureIO(textureIO) {
  _box = new QGroupBox("Image settings");
  _clamp = new QCheckBox("Clamp values in [0,1]?");
  _alpha = new QCheckBox("Use Alpha if possible?");
  _buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  if(_textureIO->clampColors()) {
    _clamp->setCheckState(Qt::Checked);
  } else {
    _clamp->setCheckState(Qt::Unchecked);
  }

  if(_textureIO->useAlpha()) {
    _alpha->setCheckState(Qt::Checked);
  } else {
    _alpha->setCheckState(Qt::Unchecked);
  }

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addWidget(_clamp);
  vbox->addWidget(_alpha);
  vbox->addWidget(_buttons);
  _box->setLayout(vbox);
  vbox = new QVBoxLayout();
  vbox->addWidget(_box);
  setLayout(vbox);

  setWindowModality(Qt::WindowModal);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));
}

void ImageSettings::confirmClicked() {
  accept();
}

void ImageSettings::cancelClicked() {
  reject();
}
