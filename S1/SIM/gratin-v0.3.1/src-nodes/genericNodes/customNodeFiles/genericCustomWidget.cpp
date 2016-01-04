// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "genericCustomWidget.h"
#include "genericCustomNode.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

using namespace std;

GenericCustomWidget::GenericCustomWidget(GenericCustomNode *node)
  : NodeWidget(node),
    _customNode(node),
    _outFilter(GL_LINEAR),
    _outWrap(GL_CLAMP_TO_EDGE),
    _useDepthTest(false),
    _useBlending(false),
    _depthFunc(GL_LESS),
    _blendSrc(GL_ONE),
    _blendDst(GL_ZERO),
    _background(0.0f,0.0f,0.0f,0.0f),
    _userLayout(new QVBoxLayout()),
    _mouseWidget(NULL),
    _keyboardWidget(NULL),
    _vertHead(new QLabel()),
    _vertSource(new CodeEditor()),
    _tessCHead(new QLabel()),
    _tessCSource(new CodeEditor()),
    _tessEHead(new QLabel()),
    _tessESource(new CodeEditor()),
    _geomHead(new QLabel()),
    _geomSource(new CodeEditor()),
    _fragHead(new QLabel()),
    _fragSource(new CodeEditor()),
    _log(new QLabel("")),
    _settings(new QPushButton("Settings...")),
    _apply(new QPushButton("Apply")),
    _userArea(new QScrollArea()),
    _vertArea(new QScrollArea()),
    _tessCArea(new QScrollArea()),
    _tessEArea(new QScrollArea()),
    _geomArea(new QScrollArea()),
    _fragArea(new QScrollArea()),
    _logArea(new QScrollArea()),
    _tabWidget(new QTabWidget()) {

  QWidget     *tmp;
  QVBoxLayout *l;
  setMinimumHeight(500);

  // tab 
  _userArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(userLayout());
  _userArea->setWidget(tmp);

  // tab
  _vertArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_vertHead);
  l->addWidget(_vertSource);
  tmp->setLayout(l);
  _vertArea->setWidget(tmp);

  // tab
  _tessCArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_tessCHead);
  l->addWidget(_tessCSource);
  tmp->setLayout(l);
  _tessCArea->setWidget(tmp);

  // tab
  _tessEArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_tessEHead);
  l->addWidget(_tessESource);
  tmp->setLayout(l);
  _tessEArea->setWidget(tmp);

  // tab
  _geomArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_geomHead);
  l->addWidget(_geomSource);
  tmp->setLayout(l);
  _geomArea->setWidget(tmp);

  // tab
  _fragArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_fragHead);
  l->addWidget(_fragSource);
  tmp->setLayout(l);
  _fragArea->setWidget(tmp);

  // tab
  _logArea->setWidgetResizable(true);
  _logArea->setWidget(_log);

  l = new QVBoxLayout();
  l->addWidget(_settings);
  l->addWidget(_tabWidget);
  l->addWidget(_apply);
  setLayout(l);

  connect(_settings,SIGNAL(clicked()),this,SLOT(settingsClicked()));
  connect(_apply,SIGNAL(clicked()),this,SLOT(applyClicked()));
}

void GenericCustomWidget::updateWidget() {
  setVertSource(_customNode->vertHead(),_customNode->vertBody());
  setTessCSource(_customNode->tessCHead(),_customNode->tessCBody());
  setTessESource(_customNode->tessEHead(),_customNode->tessEBody());
  setGeomSource(_customNode->geomHead(),_customNode->geomBody());
  setFragSource(_customNode->fragHead(),_customNode->fragBody());
  _outFilter      = _customNode->outFilter();
  _outWrap        = _customNode->outWrap();
  _glslVersion    = _customNode->glslVersion();
  _useVertex      = _customNode->useVertex();
  _useTesselation = _customNode->useTesselation();
  _useGeometry    = _customNode->useGeometry();
  _useFragment    = _customNode->useFragment();
  _useDepthTest   = _customNode->useDepthTest();
  _useBlending    = _customNode->useBlending();
  _depthFunc      = _customNode->depthFunc();
  _blendSrc       = _customNode->blendSrc();
  _blendDst       = _customNode->blendDst();
  _background     = _customNode->background();

  _tabWidget->clear();
  _tabWidget->addTab(_userArea,"Params");

  if(_customNode->useVertex())
    _tabWidget->addTab(_vertArea,"Vertex");

  if(_customNode->useTesselation()) {
    _tabWidget->addTab(_tessCArea,"Tess Control");
    _tabWidget->addTab(_tessEArea,"Tess Eval");
  }

  if(_customNode->useGeometry())
    _tabWidget->addTab(_geomArea,"Geometry");

  if(_customNode->useFragment())
    _tabWidget->addTab(_fragArea,"Fragment");

  _tabWidget->addTab(_logArea,"Log");
}

void GenericCustomWidget::settingsClicked() {
  // save changes

  applyClicked();

  // update parameters for the settings
  _useVertex      = _customNode->useVertex();
  _useTesselation = _customNode->useTesselation();
  _useGeometry    = _customNode->useGeometry();
  _useFragment    = _customNode->useFragment();
  _glslVersion    = _customNode->glslVersion();
  _tmpName        = _customNode->name();
  _outFilter      = _customNode->outFilter();
  _outWrap        = _customNode->outWrap();
  _useDepthTest   = _customNode->useDepthTest();
  _useBlending    = _customNode->useBlending();
  _depthFunc      = _customNode->depthFunc();
  _blendSrc       = _customNode->blendSrc();
  _blendDst       = _customNode->blendDst();
  _background     = _customNode->background();

  GenericCustomWidgetSetting settings(this);

  int ret = settings.exec();
  if(ret==QDialog::Rejected)
    return;

  // remove everything from the animation widget 
  AnimationWidget::instance()->delParams(this);

  initFromSettings(settings);

  _customNode->settingsChanged();
  updateWidget();
  updateGraph();
}

void GenericCustomWidget::applyClicked(bool update) {
  QString vbody  = _vertSource->toPlainText();
  QString tcbody = _tessCSource->toPlainText();
  QString tebody = _tessESource->toPlainText();
  QString gbody  = _geomSource->toPlainText();
  QString fbody  = _fragSource->toPlainText();
  _customNode->sourceChanged(vbody,fbody,tcbody,tebody,gbody);
  if(update)
    updateGraph();
}

void GenericCustomWidget::setLogFromProg(GPUProgram *p) {
  if(!p) return;

  QString l;
  
  if(p->haveShaderOfType(VERT)) {
    QString v = p->log(VERT);  
    l = l+"Vertex shader:\n"+v+"\n\n";
  }

  if(p->haveShaderOfType(TESSC)) {
    QString v = p->log(TESSC);  
    l = l+"Tesselation Control shader:\n"+v+"\n\n";
  }

  if(p->haveShaderOfType(TESSE)) {
    QString v = p->log(TESSE);  
    l = l+"Tesselation Eval shader:\n"+v+"\n\n";
  }

  if(p->haveShaderOfType(GEOM)) {
    QString v = p->log(GEOM);  
    l = l+"Geometry shader:\n"+v+"\n\n";
  }

  if(p->haveShaderOfType(FRAG)) {
    QString v = p->log(FRAG);  
    l = l+"Fragment shader:\n"+v+"\n\n";
  }

  _log->setText(l);
}


QString GenericCustomWidget::generateOutLocationsHead() {
  QString head;

  const std::vector<QString> onames = outputNames();
  for(unsigned int i=0;i<onames.size();++i) {
    head = head+tr("layout(location = ")+QString::number(i)+tr(") out vec4 ")+onames[i]+tr(";\n");
  }
  
  return head;
}

QString GenericCustomWidget::generateUniformsHead() {
  QString head;

  const std::vector<QString> inames = inputNames();
  for(unsigned int i=0;i<inames.size();++i) {
    head = head+tr("uniform sampler2D ")+inames[i]+tr(";\n");
  }
  
  if(useMouse()) {
    head = head+tr("uniform vec2 ")+_mouseWidget->name()+tr(";\n");
  }

  if(useKeyboard()) {
    head = head+tr("uniform int ")+_keyboardWidget->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_floatWidgets.size();++i) {
    head = head+tr("uniform float ")+_floatWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_intWidgets.size();++i) {
    head = head+tr("uniform int ")+_intWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
    head = head+tr("uniform vec2 ")+_vector2fWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
    head = head+tr("uniform ivec2 ")+_vector2iWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
    head = head+tr("uniform vec3 ")+_vector3fWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
    head = head+tr("uniform ivec3 ")+_vector3iWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
    head = head+tr("uniform vec4 ")+_vector4fWidgets[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
    head = head+tr("uniform ivec4 ")+_vector4iWidgets[i]->name()+tr(";\n");
  }

  return head;
}

void GenericCustomWidget::sendUniformsToProg(GPUProgram *p) {
  if(useMouse()) {
    Vector2f v((float)_mouseWidget->val()[0],(float)_mouseWidget->val()[1]);
    p->setUniform2fv(_mouseWidget->name(),v.data());
  }

  if(useKeyboard()) {
    p->setUniform1i(_keyboardWidget->name(),_keyboardWidget->val());
  }

  for(unsigned int i=0;i<_floatWidgets.size();++i) {
    p->setUniform1f(_floatWidgets[i]->name(),_floatWidgets[i]->val());
  }

  for(unsigned int i=0;i<_intWidgets.size();++i) {
    p->setUniform1i(_intWidgets[i]->name(),_intWidgets[i]->val());
  }

  for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
    p->setUniform2fv(_vector2fWidgets[i]->name(),_vector2fWidgets[i]->val().data());
  }

  for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
    p->setUniform2iv(_vector2iWidgets[i]->name(),_vector2iWidgets[i]->val().data());
  }

  for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
    p->setUniform3fv(_vector3fWidgets[i]->name(),_vector3fWidgets[i]->val().data());
  }

  for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
    p->setUniform3iv(_vector3iWidgets[i]->name(),_vector3iWidgets[i]->val().data());
  }

  for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
    p->setUniform4fv(_vector4fWidgets[i]->name(),_vector4fWidgets[i]->val().data());
  }

  for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
    p->setUniform4iv(_vector4iWidgets[i]->name(),_vector4iWidgets[i]->val().data());
  }
}

const QStringList GenericCustomWidget::getUniformNames() {
  QStringList l;
  const std::vector<QString> inames = inputNames();

  for(unsigned int i=0;i<inames.size();++i) {
    l.push_back(inames[i]);
  }

  if(useMouse()) {
    l.push_back(_mouseWidget->name());
  }

  if(useKeyboard()) {
    l.push_back(_keyboardWidget->name());
  }

  for(unsigned int i=0;i<_floatWidgets.size();++i) {
    l.push_back(_floatWidgets[i]->name());
  }

  for(unsigned int i=0;i<_intWidgets.size();++i) {
    l.push_back(_intWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
    l.push_back(_vector2fWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
    l.push_back(_vector2iWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
    l.push_back(_vector3fWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
    l.push_back(_vector3iWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
    l.push_back(_vector4fWidgets[i]->name());
  }

  for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
    l.push_back(_vector4iWidgets[i]->name());
  }

  return l;
}

const std::vector<QString> GenericCustomWidget::inputNames() {
  unsigned int nb = _customNode->nbInputs();
  QStringList l = _customNode->inputNames();

  vector<QString> names;
  for(unsigned i=0;i<nb;++i) {
    QString current = l[i];
    current.remove(QChar(' ')); // remove spaces (just in case)
    names.push_back(current);
  }

  return names;
}

const std::vector<QString> GenericCustomWidget::outputNames() {
  unsigned int nb = _customNode->nbOutputs();
  QStringList l = _customNode->outputNames();

  vector<QString> names;
  for(unsigned i=0;i<nb;++i) {
    QString current = l[i];
    current.remove(QChar(' ')); // remove spaces (just in case)
    names.push_back(current);
  }
  
  return names;
}

bool GenericCustomWidget::nodePartiallyConnected() const {
  return _customNode->isPartlyConnected();
}

Vector2f GenericCustomWidget::tsize  () const {
  return _customNode->tsize();
}

Vector2f GenericCustomWidget::toffset() const {
  return _customNode->toffset();
}


void GenericCustomWidget::cleanLayout() {
  if(useMouse()) 
    _userLayout->removeWidget(_mouseWidget);

  if(useKeyboard()) 
    _userLayout->removeWidget(_keyboardWidget);

  for(unsigned int i=0;i<_mapWidgets.size();++i) {
    switch(_mapWidgets[i].first) {
    case WidgetFactory::FloatID:
      _userLayout->removeWidget(_floatWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::IntID:
      _userLayout->removeWidget(_intWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector2fID:
      _userLayout->removeWidget(_vector2fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector2iID:
      _userLayout->removeWidget(_vector2iWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector3fID:
      _userLayout->removeWidget(_vector3fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector3iID:
      _userLayout->removeWidget(_vector3iWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector4fID:
      _userLayout->removeWidget(_vector4fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector4iID:
      _userLayout->removeWidget(_vector4iWidgets[_mapWidgets[i].second]); break;
    default:break;
    }
  }

  // for(unsigned int i=0;i<_floatWidgets.size();++i) {
  //   _userLayout->removeWidget(_floatWidgets[i]);
  // }

  // for(unsigned int i=0;i<_intWidgets.size();++i) {
  //   _userLayout->removeWidget(_intWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector2fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector2iWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector3fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector3iWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector4fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
  //   _userLayout->removeWidget(_vector4iWidgets[i]);
  // }
}

void GenericCustomWidget::initLayout() {
  if(useMouse()) {
    _userLayout->addWidget(_mouseWidget);
  }

  if(useKeyboard()) {
    _userLayout->addWidget(_keyboardWidget);
  }

  for(unsigned int i=0;i<_mapWidgets.size();++i) {
    switch(_mapWidgets[i].first) {
    case WidgetFactory::FloatID:
      _userLayout->addWidget(_floatWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::IntID:
      _userLayout->addWidget(_intWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector2fID:
      _userLayout->addWidget(_vector2fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector2iID:
      _userLayout->addWidget(_vector2iWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector3fID:
      _userLayout->addWidget(_vector3fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector3iID:
      _userLayout->addWidget(_vector3iWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector4fID:
      _userLayout->addWidget(_vector4fWidgets[_mapWidgets[i].second]); break;
    case WidgetFactory::Vector4iID:
      _userLayout->addWidget(_vector4iWidgets[_mapWidgets[i].second]); break;
    default:break;
    }
  }

  // for(unsigned int i=0;i<_floatWidgets.size();++i) {
  //   _userLayout->addWidget(_floatWidgets[i]);
  // }

  // for(unsigned int i=0;i<_intWidgets.size();++i) {
  //   _userLayout->addWidget(_intWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
  //   _userLayout->addWidget(_vector2fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
  //   _userLayout->addWidget(_vector2iWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
  //   _userLayout->addWidget(_vector3fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
  //   _userLayout->addWidget(_vector3iWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
  //   _userLayout->addWidget(_vector4fWidgets[i]);
  // }

  // for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
  //   _userLayout->addWidget(_vector4iWidgets[i]);
  // }
}

void GenericCustomWidget::cleanWidgets() {

  // mouse 
  delChildWidget(_mouseWidget);
  delChildWidget(_keyboardWidget);
  delete _mouseWidget;
  delete _keyboardWidget;
  _mouseWidget    = NULL;
  _keyboardWidget = NULL;

  // floats
  for(unsigned int i=0;i<_floatWidgets.size();++i) {
    delChildWidget(_floatWidgets[i]);
    delete _floatWidgets[i];
  }

  // ints
  for(unsigned int i=0;i<_intWidgets.size();++i) {
    delChildWidget(_intWidgets[i]);
    delete _intWidgets[i];
  }

  // vector2f
  for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
    delChildWidget(_vector2fWidgets[i]);
    delete _vector2fWidgets[i];
  }

  // vector2i
  for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
    delChildWidget(_vector2iWidgets[i]);
    delete _vector2iWidgets[i];
  }

  // vector3f
  for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
    delChildWidget(_vector3fWidgets[i]);
    delete _vector3fWidgets[i];
  }

  // vector3i
  for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
    delChildWidget(_vector3iWidgets[i]);
    delete _vector3iWidgets[i];
  }

  // vector4f
  for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
    delChildWidget(_vector4fWidgets[i]);
    delete _vector4fWidgets[i];
  }

  // vector4i
  for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
    delChildWidget(_vector4iWidgets[i]);
    delete _vector4iWidgets[i];
  }

  _floatWidgets.clear();
  _intWidgets.clear();
  _vector2fWidgets.clear();
  _vector2iWidgets.clear();
  _vector3fWidgets.clear();
  _vector3iWidgets.clear();
  _vector4fWidgets.clear();
  _vector4iWidgets.clear();
  _mapWidgets.clear();
}



void GenericCustomWidget::initFromSettings(const GenericCustomWidgetSetting &settings) {

  // check if the number of inputs/outputs has changed
  std::vector<GenericCustomIOItem *> iitems = settings.inputItems();
  std::vector<GenericCustomIOItem *> oitems = settings.outputItems();

  unsigned int nbIn  = static_cast<unsigned int>(iitems.size());
  unsigned int nbOut = static_cast<unsigned int>(oitems.size());

  if(!nodePartiallyConnected() || (nodePartiallyConnected() && 
				   (nbIn==_customNode->nbInputs() && nbOut==_customNode->nbOutputs()))) {
    // set input/output names 
    QStringList inames;
    QStringList onames;
    for(unsigned int i=0;i<nbIn;++i) {
      inames.push_back(iitems[i]->name());
    }
    for(unsigned int i=0;i<nbOut;++i) {
      onames.push_back(oitems[i]->name());
    }
    _customNode->setIONames(inames,onames);
  }
    
  _useVertex      = settings.useVertex();
  _useFragment    = settings.useFragment();
  _useTesselation = settings.useTesselation();
  _useGeometry    = settings.useGeometry();
  _glslVersion    = settings.glslVersion();
  _tmpName        = settings.name();
  _outFilter      = settings.outFilter();
  _outWrap        = settings.outWrap();
  _useDepthTest   = settings.useDepthTest();
  _useBlending    = settings.useBlending();
  _depthFunc      = settings.depthFunc();
  _blendSrc       = settings.blendSrc();
  _blendDst       = settings.blendDst();
  _background     = settings.background();

  // set outputsize 
  const Vector2f ts = tsize();
  const Vector2f to = toffset();
  if(ts!=settings.tsize() || to!=settings.toffset()) {
    _customNode->setImgSize(settings.tsize(),settings.toffset());    
  }

  initWidgets(settings);
  initLayout();
}


void GenericCustomWidget::initWidgets(const GenericCustomWidgetSetting &settings) {
  // clean everything 
  cleanLayout();
  cleanWidgets();

  // mouse widget 
  if(settings.useMouse()) {
    _mouseWidget = new Vector2iSpinWidget(_customNode,"mouse",Vector2i(-10000,-10000),
					  Vector2i(10000,10000),Vector2i(0,0),true);
    addChildWidget(_mouseWidget);
  }

  // mouse widget 
  if(settings.useKeyboard()) {
    _keyboardWidget = new IntSpinWidget(_customNode,"keyboard",0,255,0,true);
    addChildWidget(_keyboardWidget);
  }

  // user-defined widgets 
  std::vector<GenericCustomParamItem *> params = settings.paramItems();
  for(unsigned int i=0;i<params.size();++i) {
    // get all params 
    const int type     = params[i]->type();
    const int view     = params[i]->view();
    const bool kf      = params[i]->keyframed();
    const QString name = params[i]->name();
    const float minVal = params[i]->minVal();
    const float maxVal = params[i]->maxVal();
    const float val    = params[i]->val();
    
    switch(type) {
    case WidgetFactory::FloatID: // float
      _floatWidgets.push_back(WidgetFactory::createFloatAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_floatWidgets[_floatWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::FloatID,(int)(_floatWidgets.size()-1)));
      break;
    case WidgetFactory::IntID: // int
      _intWidgets.push_back(WidgetFactory::createIntAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_intWidgets[_intWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::IntID,(int)(_intWidgets.size()-1)));
      break;
    case WidgetFactory::Vector2fID: // vector2f
      _vector2fWidgets.push_back(WidgetFactory::createVector2fAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector2fWidgets[_vector2fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector2fID,(int)(_vector2fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector2iID: // vector2i
      _vector2iWidgets.push_back(WidgetFactory::createVector2iAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector2iWidgets[_vector2iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector2iID,(int)(_vector2iWidgets.size()-1)));
      break;
    case WidgetFactory::Vector3fID: // vector3f
      _vector3fWidgets.push_back(WidgetFactory::createVector3fAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector3fWidgets[_vector3fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector3fID,(int)(_vector3fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector3iID: // vector3i
      _vector3iWidgets.push_back(WidgetFactory::createVector3iAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector3iWidgets[_vector3iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector3iID,(int)(_vector3iWidgets.size()-1)));
      break;
    case WidgetFactory::Vector4fID: // vector4f
      _vector4fWidgets.push_back(WidgetFactory::createVector4fAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector4fWidgets[_vector4fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector4fID,(int)(_vector4fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector4iID: // vector4i
      _vector4iWidgets.push_back(WidgetFactory::createVector4iAttribWidget(_customNode,name,view,minVal,maxVal,val,kf));
      addChildWidget(_vector4iWidgets[_vector4iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector4iID,(int)(_vector4iWidgets.size()-1)));
      break;


    default:break;
    }
  }
}

bool GenericCustomWidget::saveWidgets(SceneSaver  *saver) {
  // we only have to save/load widget names
  // they will be automatically fully loaded by the nodeWidget class
  bool ret = true;
  FloatAttribWidget    *wf;
  IntAttribWidget      *wi;
  Vector2fAttribWidget *w2f;
  Vector2iAttribWidget *w2i;
  Vector3fAttribWidget *w3f;
  Vector3iAttribWidget *w3i;
  Vector4fAttribWidget *w4f;
  Vector4iAttribWidget *w4i;

  // mouse 
  if(!saver->saveBool("genericNeedMouse",useMouse()))         ret = false;
  if(!saver->saveBool("genericNeedKeyboard",useKeyboard()))   ret = false;
  if(!saver->saveUint("genericNbWidgets",static_cast<unsigned int>(_mapWidgets.size()))) ret = false;
  
  for(unsigned int i=0;i<_mapWidgets.size();++i) {
    switch(_mapWidgets[i].first) {
    case WidgetFactory::FloatID:
      wf = _floatWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::FloatID))    ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),wf->type()))                ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),wf->name()))             ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),wf->keyframed()))     ret = false;
      break;
    case WidgetFactory::IntID:
      wi = _intWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::IntID))      ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),wi->type()))                ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),wi->name()))             ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),wi->keyframed()))     ret = false;
      break;
    case WidgetFactory::Vector2fID:
      w2f = _vector2fWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector2fID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w2f->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w2f->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w2f->keyframed()))    ret = false;      
      break;
    case WidgetFactory::Vector2iID:
      w2i = _vector2iWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector2iID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w2i->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w2i->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w2i->keyframed()))    ret = false;          
      break;
    case WidgetFactory::Vector3fID:
      w3f = _vector3fWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector3fID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w3f->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w3f->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w3f->keyframed()))    ret = false;
      break;
    case WidgetFactory::Vector3iID:
      w3i = _vector3iWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector3iID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w3i->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w3i->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w3i->keyframed()))    ret = false;    
      break;
    case WidgetFactory::Vector4fID:
      w4f = _vector4fWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector4fID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w4f->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w4f->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w4f->keyframed()))    ret = false;
      break;
    case WidgetFactory::Vector4iID:
      w4i = _vector4iWidgets[_mapWidgets[i].second];
      if(!saver->saveInt(Tags::attribId("genericWidgetType",i),WidgetFactory::Vector4iID)) ret = false;
      if(!saver->saveInt(Tags::attribId("genericWidgetView",i),w4i->type()))               ret = false;
      if(!saver->saveString(Tags::attribId("genericWidgetName",i),w4i->name()))            ret = false;
      if(!saver->saveBool(Tags::attribId("genericWidgetKeyframed",i),w4i->keyframed()))    ret = false;
      break;
    default:break;
    }
  }




  // // float sliders
  // if(!saver->saveUint("genericNbFloatWidgets",_floatWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_floatWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericFloatWidgetType",i),_floatWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericFloatWidgetName",i),_floatWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericFloatWidgetKeyframed",i),_floatWidgets[i]->keyframed())) ret = false;
  // }

  // // int sliders
  // if(!saver->saveUint("genericNbIntWidgets",_intWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_intWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericIntWidgetType",i),_intWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericIntWidgetName",i),_intWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericIntWidgetKeyframed",i),_intWidgets[i]->keyframed())) ret = false;
  // }

  // // vector2f sliders
  // if(!saver->saveUint("genericNbVector2fWidgets",_vector2fWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector2fWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector2fWidgetType",i),_vector2fWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector2fWidgetName",i),_vector2fWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector2fWidgetKeyframed",i),_vector2fWidgets[i]->keyframed())) ret = false;
  // }

  // // vector2i sliders
  // if(!saver->saveUint("genericNbVector2iWidgets",_vector2iWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector2iWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector2iWidgetType",i),_vector2iWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector2iWidgetName",i),_vector2iWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector2iWidgetKeyframed",i),_vector2iWidgets[i]->keyframed())) ret = false;
  // }


  // // vector3f sliders
  // if(!saver->saveUint("genericNbVector3fWidgets",_vector3fWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector3fWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector3fWidgetType",i),_vector3fWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector3fWidgetName",i),_vector3fWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector3fWidgetKeyframed",i),_vector3fWidgets[i]->keyframed())) ret = false;
  // }

  // // vector3i sliders
  // if(!saver->saveUint("genericNbVector3iWidgets",_vector3iWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector3iWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector3iWidgetType",i),_vector3iWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector3iWidgetName",i),_vector3iWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector3iWidgetKeyframed",i),_vector3iWidgets[i]->keyframed())) ret = false;
  // }

  // // vector4f sliders
  // if(!saver->saveUint("genericNbVector4fWidgets",_vector4fWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector4fWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector4fWidgetType",i),_vector4fWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector4fWidgetName",i),_vector4fWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector4fWidgetKeyframed",i),_vector4fWidgets[i]->keyframed())) ret = false;
  // }

  // // vector4i sliders
  // if(!saver->saveUint("genericNbVector4iWidgets",_vector4iWidgets.size())) ret = false;
  // for(unsigned int i=0;i<_vector4iWidgets.size();++i) {
  //   if(!saver->saveInt(Tags::attribId("genericVector4iWidgetType",i),_vector4iWidgets[i]->type()))            ret = false;
  //   if(!saver->saveString(Tags::attribId("genericVector4iWidgetName",i),_vector4iWidgets[i]->name()))         ret = false;
  //   if(!saver->saveBool(Tags::attribId("genericVector4iWidgetKeyframed",i),_vector4iWidgets[i]->keyframed())) ret = false;
  // }



  return ret;
}

bool GenericCustomWidget::loadWidgets(SceneLoader *loader) {
  // we only have to save/load widget names
  // they will be automatically fully loaded by the nodeWidget class
  bool ret = true;

  cleanLayout();
  cleanWidgets();

  bool usemouse = false;
  bool usekeyboard = false;
  unsigned int nb = 0;

  // mouse 
  if(!loader->loadBool("genericNeedMouse",usemouse)) ret = false;
  if(usemouse) {
    _mouseWidget = new Vector2iSpinWidget(_customNode,"mouse",Vector2i(-10000,-10000),
					  Vector2i(10000,10000),Vector2i(0,0),true);
    addChildWidget(_mouseWidget);
  }

  if(!loader->loadBool("genericNeedKeyboard",usekeyboard)) ret = false;
  if(usekeyboard) {
    _keyboardWidget = new IntSpinWidget(_customNode,"keyboard",0,255,0,true);
    addChildWidget(_keyboardWidget);
  }

  if(!loader->loadUint("genericNbWidgets",nb)) ret = false;
  
  for(unsigned int i=0;i<nb;++i) {
    int type = 0;
    int view = 0;
    bool kf  = true;
    QString name = "widget"+QString::number(i);

    if(!loader->loadInt(Tags::attribId("genericWidgetType",i),type))     ret = false;
    if(!loader->loadInt(Tags::attribId("genericWidgetView",i),view))     ret = false;
    if(!loader->loadString(Tags::attribId("genericWidgetName",i),name))  ret = false;
    if(!loader->loadBool(Tags::attribId("genericWidgetKeyframed",i),kf)) ret = false;

    switch(type) {
    case WidgetFactory::FloatID:
      _floatWidgets.push_back(WidgetFactory::createFloatAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_floatWidgets[_floatWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::FloatID,(int)(_floatWidgets.size()-1)));
      break;
    case WidgetFactory::IntID:
      _intWidgets.push_back(WidgetFactory::createIntAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_intWidgets[_intWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::IntID,(int)(_intWidgets.size()-1)));
      break;
    case WidgetFactory::Vector2fID:
      _vector2fWidgets.push_back(WidgetFactory::createVector2fAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector2fWidgets[_vector2fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector2fID,(int)(_vector2fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector2iID:
      _vector2iWidgets.push_back(WidgetFactory::createVector2iAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector2iWidgets[_vector2iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector2iID,(int)(_vector2iWidgets.size()-1)));
      break;
    case WidgetFactory::Vector3fID:
      _vector3fWidgets.push_back(WidgetFactory::createVector3fAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector3fWidgets[_vector3fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector3fID,(int)(_vector3fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector3iID:
      _vector3iWidgets.push_back(WidgetFactory::createVector3iAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector3iWidgets[_vector3iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector3iID,(int)(_vector3iWidgets.size()-1)));
      break;
    case WidgetFactory::Vector4fID:
      _vector4fWidgets.push_back(WidgetFactory::createVector4fAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector4fWidgets[_vector4fWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector4fID,(int)(_vector4fWidgets.size()-1)));
      break;
    case WidgetFactory::Vector4iID:
      _vector4iWidgets.push_back(WidgetFactory::createVector4iAttribWidget(_customNode,name,view-1,0.0f,1.0f,0.0f,kf));
      addChildWidget(_vector4iWidgets[_vector4iWidgets.size()-1]);
      _mapWidgets.push_back(pair<int,int>(WidgetFactory::Vector4iID,(int)(_vector4iWidgets.size()-1)));
      break;
    default:break;
    }
  }







  // // float
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbFloatWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "floatWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericFloatWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericFloatWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericFloatWidgetKeyframed",i),kf);
  //   _floatWidgets.push_back(WidgetFactory::createFloatAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_floatWidgets[i]);
  // }

  // // int
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbIntWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "intWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericIntWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericIntWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericIntWidgetKeyframed",i),kf);
  //   _intWidgets.push_back(WidgetFactory::createIntAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_intWidgets[i]);
  // }

  // // vector2f
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector2fWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector2fWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector2fWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector2fWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector2fWidgetKeyframed",i),kf);
  //   _vector2fWidgets.push_back(WidgetFactory::createVector2fAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector2fWidgets[i]);
  // }

  // // vector2i
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector2iWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector2iWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector2iWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector2iWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector2iWidgetKeyframed",i),kf);
  //   _vector2iWidgets.push_back(WidgetFactory::createVector2iAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector2iWidgets[i]);
  // }

  // // vector3f
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector3fWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector3fWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector3fWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector3fWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector3fWidgetKeyframed",i),kf);
  //   _vector3fWidgets.push_back(WidgetFactory::createVector3fAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector3fWidgets[i]);
  // }

  // // vector3i
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector3iWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector3iWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector3iWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector3iWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector3iWidgetKeyframed",i),kf);
  //   _vector3iWidgets.push_back(WidgetFactory::createVector3iAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector3iWidgets[i]);
  // }

  // // vector4f
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector4fWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector4fWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector4fWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector4fWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector4fWidgetKeyframed",i),kf);
  //   _vector4fWidgets.push_back(WidgetFactory::createVector4fAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector4fWidgets[i]);
  // }

  // // vector4i
  // nb = 0;
  // ret = ret && loader->loadUint("genericNbVector4iWidgets",nb);
  // for(unsigned int i=0;i<nb;++i) {
  //   bool kf = true;
  //   int type = 0;
  //   QString name = "vector4iWidget"+QString::number(i);
  //   ret = ret && loader->loadInt(Tags::attribId("genericVector4iWidgetType",i),type);
  //   ret = ret && loader->loadString(Tags::attribId("genericVector4iWidgetName",i),name);
  //   ret = ret && loader->loadBool(Tags::attribId("genericVector4iWidgetKeyframed",i),kf);
  //   _vector4iWidgets.push_back(WidgetFactory::createVector4iAttribWidget(_customNode,name,type-1,0.0f,1.0f,0.0f,kf));
  //   addChildWidget(_vector4iWidgets[i]);
  // }

  return ret;
}

bool GenericCustomWidget::loadUserDefinedWidgets(SceneLoader *) {
  initLayout();
  return true;
}


void GenericCustomWidget::setVertSource(const QString &head,const QString &body) {
  _vertHead->setText(head);
  _vertSource->setLineShift(_vertHead->text().split(QRegExp("\n|\r")).size());
  _vertSource->setPlainText(body);
}

void GenericCustomWidget::setTessCSource(const QString &head,const QString &body) {
  _tessCHead->setText(head);
  _tessCSource->setLineShift(_tessCHead->text().split(QRegExp("\n|\r")).size());
  _tessCSource->setPlainText(body);
}

void GenericCustomWidget::setTessESource(const QString &head,const QString &body) {
  _tessEHead->setText(head);
  _tessESource->setLineShift(_tessEHead->text().split(QRegExp("\n|\r")).size());
  _tessESource->setPlainText(body);
}

void GenericCustomWidget::setGeomSource(const QString &head,const QString &body) {
  _geomHead->setText(head);
  _geomSource->setLineShift(_geomHead->text().split(QRegExp("\n|\r")).size());
  _geomSource->setPlainText(body);
}

void GenericCustomWidget::setFragSource(const QString &head,const QString &body) {
  _fragHead->setText(head);
  _fragSource->setLineShift(_fragHead->text().split(QRegExp("\n|\r")).size());
  _fragSource->setPlainText(body);
}
