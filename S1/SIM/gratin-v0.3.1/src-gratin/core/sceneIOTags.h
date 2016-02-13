// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SCENE_TAGS_H
#define SCENE_TAGS_H

#include "misc/extinclude.h"
#include "core/sceneSaver.h"
#include "core/sceneLoader.h"

#include <QString>

// inherits IOData to save and load specific data in your class 
class LIBRARY_EXPORT IOData {
 public:
  virtual ~IOData() {}
  virtual bool load(SceneLoader *) {return true;}
  virtual bool save(SceneSaver  *) {return true;}

  static int LOAD_MAJOR_VERSION;
  static int LOAD_MINOR_VERSION;
  static int SAVE_MAJOR_VERSION;
  static int SAVE_MINOR_VERSION;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// use Tags to load and save specific data 
class LIBRARY_EXPORT Tags {
 public:
  // root 
  static const QString rootTag() {return GRATIN_NAME;}

  // basic types 
  static const QString boolTag  ()  {return "bool";  }
  static const QString intTag   ()  {return "int";   }
  static const QString uintTag  ()  {return "uint";  }
  static const QString floatTag ()  {return "float"; }
  static const QString doubleTag()  {return "double";}
  static const QString stringTag()  {return "string";}

  // vectors, matrices, quaternions 
  static const QString vector2uTag()  {return "vector2u";}
  static const QString vector3uTag()  {return "vector3u";}
  static const QString vector4uTag()  {return "vector4u";}

  static const QString vector2iTag()  {return "vector2i";}
  static const QString vector3iTag()  {return "vector3i";}
  static const QString vector4iTag()  {return "vector4i";}
  static const QString vectorXiTag()  {return "vectorXi";}

  static const QString vector2fTag()  {return "vector2f";}
  static const QString vector3fTag()  {return "vector3f";}
  static const QString vector4fTag()  {return "vector4f";}
  static const QString vectorXfTag()  {return "vectorXf";}
  
  static const QString vector2dTag()  {return "vector2d";}
  static const QString vector3dTag()  {return "vector3d";}
  static const QString vector4dTag()  {return "vector4d";}
  static const QString vectorXdTag()  {return "vectorXd";}
  
  static const QString matrix2iTag()  {return "matrix2i";}
  static const QString matrix3iTag()  {return "matrix3i";}
  static const QString matrix4iTag()  {return "matrix4i";}
  static const QString matrixXiTag()  {return "matrixXi";}
  
  static const QString matrix2fTag()  {return "matrix2f";}
  static const QString matrix3fTag()  {return "matrix3f";}
  static const QString matrix4fTag()  {return "matrix4f";}
  static const QString matrixXfTag()  {return "matrixXf";}
  
  static const QString matrix2dTag()  {return "matrix2d";}
  static const QString matrix3dTag()  {return "matrix3d";}
  static const QString matrix4dTag()  {return "matrix4d";}
  static const QString matrixXdTag()  {return "matrixXd";}

  static const QString quaternionfTag() {return "quaternionf";}
  static const QString quaterniondTag() {return "quaterniond";}


  // data 
  static const QString ioDataTag  ()  {return "IOData";}

  // attributes 
  static const QString attribType  ()  {return "type";}
  static const QString attribName  ()  {return "name";}
  static const QString attribVal   ()  {return "val";}
  static const QString attribNumber()  {return "nb";}
  static const QString attribId(unsigned int i)  {
    return QString::number(i);}
  static const QString attribId(const QString &name,unsigned int i)  {
    return name+"-"+QString::number(i);}
  static const QString attribId(const QString &name,unsigned int i,unsigned int j)  {
    return name+"-"+QString::number(i)+"-"+QString::number(j);}
  static const QString attribId(const QString &name,unsigned int i,unsigned int j,unsigned int k)  {
    return name+"-"+QString::number(i)+"-"+QString::number(j)+"-"+QString::number(k);}

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


#endif // SCENE_TAGS_H
