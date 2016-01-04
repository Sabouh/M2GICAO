// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/widgetParams.h"

#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace std;

// *** WidgetFactory ***
QStringList WidgetFactory::_types = QStringList() << "Float" << "Integer" << "Vector2f" << "Vector2i" << "Vector3f" << "Vector3i" << "Vector4f" << "Vector4i";
QStringList WidgetFactory::_floatViews    = QStringList() << "Slider" << "Spin";
QStringList WidgetFactory::_intViews      = QStringList() << "Slider" << "Spin";
QStringList WidgetFactory::_vector2fViews = QStringList() << "Spin";
QStringList WidgetFactory::_vector2iViews = QStringList() << "Spin";
QStringList WidgetFactory::_vector3fViews = QStringList() << "Spin" << "Color";
QStringList WidgetFactory::_vector3iViews = QStringList() << "Spin";
QStringList WidgetFactory::_vector4fViews = QStringList() << "Spin" << "Color";
QStringList WidgetFactory::_vector4iViews = QStringList() << "Spin";

// const int WidgetFactory::FloatID    = 0;
// const int WidgetFactory::IntID      = 1;
// const int WidgetFactory::Vector2fID = 2;
// const int WidgetFactory::Vector2iID = 3;
// const int WidgetFactory::Vector3fID = 4;
// const int WidgetFactory::Vector3iID = 5;
// const int WidgetFactory::Vector4fID = 6;
// const int WidgetFactory::Vector4iID = 7;

QStringList WidgetFactory::types() {
  return _types;
}

QStringList WidgetFactory::views(int type) {
  if(type==0) {
    return _floatViews;
  } else if(type==1) {
    return _intViews;
  } else if(type==2) {
    return _vector2fViews;
  } else if(type==3) {
    return _vector2iViews;
  } else if(type==4) {
    return _vector3fViews;
  } else if(type==5) {
    return _vector3iViews;
  } else if(type==6) {
    return _vector4fViews;
  } else if(type==7) {
    return _vector4iViews;
  } else {
    return QStringList();
  }
}

NodeWidget *WidgetFactory::createAttribWidget(NodeInterface *node,const QString &name,int type,int view,float minv,float maxv,float defaultv,bool kf) {
  if(type<0 || type>=_types.size())
    return NULL;
  
  switch(type) {
  case 0: return createFloatAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 1: return createIntAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 2: return createVector2fAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 3: return createVector2iAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 4: return createVector3fAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 5: return createVector3iAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 6: return createVector4fAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  case 7: return createVector4iAttribWidget(node,name,view,minv,maxv,defaultv,kf);break;
  default: return NULL;
  }
}

FloatAttribWidget *WidgetFactory::createFloatAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {

  if(view<0 || view>=_floatViews.size())
    return NULL;

  switch(view) {
  case 0: return new FloatSliderWidget(node,name,minv,maxv,defaultv,kf);break;
  case 1: return new FloatSpinWidget(node,name,minv,maxv,defaultv,kf);break;
  default: return NULL;break;
  }
}

IntAttribWidget *WidgetFactory::createIntAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_intViews.size())
    return NULL;

  switch(view) {
  case 0: return new IntSliderWidget(node,name,(int)minv,(int)maxv,(int)defaultv,kf);break;
  case 1: return new IntSpinWidget(node,name,(int)minv,(int)maxv,(int)defaultv,kf);break;
  default: return NULL;break;
  }
}

Vector2fAttribWidget *WidgetFactory::createVector2fAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector2fViews.size())
    return NULL;

  Vector2f minvec(minv,minv);
  Vector2f maxvec(maxv,maxv);
  Vector2f defaultvec(defaultv,defaultv);

  switch(view) {
  case 0: return new Vector2fSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}

Vector2iAttribWidget *WidgetFactory::createVector2iAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector2iViews.size())
    return NULL;

  Vector2i minvec((int)minv,(int)minv);
  Vector2i maxvec((int)maxv,(int)maxv);
  Vector2i defaultvec((int)defaultv,(int)defaultv);

  switch(view) {
  case 0: return new Vector2iSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}


Vector3fAttribWidget *WidgetFactory::createVector3fAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector3fViews.size())
    return NULL;

  Vector3f minvec(minv,minv,minv);
  Vector3f maxvec(maxv,maxv,maxv);
  Vector3f defaultvec(defaultv,defaultv,defaultv);

  switch(view) {
  case 0: return new Vector3fSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  case 1: return new Vector3fColorWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}

Vector3iAttribWidget *WidgetFactory::createVector3iAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector3iViews.size())
    return NULL;

  Vector3i minvec((int)minv,(int)minv,(int)minv);
  Vector3i maxvec((int)maxv,(int)maxv,(int)maxv);
  Vector3i defaultvec((int)defaultv,(int)defaultv,(int)defaultv);

  switch(view) {
  case 0: return new Vector3iSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}

Vector4fAttribWidget *WidgetFactory::createVector4fAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector4fViews.size())
    return NULL;

  Vector4f minvec(minv,minv,minv,minv);
  Vector4f maxvec(maxv,maxv,maxv,maxv);
  Vector4f defaultvec(defaultv,defaultv,defaultv,defaultv);

  switch(view) {
  case 0: return new Vector4fSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  case 1: return new Vector4fColorWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}

Vector4iAttribWidget *WidgetFactory::createVector4iAttribWidget(NodeInterface *node,const QString &name,int view,float minv,float maxv,float defaultv,bool kf) {
  if(view<0 || view>=_vector4iViews.size())
    return NULL;

  Vector4i minvec((int)minv,(int)minv,(int)minv,(int)minv);
  Vector4i maxvec((int)maxv,(int)maxv,(int)maxv,(int)maxv);
  Vector4i defaultvec((int)defaultv,(int)defaultv,(int)defaultv,(int)defaultv);

  switch(view) {
  case 0: return new Vector4iSpinWidget(node,name,minvec,maxvec,defaultvec,kf);break;
  default: return NULL;break;
  }
}



// *** AttribLayout ***
QGridLayout *AttribLayout::create(QWidget *left,QWidget *center,QWidget *right) {
  QGridLayout *l = new QGridLayout();

  if(left)   l->addWidget(left,0,0,1,1);
  if(center) l->addWidget(center,0,1,1,4);
  if(right)  l->addWidget(right,0,5,1,1);
  return l;
}

QGridLayout *AttribLayout::create(QWidget *left,QLayout *center,QWidget *right) {
  QGridLayout *l = new QGridLayout();

  if(left)   l->addWidget(left,0,0,1,1);
  if(center) l->addLayout(center,0,1,1,4);
  if(right)  l->addWidget(right,0,5,1,1);
  return l;
}

// *** KeyframButtons ***
KeyframeButtons::KeyframeButtons(QWidget *parent) 
: QWidget(parent),
  _setButton(new QPushButton()),
  _editButton(new QPushButton()) {

  _setButton->setIcon(QIcon(":/set"));
  _editButton->setIcon(QIcon(":/edit"));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_setButton);
  l->addWidget(_editButton);
  setLayout(l);
}

// *** FloatAttribWidget ***
FloatAttribWidget::FloatAttribWidget(NodeInterface *node, const QString &name,float minVal,float maxVal,float value,bool keyf) 
  : NodeWidget(node),
    _nameLabel(new QLabel(name)),
    _kfButtons(NULL),
    _kf(name,value),
    _minVal(minVal),
    _maxVal(maxVal),
    _update(true),
    _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&FloatAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&FloatAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool FloatAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("floatAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveFloat("floatAttribMinVal",_minVal))       ret = false;
  if(!saver->saveFloat("floatAttribMaxVal",_maxVal))       ret = false;
  if(!saver->saveIOData("floatAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                             ret = false;

  return ret;
}

bool FloatAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("floatAttribKeyframed",k))     ret = false;
  if(!loader->loadFloat("floatAttribMinVal",_minVal)) ret = false;
  if(!loader->loadFloat("floatAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("floatAttribCurve",&_kf))    ret = false;
  if(!NodeWidget::load(loader))                       ret = false;

  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&FloatAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&FloatAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&FloatAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&FloatAttribWidget::setClicked);
  }

  _update = _changed = false;
  float v = val();

  setMin(_minVal);
  setMax(_maxVal);
  setVal(v,false);
  return ret;
}

void FloatAttribWidget::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void FloatAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.curve()->addKeyframe(Vector2f(current,val()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void FloatAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],val());
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}


// *** IntAttribWidget ***
IntAttribWidget::IntAttribWidget(NodeInterface *node, const QString &name,int minVal,int maxVal,int value,bool keyf) 
  : NodeWidget(node),
    _nameLabel(new QLabel(name)),
    _kfButtons(NULL),
    _kf(name,value),
    _minVal(minVal),
    _maxVal(maxVal),
    _update(true),
    _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&IntAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&IntAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool IntAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("intAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveInt("intAttribMinVal",_minVal))         ret = false;
  if(!saver->saveInt("intAttribMaxVal",_maxVal))         ret = false;
  if(!saver->saveIOData("intAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                           ret = false;

  return ret;
}

bool IntAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("intAttribKeyframed",k))     ret = false;
  if(!loader->loadInt("intAttribMinVal",_minVal))   ret = false;
  if(!loader->loadInt("intAttribMaxVal",_maxVal))   ret = false;
  if(!loader->loadIOData("intAttribCurve",&_kf))    ret = false;
  if(!NodeWidget::load(loader))                     ret = false;

  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&IntAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&IntAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&IntAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&IntAttribWidget::setClicked);
  }

  _update = _changed = false;
  int v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);

  return ret;
}

void IntAttribWidget::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void IntAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.curve()->addKeyframe(Vector2f(current,val()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void IntAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],(float)val());
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}

// *** Vector2fAttribWidget ***
Vector2fAttribWidget::Vector2fAttribWidget(NodeInterface *node, const QString &name,
					   const Vector2f &minVal,const Vector2f &maxVal,
					   const Vector2f &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector2fAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector2fAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector2f("vector2fAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector2f("vector2fAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector2fAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector2fAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector2fAttribKeyframed",k))        ret = false;
  if(!loader->loadVector2f("vector2fAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector2f("vector2fAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector2fAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2fAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector2f v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector2fAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector2fAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector2fAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}



// *** Vector2iAttribWidget ***
Vector2iAttribWidget::Vector2iAttribWidget(NodeInterface *node, const QString &name,
					   const Vector2i &minVal,const Vector2i &maxVal,
					   const Vector2i &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector2iAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector2iAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector2i("vector2iAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector2i("vector2iAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector2iAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector2iAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector2iAttribKeyframed",k))        ret = false;
  if(!loader->loadVector2i("vector2iAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector2i("vector2iAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector2iAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector2iAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector2i v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector2iAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector2iAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector2iAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}


// *** Vector3fAttribWidget ***
Vector3fAttribWidget::Vector3fAttribWidget(NodeInterface *node, const QString &name,
					   const Vector3f &minVal,const Vector3f &maxVal,
					   const Vector3f &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector3fAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector3fAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector3f("vector3fAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector3f("vector3fAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector3fAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector3fAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector3fAttribKeyframed",k))        ret = false;
  if(!loader->loadVector3f("vector3fAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector3f("vector3fAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector3fAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3fAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector3f v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector3fAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector3fAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector3fAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}



// *** Vector3iAttribWidget ***
Vector3iAttribWidget::Vector3iAttribWidget(NodeInterface *node, const QString &name,
					   const Vector3i &minVal,const Vector3i &maxVal,
					   const Vector3i &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector3iAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector3iAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector3i("vector3iAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector3i("vector3iAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector3iAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector3iAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector3iAttribKeyframed",k))        ret = false;
  if(!loader->loadVector3i("vector3iAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector3i("vector3iAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector3iAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector3iAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector3i v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector3iAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector3iAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector3iAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}



// *** Vector4fAttribWidget ***
Vector4fAttribWidget::Vector4fAttribWidget(NodeInterface *node, const QString &name,
					   const Vector4f &minVal,const Vector4f &maxVal,
					   const Vector4f &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector4fAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector4fAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector4f("vector4fAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector4f("vector4fAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector4fAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector4fAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector4fAttribKeyframed",k))        ret = false;
  if(!loader->loadVector4f("vector4fAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector4f("vector4fAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector4fAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4fAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector4f v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector4fAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector4fAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector4fAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}



// *** Vector4iAttribWidget ***
Vector4iAttribWidget::Vector4iAttribWidget(NodeInterface *node, const QString &name,
					   const Vector4i &minVal,const Vector4i &maxVal,
					   const Vector4i &value,bool keyf)
  : NodeWidget(node),
  _nameLabel(new QLabel(name)),
  _kfButtons(NULL),
  _kf(name,value),
  _minVal(minVal),
  _maxVal(maxVal),
  _update(true),
  _changed(false) {

  _kfButtons = new KeyframeButtons();

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }
}


bool Vector4iAttribWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("vector4iAttribKeyframed",keyframed())) ret = false;
  if(!saver->saveVector4i("vector4iAttribMinVal",_minVal))    ret = false;
  if(!saver->saveVector4i("vector4iAttribMaxVal",_maxVal))    ret = false;
  if(!saver->saveIOData("vector4iAttribCurve",&_kf))          ret = false;
  if(!NodeWidget::save(saver))                                ret = false;

  return ret;
}

bool Vector4iAttribWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;

  if(!loader->loadBool("vector4iAttribKeyframed",k))        ret = false;
  if(!loader->loadVector4i("vector4iAttribMinVal",_minVal)) ret = false;
  if(!loader->loadVector4i("vector4iAttribMaxVal",_maxVal)) ret = false;
  if(!loader->loadIOData("vector4iAttribCurve",&_kf))       ret = false;
  if(!NodeWidget::load(loader))                             ret = false;


  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&Vector4iAttribWidget::setClicked);
  }

  _update = _changed = false;
  Vector4i v = val();

  setMin(_minVal);
  setMax(_maxVal);

  setVal(v,false);
  return ret;
}

void Vector4iAttribWidget::editClicked() {
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kf.curve(i),QStringList() << name() << _kf.curveName(i),_minVal[i],_maxVal[i]);
  }
  AnimationWidget::instance()->show();
}

void Vector4iAttribWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kf.nbCurves();++i) {
    _kf.curve(i)->addKeyframe(Vector2f(current,val()[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
  }
}

void Vector4iAttribWidget::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kf.nbCurves();++i) {
      const Vector2f p(_kf.curve(i)->point()[0],(float)val()[i]);
      _kf.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kf.curveName(i));
    }
  }
}




// *** FloatSliderWidget ***
FloatSliderWidget::FloatSliderWidget(NodeInterface *node, const QString &name,float minVal,float maxVal,float value,bool keyf) 
  : FloatAttribWidget(node,name,minVal,maxVal,value,keyf),
    _scale(1000.0f),
    _precision(3) {
  
  _slider = new QSlider(Qt::Horizontal);
  _slider->setMinimum(0);
  _slider->setMaximum((int)_scale);
  _slider->setValue(floatToInt(val()));
  _valLabel  = new QLabel(QString::number(val(),'f',_precision));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_slider);
  l->addWidget(_valLabel);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_slider,&QSlider::valueChanged,this,&FloatSliderWidget::valueChanged);
}

void FloatSliderWidget::valueChanged(int v) {
  _kf.set(intToFloat(v));
  emit valChanged();
  _valLabel->setText(QString::number(val(),'f',_precision));
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}


// *** FloatSpinWidget ***
FloatSpinWidget::FloatSpinWidget(NodeInterface *node, const QString &name,float minVal,float maxVal,float value,bool keyf) 
  : FloatAttribWidget(node,name,minVal,maxVal,value,keyf) {
  
  _spin = new QDoubleSpinBox();
  _spin->setMinimum((double)minVal);
  _spin->setMaximum((double)maxVal);
  _spin->setSingleStep((double)(maxVal-minVal)/100.0);
  _spin->setValue((double)value);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spin);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spin,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

void FloatSpinWidget::valueChanged(double v) {
  _kf.set((float)v);
  emit valChanged();

  if(_update) {
    updateGraph();
  }

  checkChangeCurve();
}


// *** IntSliderWidget ***
IntSliderWidget::IntSliderWidget(NodeInterface *node, const QString &name,int minVal,int maxVal,int value,bool keyf) 
  : IntAttribWidget(node,name,minVal,maxVal,value,keyf) {
  
  _slider = new QSlider(Qt::Horizontal);
  _slider->setMinimum(_minVal);
  _slider->setMaximum(_maxVal);
  _slider->setValue(val());
  _valLabel  = new QLabel(QString::number(val()));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_slider);
  l->addWidget(_valLabel);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_slider,&QSlider::valueChanged,this,&IntSliderWidget::valueChanged);
}

void IntSliderWidget::valueChanged(int v) {
  _kf.set(v);
  emit valChanged();
  _valLabel->setText(QString::number(val()));
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}

// *** IntSpinWidget ***
IntSpinWidget::IntSpinWidget(NodeInterface *node, const QString &name,int minVal,int maxVal,int value,bool keyf) 
  : IntAttribWidget(node,name,minVal,maxVal,value,keyf) {
  
  _spin = new QSpinBox();
  _spin->setMinimum(minVal);
  _spin->setMaximum(maxVal);
  _spin->setValue(value);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spin);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spin,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void IntSpinWidget::valueChanged(int v) {
  _kf.set(v);
  emit valChanged();

  if(_update) {
    updateGraph();
  }

  checkChangeCurve();
}


// *** Vector2iSpinWidget ***
Vector2iSpinWidget::Vector2iSpinWidget(NodeInterface *node, const QString &name,
					   const Vector2i &minVal,const Vector2i &maxVal,
					   const Vector2i &value,bool keyf)
  : Vector2iAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QSpinBox();
  _spinx->setMinimum(minVal[0]);
  _spinx->setMaximum(maxVal[0]);
  _spinx->setValue(val()[0]);

  _spiny = new QSpinBox();
  _spiny->setMinimum(minVal[1]);
  _spiny->setMaximum(maxVal[1]);
  _spiny->setValue(val()[1]);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spiny,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void Vector2iSpinWidget::valueChanged(int) {
  _kf.set(Vector2i(_spinx->value(),_spiny->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}


// *** Vector2fSpinWidget ***
Vector2fSpinWidget::Vector2fSpinWidget(NodeInterface *node, const QString &name,
					   const Vector2f &minVal,const Vector2f &maxVal,
					   const Vector2f &value,bool keyf)
  : Vector2fAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QDoubleSpinBox();
  _spinx->setMinimum((double)minVal[0]);
  _spinx->setMaximum((double)maxVal[0]);
  _spinx->setValue((double)val()[0]);
  _spinx->setSingleStep((double)(maxVal[0]-minVal[0])/1000.0);
  _spinx->setDecimals(3);

  _spiny = new QDoubleSpinBox();
  _spiny->setMinimum((double)minVal[1]);
  _spiny->setMaximum((double)maxVal[1]);
  _spiny->setValue((double)val()[1]);
  _spiny->setSingleStep((double)(maxVal[1]-minVal[1])/1000.0);
  _spiny->setDecimals(3);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spiny,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

void Vector2fSpinWidget::valueChanged(double) {
  _kf.set(Vector2f((float)_spinx->value(),(float)_spiny->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}



// *** Vector3iSpinWidget ***
Vector3iSpinWidget::Vector3iSpinWidget(NodeInterface *node, const QString &name,
					   const Vector3i &minVal,const Vector3i &maxVal,
					   const Vector3i &value,bool keyf)
  : Vector3iAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QSpinBox();
  _spinx->setMinimum(minVal[0]);
  _spinx->setMaximum(maxVal[0]);
  _spinx->setValue(val()[0]);

  _spiny = new QSpinBox();
  _spiny->setMinimum(minVal[1]);
  _spiny->setMaximum(maxVal[1]);
  _spiny->setValue(val()[1]);

  _spinz = new QSpinBox();
  _spinz->setMinimum(minVal[2]);
  _spinz->setMaximum(maxVal[2]);
  _spinz->setValue(val()[2]);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);
  l->addWidget(_spinz);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spiny,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spinz,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void Vector3iSpinWidget::valueChanged(int) {
  _kf.set(Vector3i(_spinx->value(),_spiny->value(),_spinz->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}


// *** Vector3fSpinWidget ***
Vector3fSpinWidget::Vector3fSpinWidget(NodeInterface *node, const QString &name,
					   const Vector3f &minVal,const Vector3f &maxVal,
					   const Vector3f &value,bool keyf)
  : Vector3fAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QDoubleSpinBox();
  _spinx->setMinimum((double)minVal[0]);
  _spinx->setMaximum((double)maxVal[0]);
  _spinx->setValue((double)val()[0]);
  _spinx->setSingleStep((double)(maxVal[0]-minVal[0])/1000.0);
  _spinx->setDecimals(3);

  _spiny = new QDoubleSpinBox();
  _spiny->setMinimum((double)minVal[1]);
  _spiny->setMaximum((double)maxVal[1]);
  _spiny->setValue((double)val()[1]);
  _spiny->setSingleStep((double)(maxVal[1]-minVal[1])/1000.0);
  _spiny->setDecimals(3);

  _spinz = new QDoubleSpinBox();
  _spinz->setMinimum((double)minVal[2]);
  _spinz->setMaximum((double)maxVal[2]);
  _spinz->setValue((double)val()[2]);
  _spinz->setSingleStep((double)(maxVal[2]-minVal[2])/1000.0);
  _spinz->setDecimals(3);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);
  l->addWidget(_spinz);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spiny,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spinz,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

void Vector3fSpinWidget::valueChanged(double) {
  _kf.set(Vector3f((float)_spinx->value(),(float)_spiny->value(),(float)_spinz->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}

// *** Vector3fColorWidget ***
Vector3fColorWidget::Vector3fColorWidget(NodeInterface *node, const QString &name,
					 const Vector3f &minVal,const Vector3f &maxVal,
					 const Vector3f &value,bool keyf)
  : Vector3fAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _button = new QPushButton();
  _button->setAutoFillBackground(true);
  setButtonColor(value);

  _colorPicker = new QColorDialog(QColor((int)(value[0]*255.0f),
 					 (int)(value[1]*255.0f),
 					 (int)(value[2]*255.0f)),this);
  _colorPicker->setOptions(QColorDialog::NoButtons);
  _colorPicker->setHidden(true);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_button);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_button,SIGNAL(clicked()),this,SLOT(buttonClicked()));
  connect(_colorPicker,SIGNAL(currentColorChanged(const QColor&)),this,SLOT(colorChanged(const QColor &)));
}

void Vector3fColorWidget::buttonClicked() {
  _colorPicker->setHidden(false);
}

void Vector3fColorWidget::colorChanged(const QColor &color) {
  _kf.set(Vector3f(color.redF(),color.greenF(),color.blueF()));
  setButtonColor(val());
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}
  

// *** Vector4iSpinWidget ***
Vector4iSpinWidget::Vector4iSpinWidget(NodeInterface *node, const QString &name,
					   const Vector4i &minVal,const Vector4i &maxVal,
					   const Vector4i &value,bool keyf)
  : Vector4iAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QSpinBox();
  _spinx->setMinimum(minVal[0]);
  _spinx->setMaximum(maxVal[0]);
  _spinx->setValue(val()[0]);

  _spiny = new QSpinBox();
  _spiny->setMinimum(minVal[1]);
  _spiny->setMaximum(maxVal[1]);
  _spiny->setValue(val()[1]);

  _spinz = new QSpinBox();
  _spinz->setMinimum(minVal[2]);
  _spinz->setMaximum(maxVal[2]);
  _spinz->setValue(val()[2]);

  _spinw = new QSpinBox();
  _spinw->setMinimum(minVal[3]);
  _spinw->setMaximum(maxVal[3]);
  _spinw->setValue(val()[3]);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);
  l->addWidget(_spinz);
  l->addWidget(_spinw);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spiny,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spinz,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(_spinw,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void Vector4iSpinWidget::valueChanged(int) {
  _kf.set(Vector4i(_spinx->value(),_spiny->value(),_spinz->value(),_spinw->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}


// *** Vector4fSpinWidget ***
Vector4fSpinWidget::Vector4fSpinWidget(NodeInterface *node, const QString &name,
					   const Vector4f &minVal,const Vector4f &maxVal,
					   const Vector4f &value,bool keyf)
  : Vector4fAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _spinx = new QDoubleSpinBox();
  _spinx->setMinimum((double)minVal[0]);
  _spinx->setMaximum((double)maxVal[0]);
  _spinx->setValue((double)val()[0]);
  _spinx->setSingleStep((double)(maxVal[0]-minVal[0])/1000.0);
  _spinx->setDecimals(3);

  _spiny = new QDoubleSpinBox();
  _spiny->setMinimum((double)minVal[1]);
  _spiny->setMaximum((double)maxVal[1]);
  _spiny->setValue((double)val()[1]);
  _spiny->setSingleStep((double)(maxVal[1]-minVal[1])/1000.0);
  _spiny->setDecimals(3);

  _spinz = new QDoubleSpinBox();
  _spinz->setMinimum((double)minVal[2]);
  _spinz->setMaximum((double)maxVal[2]);
  _spinz->setValue((double)val()[2]);
  _spinz->setSingleStep((double)(maxVal[2]-minVal[2])/1000.0);
  _spinz->setDecimals(3);

  _spinw = new QDoubleSpinBox();
  _spinw->setMinimum((double)minVal[3]);
  _spinw->setMaximum((double)maxVal[3]);
  _spinw->setValue((double)val()[3]);
  _spinw->setSingleStep((double)(maxVal[3]-minVal[3])/1000.0);
  _spinw->setDecimals(3);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_spinx);
  l->addWidget(_spiny);
  l->addWidget(_spinz);
  l->addWidget(_spinw);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_spinx,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spiny,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spinz,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  connect(_spinw,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

void Vector4fSpinWidget::valueChanged(double) {
  _kf.set(Vector4f((float)_spinx->value(),(float)_spiny->value(),(float)_spinz->value(),(float)_spinw->value()));
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}

// *** Vector4fColorWidget ***
Vector4fColorWidget::Vector4fColorWidget(NodeInterface *node, const QString &name,
					 const Vector4f &minVal,const Vector4f &maxVal,
					 const Vector4f &value,bool keyf)
  : Vector4fAttribWidget(node,name,minVal,maxVal,value,keyf) {

  _button = new QPushButton();
  _button->setAutoFillBackground(true);
  setButtonColor(value);

  _colorPicker = new QColorDialog(QColor((int)(value[0]*255.0f),
 					 (int)(value[1]*255.0f),
 					 (int)(value[2]*255.0f),
					 (int)(value[3]*255.0f)),this);
  _colorPicker->setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::NoButtons);
  _colorPicker->setHidden(true);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_button);

  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));
  connect(_button,SIGNAL(clicked()),this,SLOT(buttonClicked()));
  connect(_colorPicker,SIGNAL(currentColorChanged(const QColor&)),this,SLOT(colorChanged(const QColor &)));
}

void Vector4fColorWidget::buttonClicked() {
  _colorPicker->setHidden(false);
}

void Vector4fColorWidget::colorChanged(const QColor &color) {
  _kf.set(Vector4f(color.redF(),color.greenF(),color.blueF(),color.alphaF()));
  setButtonColor(val());
  emit valChanged();
  if(_update) {
    updateGraph();
  }
  checkChangeCurve();
}
  












// *** TrackballCameraWidget ***
TrackballCameraWidget::TrackballCameraWidget(NodeInterface *node, const QString &name,
					     TrackballCamera *cam,bool keyf)
  : NodeWidget(node),
    _nameLabel(new QLabel(name)),
    _kfButtons(NULL),
    _kfq("rotation",cam->rotation()),
    _kfs("shift",cam->shift()),
    _update(true),
    _changed(false),
    _cam(cam),
    _projection(new QComboBox()),
    _rotation(new QComboBox()) {

  _kfButtons = new KeyframeButtons();
  _projection->addItems(QStringList() << "perspective" << "orthographic");
  _projection->setCurrentIndex(_cam->projType());
  _rotation->addItems(QStringList() << "Scene centered" << "World centered");
  _rotation->setCurrentIndex(_cam->rotType());

  if(keyf) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&TrackballCameraWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&TrackballCameraWidget::setClicked);
  } else {
    _kfButtons->setHidden(true);
  }

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_projection);
  l->addWidget(_rotation);
  setLayout(AttribLayout::create(_nameLabel,l,_kfButtons));

  connect(_projection,SIGNAL(currentIndexChanged(int)),this,SLOT(projectionChanged(int)));
  connect(_rotation,SIGNAL(currentIndexChanged(int)),this,SLOT(rotationChanged(int)));
  connect(_cam,SIGNAL(cameraChanged()),this,SLOT(checkChangeCurve()));
}

void TrackballCameraWidget::projectionChanged(int val) {
  _cam->setProjection(val);
  if(_update) {
    updateGraph();
  }
}

void TrackballCameraWidget::rotationChanged(int val) {
  _cam->setRotType(val);
  if(_update) {
    updateGraph();
  }
}

bool TrackballCameraWidget::save(SceneSaver *saver) {
  bool ret = true;
  
  if(!saver->saveBool("trackballCameraKeyframed",keyframed()))  ret = false;
  if(!saver->saveIOData("trackballCameraAttribCurveQ",&_kfq))   ret = false;
  if(!saver->saveIOData("trackballCameraAttribCurveS",&_kfs))   ret = false;
  if(!saver->saveInt("projection",_projection->currentIndex())) ret = false;
  if(!saver->saveInt("rotation",_rotation->currentIndex()))     ret = false;
  if(!NodeWidget::save(saver))                                  ret = false;

  return ret;
}

bool TrackballCameraWidget::load(SceneLoader *loader) {
  bool ret = true, k=true;
  int proj = 0,rot = 0;

  if(!loader->loadBool("trackballCameraKeyframed",k))           ret = false;
  if(!loader->loadIOData("trackballCameraAttribCurveQ",&_kfq))  ret = false;
  if(!loader->loadIOData("trackballCameraAttribCurveS",&_kfs))  ret = false;
  if(!loader->loadInt("projection",proj))                       ret = false;
  if(!loader->loadInt("rotation",rot))                          ret = false;
  if(!NodeWidget::load(loader))                                 ret = false;

  if(keyframed()) {
    _kfButtons->setHidden(true);
    disconnect(_kfButtons->editButton(),&QPushButton::clicked,this,&TrackballCameraWidget::editClicked);
    disconnect(_kfButtons->setButton(),&QPushButton::clicked,this,&TrackballCameraWidget::setClicked);
  }

  if(k) {
    _kfButtons->setHidden(false);
    connect(_kfButtons->editButton(),&QPushButton::clicked,this,&TrackballCameraWidget::editClicked);
    connect(_kfButtons->setButton(),&QPushButton::clicked,this,&TrackballCameraWidget::setClicked);
  }

  _update = false;
  _projection->setCurrentIndex(proj);
  _rotation->setCurrentIndex(rot);
  _update = true;

  return ret;
}

void TrackballCameraWidget::editClicked() {
  for(unsigned int i=0;i<_kfq.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kfq.curve(i),QStringList() << name() << _kfq.name() << _kfq.curveName(i),-10000,10000);
  }
  
  for(unsigned int i=0;i<_kfs.nbCurves();++i) {
    AnimationWidget::instance()->addParam(this,_kfs.curve(i),QStringList() << name() << _kfs.name() << _kfs.curveName(i),-10000,10000);
  }
  AnimationWidget::instance()->show();
}

void TrackballCameraWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  for(unsigned int i=0;i<_kfq.nbCurves();++i) {
    const Quaternionf q = val()->rotation();
    const Vector4f v(q.w(),q.x(),q.y(),q.z());
    _kfq.curve(i)->addKeyframe(Vector2f(current,v[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kfq.name() << _kfq.curveName(i));
  }
  
  for(unsigned int i=0;i<_kfs.nbCurves();++i) {
    const Vector3f v = val()->shift();
    _kfs.curve(i)->addKeyframe(Vector2f(current,v[i]));
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kfs.name() << _kfs.curveName(i));
  }
}

void TrackballCameraWidget::checkChangeCurve() {
  if(_kfq.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kfq.nbCurves();++i) {
      const Quaternionf q = val()->rotation();
      const Vector4f v(q.w(),q.x(),q.y(),q.z());
      const Vector2f p(_kfq.curve(i)->point()[0],v[i]);
      _kfq.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kfq.name() << _kfq.curveName(i));
    }
  }

  if(_kfs.curve()->nbPoints()<2) {
    for(unsigned int i=0;i<_kfs.nbCurves();++i) {
      const Vector3f v = val()->shift();
      const Vector2f p(_kfs.curve(i)->point()[0],v[i]);
      _kfs.curve(i)->setKeyframe(p);
      AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << _kfs.name() << _kfs.curveName(i));
    }
  }
}



