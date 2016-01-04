// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ANIMATION_WIDGET_H
#define ANIMATION_WIDGET_H

#include <QDockWidget>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QDialog>
#include <QTimer>

#include "core/sceneIOTags.h"
#include "core/curve.h"
#include "core/animation.h"
#include "view/timelineWidget.h"
#include "view/nodeWidget.h"

class GraphWidget;

class AnimationSettings : public QDialog  {
  Q_OBJECT
    
    public:
  AnimationSettings(GraphWidget *graphWidget);

  void update();

  public slots:
  void confirmClicked();
  void cancelClicked();

 private:
  static const int MAX_RANGE = 99999;

  GraphWidget *_graphWidget;
  Animation   *_animation;

  QGroupBox   *_animBox;
  QLabel      *_startFrameLabel;
  QLabel      *_endFrameLabel;
  QLabel      *_fpsLabel;
  QSpinBox    *_startFrameSpin;
  QSpinBox    *_endFrameSpin;
  QSpinBox    *_fpsSpin;

  QDialogButtonBox *_buttons;
};


class LIBRARY_EXPORT AnimationWidget : public QDockWidget, public IOData {
  Q_OBJECT

public:
  ~AnimationWidget();

  // this widget is a singleton
  static AnimationWidget *instance(GraphWidget *graphWidget=NULL,QWidget *parent=NULL) {
    if(!_instance) {
      _instance = new AnimationWidget(graphWidget,parent);
    }
    return _instance;
  } 

  inline unsigned int currentFrame() const {return _animation->currentFrame();}

  void addParam(NodeWidget *widget,Curve *curve,const QStringList varName,float minVal=-10000,float maxVal=10000);
  void delParams(NodeWidget *widget);
  void updateIds();
  void play();
  void stop();

  void controlPointsChanged(NodeWidget *widget,const QStringList varName);

  inline void animationSettingsChanged();
  inline void animationFrameChanged();
  inline void hideCurve(const QStringList &varName) {_timelineWidget->hideCurve(varName);}
  inline void showCurve(const QStringList &varName) {_timelineWidget->showCurve(varName);}
  inline void cleanCurve(const QStringList &varName) {_timelineWidget->cleanCurve(varName);}

  // load and save 
  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

 protected:
  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);

  public slots:
  void currentFrameChanged(int v);
  void firstFrameClicked();
  void lastFrameClicked();
  void playClicked();
  void frameChanged();

private:
  AnimationWidget(GraphWidget *graphWidget,QWidget *parent=0);
  void deleteItem(QTreeWidgetItem *item);
  void deleteTimelineCurves(QTreeWidgetItem *item);
  void updateLeafIdsRec(QTreeWidgetItem *tli,QTreeWidgetItem *item);
  bool isTopLevelItem(QTreeWidgetItem *item);

  static AnimationWidget *_instance;
  
  GraphWidget    *_graphWidget;
  TimelineWidget *_timelineWidget;
  Animation      *_animation;
  QTimer         *_timer;

  bool         _playing;
  QGroupBox   *_animBox;
  QSpinBox    *_currentFrameSpin;
  QPushButton *_playButton;
  QPushButton *_firstFrameButton;
  QPushButton *_lastFrameButton;
  QTreeWidget *_varList;
};

class ItemWidget : public QWidget {
  Q_OBJECT
    public:
  ItemWidget(NodeWidget *widget,const QStringList &varName,Curve *curve);

  public slots:
  void interpolationChanged(int);
  void leftBehaviorChanged(int);
  void rightBehaviorChanged(int);
  void hideClicked();
  void cleanClicked();

  inline const QStringList &varName() const {return _varName;}
  inline void setVarName(const QStringList varName) {_varName = varName;}
  inline const QString nodeNameId() const {return _nodeWidget->nodeNamesId()[0];}

 private:
  NodeWidget *_nodeWidget;
  QStringList _varName;
  Curve      *_curve;
  bool        _hidden;

  QComboBox   *_interpolationTypes;
  QComboBox   *_leftBehaviorTypes;
  QComboBox   *_rightBehaviorTypes;
  QPushButton *_hideButton;
  QPushButton *_cleanButton;
};

inline void AnimationWidget::animationSettingsChanged() {
  _currentFrameSpin->setRange(_animation->firstFrame(),_animation->lastFrame());
  _timer->setInterval((int)(1000.0f/(float)_animation->fps()));
  _timelineWidget->timelineRectChanged();
}

inline void AnimationWidget::animationFrameChanged() {
  _currentFrameSpin->setValue(_animation->currentFrame());
}

#endif // ANIMATION_WIDGET_H
