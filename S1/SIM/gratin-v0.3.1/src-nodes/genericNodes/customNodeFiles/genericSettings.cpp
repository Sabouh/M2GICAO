// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "genericSettings.h"
#include "genericCustomWidget.h"
#include "view/widgetParams.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

using namespace std;

#define GERROR(msg) { QMessageBox::information(NULL,tr("Error"),msg); }

GenericCustomIOItemButtons::GenericCustomIOItemButtons()
  : _up(new QToolButton()),
    _down(new QToolButton()),
    _del(new QToolButton()) {
  
  _up->setIcon(QIcon(":/arrow-up"));
  _down->setIcon(QIcon(":/arrow-down"));
  _del->setIcon(QIcon(":/close"));
  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_up);
  l->addWidget(_down);
  l->addWidget(_del);
  setLayout(l);
}
    
GenericCustomIOItem::GenericCustomIOItem(unsigned int id,const QString &name) 
  : _id(id),
    _number(new QLabel(QString::number(_id))),
    _name(new QLineEdit(name)),
    _buttons(new GenericCustomIOItemButtons()) {

  QGridLayout *l = new QGridLayout();
  
  l->addWidget(_number,0,0);
  l->addWidget(_name,0,1);
  l->addWidget(_buttons,0,2);

  setLayout(l);
  
  connect(_buttons->up(),SIGNAL(clicked()),this,SLOT(needUpSignal()));
  connect(_buttons->down(),SIGNAL(clicked()),this,SLOT(needDownSignal()));
  connect(_buttons->del(),SIGNAL(clicked()),this,SLOT(needDelSignal()));
}

void GenericCustomIOItem::needUpSignal  () { emit upClicked(this);   }
void GenericCustomIOItem::needDownSignal() { emit downClicked(this); }
void GenericCustomIOItem::needDelSignal () { emit delClicked(this);  }

GenericCustomParamItem::GenericCustomParamItem(unsigned int id,const QString &name,int type,int view,bool keyframed,
					       float minVal,float maxVal,float defaultVal)
  : _id(id),
    _name(new QLineEdit(name)),
    _minEdit(new QLineEdit(QString::number(minVal))),
    _maxEdit(new QLineEdit(QString::number(maxVal))),
    _valEdit(new QLineEdit(QString::number(defaultVal))),
    _minLabel(new QLabel("Min:")),
    _maxLabel(new QLabel("Max:")),
    _valLabel(new QLabel("Default:")),
    _type(new QComboBox()),
    _view(new QComboBox()),
    _kf(new QCheckBox("Keyframed")),
    _buttons(new GenericCustomIOItemButtons()) {

  _type->addItems(WidgetFactory::types());
  _type->setCurrentIndex(type);
  _view->addItems(WidgetFactory::views(_type->currentIndex()));

  if(view<_view->count())
    _view->setCurrentIndex(view);
  
  _kf->setCheckState(keyframed ? Qt::Checked : Qt::Unchecked);

  QGridLayout *l = new QGridLayout();
  
  l->addWidget(_name   ,0,0,1,2);
  l->addWidget(_type   ,0,2,1,2);
  l->addWidget(_view   ,0,4,1,2);
  l->addWidget(_buttons,0,6,1,2);

  l->addWidget(_minLabel,1,0);
  l->addWidget(_minEdit ,1,1);
  l->addWidget(_maxLabel,1,2);
  l->addWidget(_maxEdit ,1,3);
  l->addWidget(_valLabel,1,4);
  l->addWidget(_valEdit ,1,5);
  l->addWidget(_kf      ,1,6,1,2);

  QFrame *line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  l->addWidget(line,2,0,1,8);
  setLayout(l);
  
  connect(_buttons->up(),SIGNAL(clicked()),this,SLOT(needUpSignal()));
  connect(_buttons->down(),SIGNAL(clicked()),this,SLOT(needDownSignal()));
  connect(_buttons->del(),SIGNAL(clicked()),this,SLOT(needDelSignal()));
  connect(_type,SIGNAL(currentIndexChanged(int)),this,SLOT(typeChanged(int)));
}

void GenericCustomParamItem::typeChanged(int index) {
  _view->clear();
  _view->addItems(WidgetFactory::views(index));
} 

void GenericCustomParamItem::needUpSignal  () { emit upClicked(this);   }
void GenericCustomParamItem::needDownSignal() { emit downClicked(this); }
void GenericCustomParamItem::needDelSignal () { emit delClicked(this);  }

IOList::IOList(const QString &title,const std::vector<QString> &names,const QString &prefix,bool nodeConnected)
  : QGroupBox(title),
  _names(names),
  _prefix(prefix),
  _nodeConnected(nodeConnected),
  _add(new QPushButton("Add")),
  _layout(new QVBoxLayout()) {

  initItems();
  buildLayout();

  QScrollArea *area = new QScrollArea();
  area->setWidgetResizable(true);
  QWidget *tmp = new QWidget();
  tmp->setLayout(_layout);
  area->setWidget(tmp);
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(area);
  l->addWidget(_add);
  setLayout(l);

  connect(_add,SIGNAL(clicked()),this,SLOT(addClicked()));
}

void IOList::itemUp(GenericCustomIOItem *item) {
  if(item->id()==0) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _items[id-1];
  _items[id-1]->setId(id);
  _items[id]->setId(id-1);
  _items[id-1] = item;
  _items[id] = tmpItem;
  cleanLayout();
  buildLayout();
}

void IOList::itemDown(GenericCustomIOItem *item) {
  if(item->id()==_items.size()-1) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _items[id+1];
  _items[id+1]->setId(id);
  _items[id]->setId(id+1);
  _items[id+1] = item;
  _items[id] = tmpItem;
  cleanLayout();
  buildLayout();
}

void IOList::itemDel(GenericCustomIOItem *item) {
  if(_nodeConnected) {
    GERROR("Unable to remove input\nPlease, remove all connections from the node first");
    return;
  }

  cleanLayout();

  unsigned int id = item->id();
  delete item;
  _items.erase(_items.begin()+id);
  
  for(unsigned int i=id;i<_items.size();++i) {
    _items[i]->setId(_items[i]->id()-1); 
  }

  buildLayout();
}
  
void IOList::addClicked() {
  if(_nodeConnected) {
    GERROR("Unable to add input\nPlease, remove all connections from the node first");
    return;
  }

  unsigned int index = static_cast<unsigned int>(_items.size());
  _items.push_back(new GenericCustomIOItem(index,_prefix+QString::number(index)));
  connect(_items[index],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(itemUp(GenericCustomIOItem *)));
  connect(_items[index],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(itemDown(GenericCustomIOItem *)));
  connect(_items[index],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(itemDel(GenericCustomIOItem *)));
  _layout->addWidget(_items[index]);
}
  
void IOList::buildLayout() {
  for(unsigned int i=0;i<_items.size();++i) {
    _layout->addWidget(_items[i]);
  }
}

void IOList::cleanLayout() {
  for(unsigned int i=0;i<_items.size();++i) {
    _layout->removeWidget(_items[i]);
  }
}

void IOList::initItems() {
  for(unsigned int i=0;i<_names.size();++i) {
    _items.push_back(new GenericCustomIOItem(i,_names[i]));
    connect(_items[i],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(itemUp(GenericCustomIOItem *)));
    connect(_items[i],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(itemDown(GenericCustomIOItem *)));
    connect(_items[i],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(itemDel(GenericCustomIOItem *)));
  }
}

void IOList::cleanItems() {
  for(unsigned int i=0;i<_items.size();++i) {
    delete _items[i];
  }
  _items.clear();
}


ParamList::ParamList(const QString &title,GenericCustomWidget *nodeWidget)
  : QGroupBox(title),
  _nodeWidget(nodeWidget),
  _add(new QPushButton("Add")),
  _layout(new QVBoxLayout()),
  _prefix("param") {

  initItems();
  buildLayout();

  QScrollArea *area = new QScrollArea();
  area->setWidgetResizable(true);
  QWidget *tmp = new QWidget();
  tmp->setLayout(_layout);
  area->setWidget(tmp);
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(area);
  l->addWidget(_add);
  setLayout(l);

  connect(_add,SIGNAL(clicked()),this,SLOT(addClicked()));
}


void ParamList::itemUp(GenericCustomParamItem *item) {
  if(item->id()==0) return;
  const unsigned int id = item->id();
  GenericCustomParamItem *tmpItem = _items[id-1];
  _items[id-1]->setId(id);
  _items[id]->setId(id-1);
  _items[id-1] = item;
  _items[id] = tmpItem;
  cleanLayout();
  buildLayout();
}

void ParamList::itemDown(GenericCustomParamItem *item) {
  if(item->id()==_items.size()-1) return;
  const unsigned int id = item->id();
  GenericCustomParamItem *tmpItem = _items[id+1];
  _items[id+1]->setId(id);
  _items[id]->setId(id+1);
  _items[id+1] = item;
  _items[id] = tmpItem;
  cleanLayout();
  buildLayout();
}


void ParamList::itemDel(GenericCustomParamItem *item) {
  cleanLayout();

  unsigned int id = item->id();
  delete item;
  _items.erase(_items.begin()+id);
  
  for(unsigned int i=id;i<_items.size();++i) {
    _items[i]->setId(_items[i]->id()-1); 
  }

  buildLayout();
}
  
void ParamList::addClicked() {
  unsigned int index = static_cast<unsigned int>(_items.size());
  _items.push_back(new GenericCustomParamItem(index,_prefix+QString::number(index)));
  connect(_items[index],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  connect(_items[index],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  connect(_items[index],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  _layout->addWidget(_items[index]);
}
  
void ParamList::buildLayout() {
  for(unsigned int i=0;i<_items.size();++i) {
    _layout->addWidget(_items[i]);
  }
}

void ParamList::cleanLayout() {
  for(unsigned int i=0;i<_items.size();++i) {
    _layout->removeWidget(_items[i]);
  }
}

void ParamList::initItems() {
  vector<pair<int,int>         > mw = _nodeWidget->mapWidgets();
  vector<FloatAttribWidget    *> fs = _nodeWidget->floatWidgets();
  vector<IntAttribWidget      *> is = _nodeWidget->intWidgets();
  vector<Vector2fAttribWidget *> v2fs = _nodeWidget->vector2fWidgets();
  vector<Vector2iAttribWidget *> v2is = _nodeWidget->vector2iWidgets();
  vector<Vector3fAttribWidget *> v3fs = _nodeWidget->vector3fWidgets();
  vector<Vector3iAttribWidget *> v3is = _nodeWidget->vector3iWidgets();
  vector<Vector4fAttribWidget *> v4fs = _nodeWidget->vector4fWidgets();
  vector<Vector4iAttribWidget *> v4is = _nodeWidget->vector4iWidgets();
  
  for(unsigned int i=0;i<mw.size();++i) {
    int type  = mw[i].first;
    int index = mw[i].second;

    switch(type) {
    case WidgetFactory::FloatID:
      _items.push_back(new GenericCustomParamItem(i,fs[index]->name(),WidgetFactory::FloatID,
						  fs[index]->type()-1,fs[index]->keyframed(),
						  (float)fs[index]->minVal(),
						  (float)fs[index]->maxVal(),
						  (float)fs[index]->val()));
      break;
    case WidgetFactory::IntID:
      _items.push_back(new GenericCustomParamItem(i,is[index]->name(),WidgetFactory::IntID,
						  is[index]->type()-1,is[index]->keyframed(),
						  (float)is[index]->minVal(),
						  (float)is[index]->maxVal(),
						  (float)is[index]->val()));
      break;
    case WidgetFactory::Vector2fID:
      _items.push_back(new GenericCustomParamItem(i,v2fs[index]->name(),WidgetFactory::Vector2fID,
						  v2fs[index]->type()-1,v2fs[index]->keyframed(),
						  v2fs[index]->minVal()[0],
						  v2fs[index]->maxVal()[0],
						  v2fs[index]->val()[0]));
      break;
    case WidgetFactory::Vector2iID:
      _items.push_back(new GenericCustomParamItem(i,v2is[index]->name(),WidgetFactory::Vector2iID,
						  v2is[index]->type()-1,v2is[index]->keyframed(),
						  (float)(v2is[index]->minVal()[0]),
						  (float)(v2is[index]->maxVal()[0]),
						  (float)(v2is[index]->val()[0])));
      break;
    case WidgetFactory::Vector3fID:
      _items.push_back(new GenericCustomParamItem(i,v3fs[index]->name(),WidgetFactory::Vector3fID,
						  v3fs[index]->type()-1,v3fs[index]->keyframed(),
						  v3fs[index]->minVal()[0],
						  v3fs[index]->maxVal()[0],
						  v3fs[index]->val()[0]));
      break;
    case WidgetFactory::Vector3iID:
      _items.push_back(new GenericCustomParamItem(i,v3is[index]->name(),WidgetFactory::Vector3iID,
						  v3is[index]->type()-1,v3is[index]->keyframed(),
						  (float)(v3is[index]->minVal()[0]),
						  (float)(v3is[index]->maxVal()[0]),
						  (float)(v3is[index]->val()[0])));
      break;
    case WidgetFactory::Vector4fID:
      _items.push_back(new GenericCustomParamItem(i,v4fs[index]->name(),WidgetFactory::Vector4fID,
						  v4fs[index]->type()-1,v4fs[index]->keyframed(),
						  v4fs[index]->minVal()[0],
						  v4fs[index]->maxVal()[0],
						  v4fs[index]->val()[0]));
      break;
    case WidgetFactory::Vector4iID:
      _items.push_back(new GenericCustomParamItem(i,v4is[index]->name(),WidgetFactory::Vector4iID,
						  v4is[index]->type()-1,v4is[index]->keyframed(),
						  (float)(v4is[index]->minVal()[0]),
						  (float)(v4is[index]->maxVal()[0]),
						  (float)(v4is[index]->val()[0])));
      break;

    default:
      continue;
    }
    
    connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
    connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
    connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  }



  // vector<FloatAttribWidget *> fs = _nodeWidget->floatWidgets();
  // for(unsigned int i=0;i<fs.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,fs[i]->name(),WidgetFactory::FloatID,
  // 						fs[i]->type()-1,fs[i]->keyframed(),
  // 						(float)fs[i]->minVal(),
  // 						(float)fs[i]->maxVal(),
  // 						(float)fs[i]->val()));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<IntAttribWidget *> is = _nodeWidget->intWidgets();
  // for(unsigned int i=0;i<is.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,is[i]->name(),WidgetFactory::IntID,
  // 						is[i]->type()-1,is[i]->keyframed(),
  // 						(float)is[i]->minVal(),
  // 						(float)is[i]->maxVal(),
  // 						(float)is[i]->val()));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector2fAttribWidget *> v2fs = _nodeWidget->vector2fWidgets();
  // for(unsigned int i=0;i<v2fs.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v2fs[i]->name(),WidgetFactory::Vector2fID,
  // 						v2fs[i]->type()-1,v2fs[i]->keyframed(),
  // 						v2fs[i]->minVal()[0],
  // 						v2fs[i]->maxVal()[0],
  // 						v2fs[i]->val()[0]));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector2iAttribWidget *> v2is = _nodeWidget->vector2iWidgets();
  // for(unsigned int i=0;i<v2is.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v2is[i]->name(),WidgetFactory::Vector2iID,
  // 						v2is[i]->type()-1,v2is[i]->keyframed(),
  // 						(float)(v2is[i]->minVal()[0]),
  // 						(float)(v2is[i]->maxVal()[0]),
  // 						(float)(v2is[i]->val()[0])));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector3fAttribWidget *> v3fs = _nodeWidget->vector3fWidgets();
  // for(unsigned int i=0;i<v3fs.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v3fs[i]->name(),WidgetFactory::Vector3fID,
  // 						v3fs[i]->type()-1,v3fs[i]->keyframed(),
  // 						v3fs[i]->minVal()[0],
  // 						v3fs[i]->maxVal()[0],
  // 						v3fs[i]->val()[0]));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector3iAttribWidget *> v3is = _nodeWidget->vector3iWidgets();
  // for(unsigned int i=0;i<v3is.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v3is[i]->name(),WidgetFactory::Vector3iID,
  // 						v3is[i]->type()-1,v3is[i]->keyframed(),
  // 						(float)(v3is[i]->minVal()[0]),
  // 						(float)(v3is[i]->maxVal()[0]),
  // 						(float)(v3is[i]->val()[0])));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector4fAttribWidget *> v4fs = _nodeWidget->vector4fWidgets();
  // for(unsigned int i=0;i<v4fs.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v4fs[i]->name(),WidgetFactory::Vector4fID,
  // 						v4fs[i]->type()-1,v4fs[i]->keyframed(),
  // 						v4fs[i]->minVal()[0],
  // 						v4fs[i]->maxVal()[0],
  // 						v4fs[i]->val()[0]));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

  // vector<Vector4iAttribWidget *> v4is = _nodeWidget->vector4iWidgets();
  // for(unsigned int i=0;i<v4is.size();++i) {
  //   _items.push_back(new GenericCustomParamItem(i,v4is[i]->name(),WidgetFactory::Vector4iID,
  // 						v4is[i]->type()-1,v4is[i]->keyframed(),
  // 						(float)(v4is[i]->minVal()[0]),
  // 						(float)(v4is[i]->maxVal()[0]),
  // 						(float)(v4is[i]->val()[0])));
  //   connect(_items[i],SIGNAL(upClicked(GenericCustomParamItem *)),this,SLOT(itemUp(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(downClicked(GenericCustomParamItem *)),this,SLOT(itemDown(GenericCustomParamItem *)));
  //   connect(_items[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(itemDel(GenericCustomParamItem *)));
  // }

}

void ParamList::cleanItems() {
  for(unsigned int i=0;i<_items.size();++i) {
    delete _items[i];
  }
  _items.clear();
}



PropertyList::PropertyList(const QString &title,GenericCustomWidget *nodeWidget)
  : QGroupBox(title),
    _nodeWidget(nodeWidget),
    _name(new QLineEdit()),
    _xSize(new QLineEdit()),
    _ySize(new QLineEdit()),
    _xOffset(new QLineEdit()),
    _yOffset(new QLineEdit()),
    _filter(new QComboBox()),
    _wrap(new QComboBox()),
    _mouse(new QCheckBox("Mouse")),
    _keyboard(new QCheckBox("Keyboard")),
    _vertex(new QCheckBox("Vertex")),
    _tesselation(new QCheckBox("Tesselation")),
    _geometry(new QCheckBox("Geometry")),
    _fragment(new QCheckBox("Fragment")),
    _glslVersion(new QLineEdit()),
    _depth(new QCheckBox("Depth test")),
    _blend(new QCheckBox("Blending")),
    _depthFunc(new QComboBox()),
    _blendSrc(new QComboBox()),
    _blendDst(new QComboBox()),
    _background(new QPushButton()),
    _color(0.0f,0.0f,0.0f,0.0f) {

  const Vector2f tsize   = _nodeWidget->tsize();
  const Vector2f toffset = _nodeWidget->toffset();

  _name->setText(nodeWidget->nodeName());
  _xSize->setText(QString::number(tsize[0]));
  _ySize->setText(QString::number(tsize[1]));
  _xOffset->setText(QString::number(toffset[0]));
  _yOffset->setText(QString::number(toffset[1]));

  _filter->addItems((QStringList() << "LINEAR" << "NEAREST"));
  _wrap->addItems((QStringList() << "CLAMP_TO_EDGE" << "REPEAT" << "MIRRORED_REPEAT" << "CLAMP_TO_BORDER"));
  _depthFunc->addItems((QStringList() << "LESS" << "EQUAL" << "LEQUAL" << "GREATER" << "NOTEQUAL" << "GEQUAL" << "NEVER" << "ALWAYS"));
  QStringList blendItems = QStringList() << "ZERO" << "ONE" << "SRC_COLOR" << "ONE_MINUS_SRC_COLOR" << "DST_COLOR" << "ONE_MINUS_DST_COLOR" << "SRC_ALPHA" << "ONE_MINUS_SRC_ALPHA" << "DST_ALPHA" << "ONE_MINUS_DST_ALPHA";

  _blendSrc->addItems(blendItems);
  _blendDst->addItems(blendItems);

  _filter->setCurrentIndex(indexFromFilterMode(_nodeWidget->filterMode()));
  _wrap->setCurrentIndex(indexFromWrapMode(_nodeWidget->wrapMode()));
  _depthFunc->setCurrentIndex(indexFromDepthFunc(_nodeWidget->depthFunc()));
  _blendSrc->setCurrentIndex(indexFromBlendMode(_nodeWidget->blendSrc()));
  _blendDst->setCurrentIndex(indexFromBlendMode(_nodeWidget->blendDst()));
  _color = _nodeWidget->background();
  setButtonColor(_color);

  if(_nodeWidget->useDepthTest()) _depth->setCheckState(Qt::Checked);
  else _depth->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useBlending()) _blend->setCheckState(Qt::Checked);
  else _blend->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useMouse()) _mouse->setCheckState(Qt::Checked);
  else _mouse->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useKeyboard()) _keyboard->setCheckState(Qt::Checked);
  else _keyboard->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useVertex()) _vertex->setCheckState(Qt::Checked);
  else _vertex->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useTesselation()) _tesselation->setCheckState(Qt::Checked);
  else _tesselation->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useGeometry()) _geometry->setCheckState(Qt::Checked);
  else _geometry->setCheckState(Qt::Unchecked);

  if(_nodeWidget->useFragment()) _fragment->setCheckState(Qt::Checked);
  else _fragment->setCheckState(Qt::Unchecked);

  _glslVersion->setText(_nodeWidget->glslVersion());

  QHBoxLayout *hlayout;
  QVBoxLayout *vlayout = new QVBoxLayout();

  hlayout = new QHBoxLayout();
  hlayout->addWidget(new QLabel("Name"));
  hlayout->addWidget(_name);
  hlayout->addWidget(_mouse);
  hlayout->addWidget(_keyboard);
  vlayout->addLayout(hlayout);

  hlayout = new QHBoxLayout();
  hlayout->addWidget(new QLabel("Size"));
  hlayout->addWidget(_xSize);
  hlayout->addWidget(new QLabel("W +"));
  hlayout->addWidget(_xOffset);
  hlayout->addWidget(new QLabel(" , "));
  hlayout->addWidget(_ySize);
  hlayout->addWidget(new QLabel("H +"));
  hlayout->addWidget(_yOffset);
  vlayout->addLayout(hlayout);

  hlayout = new QHBoxLayout();
  hlayout->addWidget(_depth);
  hlayout->addWidget(_depthFunc);
  hlayout->addWidget(_blend);
  hlayout->addWidget(_blendSrc);
  hlayout->addWidget(_blendDst);
  vlayout->addLayout(hlayout);

  hlayout = new QHBoxLayout();
  hlayout->addWidget(new QLabel("Filtering"));
  hlayout->addWidget(_filter);
  hlayout->addWidget(new QLabel("Wrapping"));
  hlayout->addWidget(_wrap);
  hlayout->addWidget(new QLabel("Background"));
  hlayout->addWidget(_background);
  vlayout->addLayout(hlayout);

  hlayout = new QHBoxLayout();
  hlayout->addWidget(new QLabel("Shaders"));
  hlayout->addWidget(_vertex);
  hlayout->addWidget(_tesselation);
  hlayout->addWidget(_geometry);
  hlayout->addWidget(_fragment);
  hlayout->addWidget(new QLabel("GLSL"));
  hlayout->addWidget(_glslVersion);
  vlayout->addLayout(hlayout);

  setLayout(vlayout);

  connect(_background,SIGNAL(clicked()),this,SLOT(colorClicked()));

  // QGridLayout *g = new QGridLayout();
  // g->addWidget(new QLabel("Name"),0,0,1,2);
  // g->addWidget(_name,0,2,1,6);

  // g->addWidget(new QLabel("Size"),1,0);
  // g->addWidget(_xSize,1,1);
  // g->addWidget(new QLabel("W +"),1,2);
  // g->addWidget(_xOffset,1,3);
  // g->addWidget(new QLabel(" , "),1,4);
  // g->addWidget(_ySize,1,5);
  // g->addWidget(new QLabel("H +"),1,6);
  // g->addWidget(_yOffset,1,7);

  // g->addWidget(new QLabel("Filtering"),2,0,1,1);
  // g->addWidget(_filter,2,1,1,2);
  // g->addWidget(new QLabel("Wrapping"),2,3,1,1);
  // g->addWidget(_wrap,2,4,1,2);
  // g->addWidget(_mouse,2,6,1,1);
  // g->addWidget(_keyboard,2,7,1,1);

  // g->addWidget(new QLabel("Shaders"),3,0,1,1);
  // g->addWidget(_vertex,3,1,1,1);
  // g->addWidget(_tesselation,3,2,1,1);
  // g->addWidget(_geometry,3,3,1,1);
  // g->addWidget(_fragment,3,4,1,1);
  // g->addWidget(new QLabel("GLSL"),3,5,1,1);
  // g->addWidget(_glslVersion,3,6,1,2);

  // setLayout(g);
}

void PropertyList::colorClicked() {
  QColor init((int)(_color[0]*255.0f),
	      (int)(_color[1]*255.0f),
	      (int)(_color[2]*255.0f),
	      (int)(_color[3]*255.0f));

  QColor c = QColorDialog::getColor(init,this,"Background color",QColorDialog::ShowAlphaChannel);
  if(!c.isValid()) return;
  
  _color[0] = c.redF();
  _color[1] = c.blueF();
  _color[2] = c.greenF();
  _color[3] = c.alphaF();
  setButtonColor(_color);
}


int PropertyList::indexFromFilterMode(GLint mode) {
  switch(mode) {
  case GL_LINEAR:  return 0; break;
  case GL_NEAREST: return 1; break;
  default: return 0; break;
  }
}

int PropertyList::indexFromWrapMode(GLint mode) {
  switch(mode) {
  case GL_CLAMP_TO_EDGE:   return 0; break;
  case GL_REPEAT:          return 1; break;
  case GL_MIRRORED_REPEAT: return 2; break;
  case GL_CLAMP_TO_BORDER: return 3; break;
  default: return 0; break;
  }
}

int PropertyList::indexFromDepthFunc(GLenum func) {
  switch(func) {
  case GL_LESS:     return 0; break;
  case GL_EQUAL:    return 1; break;
  case GL_LEQUAL:   return 2; break;
  case GL_GREATER:  return 3; break;
  case GL_NOTEQUAL: return 4; break;
  case GL_GEQUAL:   return 5; break;
  case GL_NEVER:    return 6; break;
  case GL_ALWAYS:   return 7; break;
  default: return 0; break;
  }
}

int PropertyList::indexFromBlendMode(GLenum mode) {
  switch(mode) {
  case GL_ZERO:                return 0; break;
  case GL_ONE:                 return 1; break;
  case GL_SRC_COLOR:           return 2; break;
  case GL_ONE_MINUS_SRC_COLOR: return 3; break;
  case GL_DST_COLOR:           return 4; break;
  case GL_ONE_MINUS_DST_COLOR: return 5; break;
  case GL_SRC_ALPHA:           return 6; break;
  case GL_ONE_MINUS_SRC_ALPHA: return 7; break;
  case GL_DST_ALPHA:           return 8; break;
  case GL_ONE_MINUS_DST_ALPHA: return 9; break;
  default: return 0; break;
  }
}

GLint PropertyList::filterMode() {
  int ind = _filter->currentIndex();
  switch(ind) {
  case 0: return GL_LINEAR; break;
  case 1: return GL_NEAREST;break;
  default: return GL_LINEAR; break;
  }
}

GLint PropertyList::wrapMode() {
  int ind = _wrap->currentIndex();
  switch(ind) {
  case 0: return GL_CLAMP_TO_EDGE;break;
  case 1: return GL_REPEAT;break;
  case 2: return GL_MIRRORED_REPEAT;break;
  case 3: return GL_CLAMP_TO_BORDER;break;
  default: return GL_CLAMP_TO_EDGE; break;
  }
}

GLenum PropertyList::depthFunc() {
  int ind = _depthFunc->currentIndex();
  switch(ind) {
  case 0:  return GL_LESS; break;
  case 1:  return GL_EQUAL; break;
  case 2:  return GL_LEQUAL; break;
  case 3:  return GL_GREATER; break;
  case 4:  return GL_NOTEQUAL; break;
  case 5:  return GL_GEQUAL; break;
  case 6:  return GL_NEVER; break;
  case 7:  return GL_ALWAYS; break;
  default: return GL_LESS; break;
  }
}

GLenum PropertyList::blendSrc() {
  int ind = _blendSrc->currentIndex();
  switch(ind) {
  case 0: return GL_ZERO; break;
  case 1: return GL_ONE; break;
  case 2: return GL_SRC_COLOR; break;
  case 3: return GL_ONE_MINUS_SRC_COLOR; break;
  case 4: return GL_DST_COLOR; break;
  case 5: return GL_ONE_MINUS_DST_COLOR; break;
  case 6: return GL_SRC_ALPHA; break;
  case 7: return GL_ONE_MINUS_SRC_ALPHA; break;
  case 8: return GL_DST_ALPHA; break;
  case 9: return GL_ONE_MINUS_DST_ALPHA; break;
  default: return GL_ONE; break;
  }
}

GLenum PropertyList::blendDst() {
  int ind = _blendDst->currentIndex();
  switch(ind) {
  case 0: return GL_ZERO; break;
  case 1: return GL_ONE; break;
  case 2: return GL_SRC_COLOR; break;
  case 3: return GL_ONE_MINUS_SRC_COLOR; break;
  case 4: return GL_DST_COLOR; break;
  case 5: return GL_ONE_MINUS_DST_COLOR; break;
  case 6: return GL_SRC_ALPHA; break;
  case 7: return GL_ONE_MINUS_SRC_ALPHA; break;
  case 8: return GL_DST_ALPHA; break;
  case 9: return GL_ONE_MINUS_DST_ALPHA; break;
  default: return GL_ZERO; break;
  }
}

const Vector2f PropertyList::tsize(bool *ok) const {
  Vector2f tsize;
  tsize[0]   = _xSize->text().toFloat(ok);
  tsize[1]   = _ySize->text().toFloat(ok);
  return tsize;
}

const Vector2f PropertyList::toffset(bool *ok) const {
  Vector2f toffset;
  toffset[0] = _xOffset->text().toFloat(ok);
  toffset[1] = _yOffset->text().toFloat(ok);
  return toffset;
}

GenericCustomWidgetSetting::GenericCustomWidgetSetting(GenericCustomWidget *nodeWidget)
  : _nodeWidget(nodeWidget),
  _propertyBox(new PropertyList("Properties",_nodeWidget)),
  _inputBox(new IOList("Inputs",_nodeWidget->inputNames(),"inBuffer",_nodeWidget->nodePartiallyConnected())),
  _outputBox(new IOList("Outputs",_nodeWidget->outputNames(),"outBuffer",_nodeWidget->nodePartiallyConnected())),
  _paramBox(new ParamList("Parameters",_nodeWidget)),
  _title(new QLabel("<center>Node parameters<br>Warning: this action will remove previously defined keyframes</center>")),
  _buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)) {

  QVBoxLayout *mainLayout = new QVBoxLayout();

  mainLayout->addWidget(_title);
  mainLayout->addWidget(_propertyBox);
  mainLayout->addWidget(_inputBox);
  mainLayout->addWidget(_outputBox);
  mainLayout->addWidget(_paramBox);
  mainLayout->addWidget(_buttons);

  QScrollArea *area = new QScrollArea();
  area->setWidgetResizable(true);
  QWidget *tmp = new QWidget();
  tmp->setLayout(mainLayout);
  area->setWidget(tmp);
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(area);
  setLayout(l);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));
  resize(800,800);
}

GenericCustomWidgetSetting::~GenericCustomWidgetSetting() {

}

std::vector<GenericCustomIOItem *> GenericCustomWidgetSetting::inputItems() const {
  return _inputBox->items();
}

std::vector<GenericCustomIOItem *> GenericCustomWidgetSetting::outputItems() const {
  return _outputBox->items();
}

std::vector<GenericCustomParamItem *> GenericCustomWidgetSetting::paramItems() const {
  return _paramBox->items();
}

bool GenericCustomWidgetSetting::checkParams() {
  // check if the output size is null
  bool ok = true;
  Vector2f ts = tsize(&ok);
  Vector2f to = toffset(&ok);

  vector<GenericCustomIOItem *> inputItems = _inputBox->items();
  vector<GenericCustomIOItem *> outputItems = _outputBox->items();
  vector<GenericCustomParamItem *> paramItems = _paramBox->items();
  
  if(!ok) {
    GERROR("Output size must contain valid numbers.");
    return false;
  }

  if((inputItems.empty() || ts==Vector2f(0,0)) && to==Vector2f(0,0)) {
    GERROR("Output size must not be zero");
    return false;
  }

  if(outputItems.empty()) {
    GERROR("At least one output image needed");
    return false;
  }

  QStringList names;
  for(unsigned int i=0;i<inputItems.size();++i) {
    names.push_back(inputItems[i]->name());
  }

  for(unsigned int i=0;i<outputItems.size();++i) {
    names.push_back(outputItems[i]->name());
  }

  for(unsigned int i=0;i<paramItems.size();++i) {
    names.push_back(paramItems[i]->name());
  }

  if(names.removeDuplicates()>0) {
    GERROR("At least two parameters/inputs/outputs have the same name");
    return false;
  }

  for(int i=0;i<names.size();++i) {
    // non exhaustive list of special caracters
    if(names[i].contains(' ')) {
      GERROR("Names must not contain spaces");
      return false;
    }
  }

  if(outputItems.empty()) {
    GERROR("The node should contain at least one output");
    return false;
  }

  return true;
}

void GenericCustomWidgetSetting::confirmClicked() {
  if(!checkParams()) return;

  accept();
}

void GenericCustomWidgetSetting::cancelClicked() {
  reject();
}


