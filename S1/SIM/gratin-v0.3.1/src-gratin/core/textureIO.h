// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXTURE_IO
#define TEXTURE_IO

#include "misc/extinclude.h"
#include "misc/texture2D.h"

#include <QString>
#include <QStringList>
#include <QDir>
#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QDialogButtonBox>

class LIBRARY_EXPORT TextureIO : public GlContext {
 public:
  TextureIO();
  ~TextureIO();

  const QString askForSaveParams();
  const QString askForLoadParams();
  bool save(const QString &filename,FloatTexture2D *tex);
  FloatTexture2D *load(const QString &filename);

  inline const QString &path() const {return _path;}
  inline const QString &name() const {return _name;}
  inline const QString fullname() const {return _path+"/"+_name;}
  inline void setClampColors(bool clampColors) {_clampColors = clampColors;}
  inline void setUseAlpha(bool useAlpha) {_useAlpha = useAlpha;}
  inline bool clampColors() const {return _clampColors;}
  inline bool useAlpha() const {return _useAlpha;}

 private:
  QString _path;
  QString _name;
  bool    _clampColors;
  bool    _useAlpha;
  static QDir        _currentPath;
  static QStringList _formats;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class ImageSettings : public QDialog  {
  Q_OBJECT
    
    public:
  ImageSettings(TextureIO *textureIO);
  inline bool clamp() const {return _clamp->checkState()==Qt::Checked;}
  inline bool alpha() const {return _alpha->checkState()==Qt::Checked;}

  public slots:
  void confirmClicked();
  void cancelClicked();

 private:
  TextureIO           *_textureIO;
  QGroupBox           *_box;
  QCheckBox           *_clamp;
  QCheckBox           *_alpha;
  QDialogButtonBox    *_buttons;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // TEXTURE_IO
