// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/animationWidget.h"
#include "view/graphWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSplitter>

using namespace std;

AnimationSettings::AnimationSettings(GraphWidget *graphWidget) 
  : QDialog(),
    _graphWidget(graphWidget),
    _animation(_graphWidget->animation()) {

  _animBox = new QGroupBox("Animation");

  _startFrameLabel  = new QLabel(tr("Start frame"));
  _endFrameLabel    = new QLabel(tr("End frame"));
  _fpsLabel         = new QLabel(tr("Framerate"));

  _startFrameSpin   = new QSpinBox();
  _endFrameSpin     = new QSpinBox();
  _fpsSpin          = new QSpinBox();
  
  _buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  _startFrameSpin->setRange(0,MAX_RANGE);
  _endFrameSpin->setRange(0,MAX_RANGE);
  _fpsSpin->setRange(1,MAX_RANGE);

  update();

  QVBoxLayout *vbox = new QVBoxLayout();
  QHBoxLayout *hbox;

  hbox = new QHBoxLayout();
  hbox->addWidget(_startFrameLabel);
  hbox->addWidget(_startFrameSpin);
  vbox->addItem(hbox);
  
  hbox = new QHBoxLayout();
  hbox->addWidget(_endFrameLabel);
  hbox->addWidget(_endFrameSpin);
  vbox->addItem(hbox);

  hbox = new QHBoxLayout();
  hbox->addWidget(_fpsLabel);
  hbox->addWidget(_fpsSpin);
  vbox->addItem(hbox);

  vbox->addWidget(_buttons);

  _animBox->setLayout(vbox);
  vbox = new QVBoxLayout();
  vbox->addWidget(_animBox);
  setLayout(vbox);

  setWindowModality(Qt::WindowModal);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));
}

void AnimationSettings::update() {
  _startFrameSpin->setValue(_animation->firstFrame());
  _endFrameSpin->setValue(_animation->lastFrame());
  _fpsSpin->setValue(_animation->fps());
}

void AnimationSettings::confirmClicked() {
  unsigned int first = (unsigned int)_startFrameSpin->value();
  unsigned int last  = (unsigned int)_endFrameSpin->value();
  unsigned int fps   = max((unsigned int)_fpsSpin->value(),(unsigned int)1);
  unsigned int curr  = _animation->currentFrame();

  if(last>first) {
    _animation->setFirstFrame(first);
    _animation->setLastFrame(last);
    _animation->setFps(fps);
    
    if(curr<_animation->firstFrame()) {
      _animation->goToFirst();
    } else if(curr>_animation->lastFrame()) {
      _animation->goToLast();
    }

    _graphWidget->animationSettingsChanged();
  }

  close();
}

void AnimationSettings::cancelClicked() {
  close();
}


AnimationWidget *AnimationWidget::_instance = NULL;

AnimationWidget::AnimationWidget(GraphWidget *graphWidget,QWidget *parent)
  : QDockWidget("Animation",parent),
    _graphWidget(graphWidget),
    _animation(_graphWidget->animation()),
    _timer(new QTimer(this)),
    _playing(false) {

  QScrollArea *scrollbar = new QScrollArea();
  scrollbar->setWidgetResizable(true);
  scrollbar->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollbar->setAlignment(Qt::AlignRight);

  _timelineWidget   = new TimelineWidget(graphWidget,this);  
  _animBox          = new QGroupBox();
  _currentFrameSpin = new QSpinBox();
  _playButton       = new QPushButton(tr("Play"));
  _firstFrameButton = new QPushButton(tr("Left"));
  _lastFrameButton  = new QPushButton(tr("Right"));

  _playButton->setIcon(QIcon(":/play"));
  _firstFrameButton->setIcon(QIcon(":/left-left"));
  _lastFrameButton->setIcon(QIcon(":/right-right"));

  _varList          = new QTreeWidget();
  _varList->setColumnCount(2);
  _varList->setHeaderLabels(QStringList() << "Parameters" << "");
  _varList->setSizePolicy(QSizePolicy::Ignored, 
			  QSizePolicy::Expanding);
  _varList->setColumnWidth(0,200);
  _varList->setColumnWidth(1,500);
  _varList->setMinimumWidth(width()/4);

  QVBoxLayout *vbox;
  QHBoxLayout *hbox;

  QWidget *animBoxWidget  = new QWidget();
  QSplitter *split = new QSplitter();

  hbox = new QHBoxLayout();
  hbox->addWidget(_firstFrameButton);
  hbox->addWidget(_playButton);
  hbox->addWidget(_lastFrameButton);
  hbox->addWidget(_currentFrameSpin);
  _animBox->setLayout(hbox);

  vbox = new QVBoxLayout();
  vbox->addWidget(_varList);
  vbox->addWidget(_animBox);

  animBoxWidget->setLayout(vbox);
  
  split->addWidget(animBoxWidget);
  split->addWidget(_timelineWidget);

  scrollbar->setWidget(split);
  setWidget(scrollbar);

  animationSettingsChanged();
  animationFrameChanged();

  connect(_timer,SIGNAL(timeout()),this,SLOT(frameChanged()));
  connect(_currentFrameSpin,SIGNAL(valueChanged(int)),this,SLOT(currentFrameChanged(int)));
  connect(_firstFrameButton,SIGNAL(clicked()),this,SLOT(firstFrameClicked()));
  connect(_lastFrameButton,SIGNAL(clicked()),this,SLOT(lastFrameClicked()));
  connect(_playButton,SIGNAL(clicked()),this,SLOT(playClicked()));
}

void AnimationWidget::resizeEvent(QResizeEvent *) {
  _varList->setMinimumWidth(width()/4);
}

AnimationWidget::~AnimationWidget() {
  delete _timelineWidget;
  delete _timer;
}

void AnimationWidget::currentFrameChanged(int v) {
  _graphWidget->animationFrameChanged();
  _animation->setCurrentFrame(v);
  _timelineWidget->currentFrameChanged();
}

void AnimationWidget::addParam(NodeWidget *widget,Curve *curve,const QStringList varName,float minVal,float maxVal) {
  QTreeWidgetItem *topLevelItem = NULL;
  QTreeWidgetItem *parentItem   = NULL;
  const QStringList nodeNames = widget->nodeNamesId();
  const QStringList fullName = QStringList() << nodeNames << varName;
  
  if(fullName.isEmpty()) return;
  const QString nodeName = fullName[0];

  QList<QTreeWidgetItem *> items = _varList->findItems(nodeName,Qt::MatchFixedString);

  // check the top level item
  if(items.isEmpty()) {
    const QColor col = Qt::black;
    const QBrush brush(col);
    QFont font("",9);
    if(fullName.size()==1) font.setBold(true);

    topLevelItem = new QTreeWidgetItem();
    topLevelItem->setForeground(0,brush);
    topLevelItem->setFont(0,font);
    topLevelItem->setText(0,nodeName);
    _varList->addTopLevelItem(topLevelItem);
  } else {
    topLevelItem = items[0];
  }

  // search for the variable names in the tree
  parentItem = topLevelItem;

  int index;
  for(index=1;index<fullName.size();++index) {
    bool found = false;
    
    for(int i=0;i<parentItem->childCount();++i) {
      if(fullName[index]==parentItem->child(i)->text(0)) {
	parentItem = parentItem->child(i);
	found = true;
	break;
      } 
    }
    
    if(!found) break;
  }


  const QColor c((rand()%256),(rand()%256),(rand()%256));

  for(int i=index;i<fullName.size();++i) {
    const QColor col = i==fullName.size()-1 ? c : Qt::black;
    const QBrush brush(col);
    QFont font("",9,QFont::Bold);
    if(i<nodeNames.size()) font.setBold(false);

    parentItem = new QTreeWidgetItem(parentItem);
    parentItem->setForeground(0,brush);
    parentItem->setFont(0,font);
    parentItem->setText(0,fullName[i]);
    if(i==fullName.size()-1) {
      _varList->setItemWidget(parentItem,1,new ItemWidget(widget,fullName,curve));
    }
  }

  if(index<fullName.size()) {
    _timelineWidget->addCurve(widget,fullName,curve,c,minVal,maxVal);
  }
}

void AnimationWidget::delParams(NodeWidget *widget) {
  if(!widget) return;

  const QString nodeName = widget->nodeNameId();
  for(int i=0;i<_varList->topLevelItemCount();++i) {
    QTreeWidgetItem *item = _varList->topLevelItem(i);

    if(item->text(0)==nodeName) {
      deleteTimelineCurves(item);
      deleteItem(item);
      break;
    }
  }
}

void AnimationWidget::updateIds() {
  for(int i=0;i<_varList->topLevelItemCount();++i) {
    QTreeWidgetItem *item = _varList->topLevelItem(i);
    updateLeafIdsRec(item,item);
  }
}


void AnimationWidget::updateLeafIdsRec(QTreeWidgetItem *tli,QTreeWidgetItem *item) {
  if(item->childCount()==0) {
    // update here
    ItemWidget *w = dynamic_cast<ItemWidget *>(_varList->itemWidget(item,1));
    const QString name = w->nodeNameId();
    QStringList oldId = w->varName();
    QStringList newId = oldId;
    newId[0] = name;

    _timelineWidget->updateId(oldId,newId);

    w->setVarName(newId);
    tli->setText(0,name);
  }

  for(int i=0;i<item->childCount();++i) {
    updateLeafIdsRec(tli,item->child(i));
  }
}

void AnimationWidget::controlPointsChanged(NodeWidget *widget,const QStringList varName) {
  const QStringList nameId = QStringList() << widget->nodeNameId() << varName;
  _timelineWidget->controlPointsChanged(nameId);
}

void AnimationWidget::frameChanged() {
  _animation->incrementCurrentFrame();
  animationFrameChanged();
}

void AnimationWidget::firstFrameClicked() {
  _animation->goToFirst();
  animationFrameChanged();
}

void AnimationWidget::lastFrameClicked() {
  _animation->goToLast();
  animationFrameChanged();
}

void AnimationWidget::playClicked() {
  if(_playing) {
    stop();
  } else {
    play();
  }
}

void AnimationWidget::play() {
  _playButton->setText("Stop");
  _playButton->setIcon(QIcon(":/stop"));
  _timer->start();
  _playing = true;
}

void AnimationWidget::stop() {
  _playButton->setText("Play");
  _playButton->setIcon(QIcon(":/play"));
  _timer->stop();
  _playing = false;
}

void AnimationWidget::keyPressEvent(QKeyEvent *event) {
  if(event->key()==Qt::Key_Delete) {
    
    if(QTreeWidgetItem *item = _varList->currentItem()) {
      deleteTimelineCurves(item);
      deleteItem(item);
    }
    
  } else {
    QDockWidget::keyPressEvent(event);
  }
}

bool AnimationWidget::isTopLevelItem(QTreeWidgetItem *item) {
  QTreeWidgetItem *tmp = _varList->topLevelItem(_varList->indexOfTopLevelItem(item));
  return item==tmp;
}

void AnimationWidget::deleteItem(QTreeWidgetItem *item) {
  
  // special case of top level items
  if(isTopLevelItem(item)) {
    QTreeWidgetItem *itemToDelete = _varList->takeTopLevelItem(_varList->indexOfTopLevelItem(item));
    delete itemToDelete;
    return;
  }

  // children
  QTreeWidgetItem *parent = item->parent();
  const int i = parent->indexOfChild(item);
  QTreeWidgetItem *itemToDelete = parent->takeChild(i);
  delete itemToDelete;

  // if the item has no brothers no need to keep its parent
  if(parent->childCount()==0) {
    deleteItem(parent);
  }
}

void AnimationWidget::deleteTimelineCurves(QTreeWidgetItem *item) {
  // delete curve if it is a leaf
  if(item->childCount()==0) {
    ItemWidget *w = dynamic_cast<ItemWidget *>(_varList->itemWidget(item,1));
    _timelineWidget->delCurve(w->varName());
  }

  // delete widgets 
  for(int i=item->columnCount()-1;i>=0;--i) {
    _varList->removeItemWidget(item,i);
  }

  // delete recusively
  for(int i=0;i<item->childCount();++i) {  
    deleteTimelineCurves(item->child(i));
  }
}

bool AnimationWidget::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveVector2i("animationWidgetSize",Vector2i(width(),height()))) ret = false;
  if(!saver->saveIOData("timelineWidget",_timelineWidget)) ret = false;
  return ret;
}

bool AnimationWidget::load(SceneLoader *loader) {
  bool ret = true;
  Vector2i size(width(),height());
  if(!loader->loadVector2i("animationWidgetSize",size)) ret = false;
  if(!loader->loadIOData("timelineWidget",_timelineWidget)) ret = false;
  stop();
  resize(size[0],size[1]);
  return ret;
}

ItemWidget::ItemWidget(NodeWidget *widget,const QStringList &varName,Curve *curve) 
  : _nodeWidget(widget),
    _varName(varName),
    _curve(curve),
    _hidden(false) {

  const int minw = 50;
  
  _interpolationTypes = new QComboBox();
  _interpolationTypes->addItems(Curve::interpNames());
  _interpolationTypes->setCurrentIndex(_curve->interpType());
  _interpolationTypes->setMinimumWidth(minw);

  _leftBehaviorTypes = new QComboBox();
  _leftBehaviorTypes->addItems(Curve::baheviorNames());
  _leftBehaviorTypes->setCurrentIndex(_curve->leftBehaviorType());
  _leftBehaviorTypes->setMinimumWidth(minw);

  _rightBehaviorTypes = new QComboBox();
  _rightBehaviorTypes->addItems(Curve::baheviorNames());
  _rightBehaviorTypes->setCurrentIndex(_curve->rightBehaviorType());
  _rightBehaviorTypes->setMinimumWidth(minw);

  _hideButton = new QPushButton();
  _cleanButton = new QPushButton();

  _hideButton->setIcon(QIcon(":/eye-open"));
  _cleanButton->setIcon(QIcon(":/clean-curve"));

  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget(_leftBehaviorTypes);
  hbox->addWidget(_interpolationTypes);
  hbox->addWidget(_rightBehaviorTypes);
  hbox->addWidget(_hideButton);
  hbox->addWidget(_cleanButton);
  
  setLayout(hbox);

  connect(_interpolationTypes,SIGNAL(currentIndexChanged(int)),this,SLOT(interpolationChanged(int)));
  connect(_leftBehaviorTypes,SIGNAL(currentIndexChanged(int)),this,SLOT(leftBehaviorChanged(int)));
  connect(_rightBehaviorTypes,SIGNAL(currentIndexChanged(int)),this,SLOT(rightBehaviorChanged(int)));
  connect(_hideButton,SIGNAL(clicked()),this,SLOT(hideClicked()));
  connect(_cleanButton,SIGNAL(clicked()),this,SLOT(cleanClicked()));
}

void ItemWidget::interpolationChanged(int index) {
  _curve->setInterpolation(index);
  QStringList l = _varName; l.pop_front();
  AnimationWidget::instance()->controlPointsChanged(_nodeWidget,l);
}

void ItemWidget::leftBehaviorChanged(int index) {
  _curve->setLeftBehavior(index);
}

void ItemWidget::rightBehaviorChanged(int index) {
  _curve->setRightBehavior(index);
}

void ItemWidget::hideClicked() {
  if(_hidden) {
    _hideButton->setIcon(QIcon(":/eye-open"));
    AnimationWidget::instance()->showCurve(_varName);
  } else {
    _hideButton->setIcon(QIcon(":/eye-close"));
    AnimationWidget::instance()->hideCurve(_varName);
  }

  _hidden = !_hidden;
}

void ItemWidget::cleanClicked() {
  AnimationWidget::instance()->cleanCurve(_varName);
}
