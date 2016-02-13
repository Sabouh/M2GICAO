// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TIMELINE_WIDGET_H
#define TIMELINE_WIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

#include "misc/extinclude.h"
#include "core/animation.h"
#include "core/sceneIOTags.h"

using namespace std;

class Curve;
class GraphWidget;
class AnimationWidget;
class ControlPoint;
class TangentPoint;
class TangentLine;
class CurveDrawer;
class BoundaryDrawer;
class PositionDrawer;
class Frame;
class NodeWidget;

class LIBRARY_EXPORT TimelineWidget : public QGraphicsView, public IOData {
  Q_OBJECT

    public:

  struct Param {
    Curve      *curve;
    NodeWidget *nodeWidget;
    QColor      varColor;
    QStringList varName;
    float       minVal;
    float       maxVal;
    bool        selected;
  };

  TimelineWidget(GraphWidget *graphWidget,AnimationWidget *animWidget,QWidget *parent=0);
  ~TimelineWidget();
  
  // update curve ids 
  void updateId(const QStringList &oldId,const QStringList &newId);

  // rectangle containing the scene
  void timelineRectChanged();
  void currentFrameChanged();
  void curveChanged(NodeWidget *widget);

  // adding/deleting curves
  void addCurve(NodeWidget *widget,const QStringList &varName,Curve *curve,const QColor c,
		float minVal=-10000,float maxVal=10000);
  void delCurve(const QStringList &varName);

  // adding/deleting control points
  void addControlPoint(const QPointF &p);
  void delControlPoint(ControlPoint *p);
  void controlPointsChanged(const QStringList &varName);

  // useful functions 
  void select(const QStringList &varName);
  void hideCurve(const QStringList &varName);
  void showCurve(const QStringList &varName);
  void cleanCurve(const QStringList &varName);
  void clean();

  inline BoundaryDrawer *boundaries() {return _boundaries;}
  inline PositionDrawer *position  () {return _position;}
  inline const QPointF &viewportStep() const {return _viewportStep;}
  inline const QPointF &sceneStep()    const {return _sceneStep;}
  inline const QRectF timelineRect()   const {return _timelineRect;}
  inline void refresh();

  // load and save 
  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

 protected:
  void drawBackground   (QPainter *painter, const QRectF &rect);
  void mousePressEvent  (QMouseEvent  *event);
  void mouseMoveEvent   (QMouseEvent  *event);
  void mouseReleaseEvent(QMouseEvent  *event);
  void keyPressEvent    (QKeyEvent    *event);
  void keyReleaseEvent  (QKeyEvent    *event);
  void wheelEvent       (QWheelEvent  *event);
  void resizeEvent      (QResizeEvent *event);

 private:
  enum {NO_MOTION,OFFSET_MOTION,SCALING_MOTION};

  // approximative number of pixels between 2 lines of the grid
  static const int _approxViewportStep = 60;

  GraphWidget     *_graphWidget;
  Animation       *_animation;
  AnimationWidget *_animWidget;
  QGraphicsScene  *_scene;
  
  QPointF _sceneStep;    // step between 2 lines in scene coords
  QPointF _viewportStep; // step between 2 lines in viewport coords
  QRectF  _timelineRect;

  QPointF _mouseCoord;   // current mouse position
  QPointF _mouseTmp;     // tmp mouse position
  int     _interaction;  // interaction type 
  bool    _ctrl;
  bool    _shift;
  bool    _needToDeleteControlPoint;
  bool    _needToAddControlPoint;

  // items for frame and curve limits 
  Frame          *_frame;
  BoundaryDrawer *_boundaries;
  PositionDrawer *_position;

  // items for curves and control points 
  std::vector<Param *>                      _params;
  std::vector<std::vector<ControlPoint *> > _points;
  std::vector<std::vector<TangentPoint *> > _tangents;
  std::vector<std::vector<TangentLine *> >  _tlines;
  std::vector<CurveDrawer *>                _curves;

  void resetTimelineRect();
  void remapControlPointsInTimelineRect(const QRectF &r1,const QRectF &r2);
  void resetSceneRect(bool transform=true);
  void setCurrentFrame(const QPointF &pt);
  void updateScaleDependentItems();

  inline unsigned int nbCurves() const {return static_cast<unsigned int>(_params.size());}
  inline void scaleView(qreal scaleFactor);
  inline void scaleView(qreal scalex,qreal scaley);
  inline void zoomIn();
  inline void zoomOut();
  inline void computeSteps(int approxStep=_approxViewportStep);
  inline double rangeSceneCoord(const double dist) const;
  inline const QPointF closestPoint(const QPointF &pt,const QPointF &step);
};

inline void TimelineWidget::refresh() {
  updateSceneRect(_scene->sceneRect());
  scene()->update(QRectF(mapToScene(QPoint(0,height())),mapToScene(width(),0)));
}
 
inline void TimelineWidget::scaleView(qreal scaleFactor) {
  scaleView(scaleFactor,scaleFactor);
}

inline void TimelineWidget::scaleView(qreal scalex,qreal scaley) {
  const float lp = 1e+6;
  const float lm = (float) (1e-4);

  if(_sceneStep.x()<lm && scalex>1.0) {
    scalex = 1.0;
  } else if(_sceneStep.x()>lp && scalex<1.0) {
    scalex = 1.0;
  }

  if(_sceneStep.y()<lm && scaley>1.0) {
    scaley = 1.0;
  } else if(_sceneStep.y()>lp && scaley<1.0) {
    scaley = 1.0;
  }

  scale(scalex,scaley);
  computeSteps();
  updateScaleDependentItems();
}

inline void TimelineWidget::zoomIn() {
  scaleView(qreal(1.2));
}

inline void TimelineWidget::zoomOut() {
  scaleView(1/qreal(1.2));
}

inline void TimelineWidget::computeSteps(int approxStep) {
  const QPointF d = mapToScene(QPoint(approxStep,approxStep))-mapToScene(QPoint(0,0));

  _sceneStep    = QPointF(rangeSceneCoord(fabs(d.x())),rangeSceneCoord(fabs(d.y())));
  _viewportStep = mapFromScene(_sceneStep)-mapFromScene(QPointF(0,0));
  _viewportStep.setX(fabs(_viewportStep.x()));
  _viewportStep.setY(fabs(_viewportStep.y()));
}

inline double TimelineWidget::rangeSceneCoord(const double dist) const {
  // dist is a distance information in scene coordinates 
  // this function computes a new distance, close to dist, but
  // aligned with 1*10^x, 2*10^x or 5*10^x (x is an integer)

  const double l = floor(log10(dist));
  const double v = dist*pow(10.0,-l);
  
  double o = 0.0;
  double s;
  if(v<1.5) {
    s = 1.0;
  } else if(v<3.5) {
    s = 2.0;
  } else if(v<7.5) {
    s = 5.0;
  } else {
    s = 1.0;
    o = 1.0;    
  }

  return s*pow(10.0,l+o);
}

inline const QPointF TimelineWidget::closestPoint(const QPointF &pt,const QPointF &step) {
  // return the closest point of pt that intersects an horizontal and vertical line
  const double vx = fmod(pt.x(),step.x());
  const double vy = fmod(pt.y(),step.y());
  const double x = vx<step.x()/2.0 ? pt.x()-vx : pt.x()+step.x()-vx;
  const double y = vy<step.y()/2.0 ? pt.y()-vy : pt.y()+step.y()-vy;
  return QPointF(x,y);
}

#endif // TIMELINE_WIDGET_H
