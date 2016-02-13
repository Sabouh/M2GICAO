// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/keyframedParams.h"

KeyframedVar::KeyframedVar(const QString &name) : _name(name) {

}

KeyframedVar::~KeyframedVar() {
  for(unsigned int i=0;i<nbCurves();++i) {
    delete _curves[i];
  }
  
  _curves.clear();
}

bool KeyframedVar::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveUint("nbCurves",nbCurves())) ret = false;
  for(unsigned int i=0;i<nbCurves();++i) {
    if(!saver->saveIOData(Tags::attribId("curve",i),_curves[i])) ret = false;
  }
  return ret;
}

bool KeyframedVar::load(SceneLoader *loader) {
  bool ret = true;
  unsigned int nb = 0;

  if(!loader->loadUint("nbCurves",nb)) ret = false;
  
  if(nb!=nbCurves()) { ret = false; nb = nbCurves(); }

  for(unsigned int i=0;i<nbCurves();++i) {
    if(!loader->loadIOData(Tags::attribId("curve",i),_curves[i])) ret = false;
  }
  return ret;
}

bool KeyframedFloat::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveFloat("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedFloat::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadFloat("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector2f::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector2f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector2f::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector2f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector3f::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector3f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector3f::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector3f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector4f::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector4f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector4f::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector4f("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedInt::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveInt("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedInt::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadInt("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector2i::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector2i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector2i::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector2i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector3i::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector3i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector3i::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector3i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedVector4i::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector4i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedVector4i::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadVector4i("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}

bool KeyframedQuaternionf::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveQuaternionf("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::save(saver)) ret = false;
  return ret;
}

bool KeyframedQuaternionf::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadQuaternionf("currentVal",_currentVal)) ret = false;
  if(!KeyframedVar::load(loader)) ret = false;
  return ret;
}
