// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/timelineItems.h"

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

void CurveDrawer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen p(_param->varColor,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
  p.setCosmetic(true);
  
  QRectF r = scene()->sceneRect();
  QPointF rmin = _timelineWidget->mapToScene(QPoint(0,0));
  QPointF rmax = _timelineWidget->mapToScene(QPoint(_timelineWidget->width(),0));

  float x1 = max(r.left(),rmin.x());
  float x2 = min(r.right(),rmax.x());

  vector<Vector2f> pts = _param->curve->sampleLines(x1,x2,250);
  painter->setPen(p);  
  for(unsigned int i=1;i<pts.size();++i) {
    painter->drawLine(QPointF(pts[i-1][0],pts[i-1][1]),
		      QPointF(pts[i][0],pts[i][1]));
  }
}

QRectF CurveDrawer::boundingRect() const {
  return scene()->sceneRect();
}

void BoundaryDrawer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen p(Qt::black,2);
  p.setCosmetic(true);
  p.setStyle(Qt::DashLine);
  painter->setPen(p);  
  const QRectF r = _timelineWidget->timelineRect();
  painter->drawLine(QPointF(r.left(),_minVal),QPointF(r.right(),_minVal));
  painter->drawLine(QPointF(r.left(),_maxVal),QPointF(r.right(),_maxVal));
}

QRectF BoundaryDrawer::boundingRect() const {
  return scene()->sceneRect();
}

 
void PositionDrawer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  const QString t = QString::number(_x,'f',3)+", "+QString::number(_y,'f',3);
  QFont f;
  f.setBold(true);
  f.setPointSize(13);
  painter->setFont(f);
  painter->drawText(boundingRect(),Qt::AlignRight,t);
}

QRectF PositionDrawer::boundingRect() const {
  return QRectF(0,0,_size,_size/10);
}

ControlPoint::ControlPoint(TimelineWidget *timelineWidget,TimelineWidget::Param *p,unsigned int index,TangentPoint *tl,TangentPoint *tr)
  : _timelineWidget(timelineWidget), 
    _param(p), 
    _index(index),
    _ctrl(false),
    _tangentLeft(tl),
    _tangentRight(tr){
  Vector2f pt = _param->curve->point(_index);
  setPos(pt[0],pt[1]);
  setFlags(ItemIsMovable | ItemIgnoresTransformations | ItemSendsGeometryChanges | ItemIsSelectable);
  setZValue(1);

  _timelineWidget->boundaries()->setLimits(_param->minVal,_param->maxVal);
  _timelineWidget->boundaries()->show();
}

void ControlPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen p(Qt::black,_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  if(_param->selected) {
    p.setWidth(_size+3);
  }

  painter->setPen(p);
  painter->drawPoint(0,0);
  p.setWidth(_size-2);
  p.setColor(_param->varColor);
  painter->setPen(p);
  painter->drawPoint(0,0);
}


QRectF ControlPoint::boundingRect() const {
  qreal s= _size/2;
  return QRectF(-s,-s,_size,_size);
}

void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if(event->button()==Qt::LeftButton) {
    QGraphicsItem::mousePressEvent(event);
    _timelineWidget->select(_param->varName);
    _timelineWidget->boundaries()->setLimits(_param->minVal,_param->maxVal);
    _timelineWidget->boundaries()->show();
    setSelected(true);
  }
  
  if(event->button()==Qt::RightButton){
    QGraphicsItem::mousePressEvent(event);
    if(_tangentLeft && _tangentRight){
      _tangentRight->toggleSymmetric();
      _tangentLeft->toggleSymmetric();
    }
  }
}

void ControlPoint::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {
  _ctrl  = (event->modifiers() & Qt::ControlModifier);
  _shift = (event->modifiers() & Qt::ShiftModifier);
  QGraphicsItem::mouseMoveEvent(event);
  _timelineWidget->curveChanged(_param->nodeWidget);
  //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << " - " << endl;
  //cout << "modifying " << _param->nodeWidget->nodeName().toStdString() << endl;
  _timelineWidget->refresh();
}

void ControlPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
}

QVariant ControlPoint::itemChange(GraphicsItemChange change, const QVariant &value) {

  if(change == QGraphicsItem::ItemPositionChange && scene()) {
    QRectF rect = _timelineWidget->timelineRect();
    QPointF p = value.toPointF();

    if(_shift) {
      const QPointF s = _timelineWidget->sceneStep()/10.0;
      p.setX(closestVal(p.x(),s.x()));
      p.setY(closestVal(p.y(),s.y()));
    } else if(_ctrl) {
      const QPointF s = _timelineWidget->sceneStep()/4.0;
      p.setX(closestVal(p.x(),s.x()));
      p.setY(closestVal(p.y(),s.y()));
    }

    if(_tangentLeft && _index>0) {
        Vector2f pt = _param->curve->point(_index-1);
        rect.setLeft(pt[0] + _param->curve->tangent(_index-1)[0] + 1e-2);
    }
    if(_tangentRight && _index+1 < _param->curve->nbPoints()) {
        Vector2f pt = _param->curve->point(_index+1);
        rect.setRight(pt[0] + _param->curve->tangent(_index+1)[2] - 1e-2);
    }

    p.setX(max(min(p.x(),rect.right()),rect.left()));
    p.setY(max(min(p.y(),qreal(_param->maxVal)),qreal(_param->minVal)));

    _param->curve->setKeyframe(Vector2f(p.x(),p.y()),_index);
    Vector2f pt = _param->curve->point(_index);
    _timelineWidget->position()->setPosition(pt[0],pt[1]);

    if(_tangentLeft)
        _tangentLeft->updatePos();
    if(_tangentRight)
        _tangentRight->updatePos();

    return QPointF(pt[0],pt[1]);
  } else if(change==QGraphicsItem::ItemSelectedChange) {
    _timelineWidget->position()->setPosition(pos().x(),pos().y());
  }
  
  return QGraphicsItem::itemChange(change, value);
}




Frame::Frame(TimelineWidget *timelineWidget,AnimationWidget *animWidget,GraphWidget *graphWidget)
  : _timelineWidget(timelineWidget),
    _animWidget(animWidget),
    _graphWidget(graphWidget),
    _animation(_graphWidget->animation()) {
  setZValue(-1);
  setFlags(ItemIsMovable | ItemIgnoresTransformations | ItemSendsGeometryChanges | ItemIsSelectable);
  computePos();
}
  
QRectF Frame::boundingRect() const {
  const qreal s = 2;
  return QRectF(-s,0,s*2,(float)_timelineWidget->height());
}

void Frame::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen pen(Qt::darkBlue,2,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter->setPen(pen);
  painter->drawLine(QPointF(0,0),QPointF(0,(float)_timelineWidget->height()));
}

QVariant Frame::itemChange(GraphicsItemChange change, const QVariant &value) {
  
  if(change == QGraphicsItem::ItemPositionChange && scene()) {
    QPointF p1 = value.toPointF();
    QPointF p2 = _timelineWidget->mapToScene(QPoint(0,0));
    p1.setX(max(min(round(p1.x()),(double)_animation->lastFrame()),(double)_animation->firstFrame()));
    p1.setY(p2.y());

    return p1;
  }
  return QGraphicsItem::itemChange(change, value);
}

void Frame::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  setSelected(false);
}

void Frame::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {
  QGraphicsItem::mouseMoveEvent(event);
  _animation->setCurrentFrame((unsigned int)pos().x());
  _animWidget->animationFrameChanged();
}

void Frame::mouseReleaseEvent (QGraphicsSceneMouseEvent * event) {
  QGraphicsItem::mouseReleaseEvent(event);
  setSelected(false);
}










TangentPoint::TangentPoint(TimelineWidget *timelineWidget, TimelineWidget::Param *p, unsigned int index, unsigned int side)
  : _hasChanged(false),
    _timelineWidget(timelineWidget),
    _param(p), 
    _index(index),
    _side(side),
    _ctrl(false),
    _shift(false),
    _symmetric(false),
    _sibling(NULL) {

  if(_param->curve->useTangents()) {
    updatePos();
    setFlags(ItemIsMovable | ItemIgnoresTransformations | ItemSendsGeometryChanges | ItemIsSelectable);
    setZValue(1);
  } else {
    setFlags(ItemIgnoresTransformations);
  }

}

void TangentPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  if(!_param->curve->useTangents()) return;

  QPen p(Qt::red,_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  painter->setPen(p);
  painter->drawPoint(0,0);
  p.setWidth(_size-2);
  p.setColor(_param->varColor);
  painter->setPen(p);
  painter->drawPoint(0,0);
}


QRectF TangentPoint::boundingRect() const {
  qreal s= _size/2;
  return QRectF(-s,-s,_size,_size);
}

void TangentPoint::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if(!_param->curve->useTangents()) {
    QGraphicsItem::mousePressEvent(event);
    return;
  }

  if(event->button()==Qt::LeftButton) {
    QGraphicsItem::mousePressEvent(event);
    _timelineWidget->select(_param->varName);
    _timelineWidget->boundaries()->setLimits(_param->minVal,_param->maxVal);
    _timelineWidget->boundaries()->show();
    setSelected(true);
  }
}

void TangentPoint::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {
  if(!_param->curve->useTangents()) {
    QGraphicsItem::mouseMoveEvent(event);
    return;
  }

  _ctrl  = (event->modifiers() & Qt::ControlModifier);
  _shift = (event->modifiers() & Qt::ShiftModifier);
  QGraphicsItem::mouseMoveEvent(event);
  _timelineWidget->curveChanged(_param->nodeWidget);
  _timelineWidget->refresh();
}

void TangentPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
  setSelected(false);
}

QVariant TangentPoint::itemChange(GraphicsItemChange change, const QVariant &value) {
  if(!_param->curve->useTangents()) return QGraphicsItem::itemChange(change, value);

  if(change == QGraphicsItem::ItemPositionChange && scene()) {
    QPointF p = value.toPointF();

    if(_shift) {
      const QPointF s = _timelineWidget->sceneStep()/10.0;
      p.setX(closestVal(p.x(),s.x()));
      p.setY(closestVal(p.y(),s.y()));
    } else if(_ctrl) {
      const QPointF s = _timelineWidget->sceneStep()/4.0;
      p.setX(closestVal(p.x(),s.x()));
      p.setY(closestVal(p.y(),s.y()));
    }

    Vector2f pt = _param->curve->point(_index);
    if(_side==CurveInterpolator::RIGHT_TANGENT  && p.x()<pt[0])
        p.setX(pt[0]+1e-3);
    if(_side==CurveInterpolator::LEFT_TANGENT && p.x()>pt[0])
        p.setX(pt[0]-1e-3);

    if(_side==CurveInterpolator::RIGHT_TANGENT && _index+1 < _param->curve->nbPoints()){
        Vector2f pt_next = _param->curve->point(_index+1);
        if(p.x() > pt_next[0])
            p.setX(pt_next[0]-1e-2);
    }
    if(_side==CurveInterpolator::LEFT_TANGENT && _index > 0){
        Vector2f pt_prev = _param->curve->point(_index-1);
        if(p.x() < pt_prev[0])
            p.setX(pt_prev[0]+1e-2);
    }

    const Vector2f t(p.x()-pt[0], p.y()-pt[1]);
    _param->curve->setTangent(t,_index,_side);

    _hasChanged = true;
    if(_symmetric && _sibling && !_sibling->_hasChanged){
        Vector2f prev_t = _param->curve->tangent(_index).segment<2>(2*((_side+1)%2));
        _param->curve->setTangent(-prev_t.norm()*t.normalized(),_index,(_side+1)%2);
        _sibling->updatePos();
    }
    _hasChanged = false;

    return computePos();
  } 
  
  return QGraphicsItem::itemChange(change, value);
}

void TangentPoint::toggleSymmetric() {
    _symmetric = !_symmetric;
    if(_symmetric && _sibling) {
        Vector4f t = _param->curve->tangent(_index);
        _param->curve->setTangent(-t.segment<2>(2*((_side+1)%2)).norm()*t.segment<2>(2*_side).normalized(),_index,(_side+1)%2);
        _sibling->updatePos();
    }
}

TangentLine::TangentLine(TimelineWidget::Param *param,ControlPoint *p,TangentPoint *t) 
  : _param(param),
    _p(p),
    _t(t) {
} 

void TangentLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  if(!_param->curve->useTangents()) return;
  QPen p(Qt::black,1);
  p.setCosmetic(true);
  painter->setPen(p); 
  painter->drawLine(_p->pos(),_t->pos());
}

QRectF TangentLine::boundingRect() const {
  if(!_param->curve->useTangents()) return QRectF(0,0,0,0);

  const QPointF p1 = _p->pos();
  const QPointF p2 = _t->pos();
  const qreal xmin = min(p1.x(),p2.x());
  const qreal xmax = max(p1.x(),p2.x());
  const qreal ymin = min(p1.y(),p2.y());
  const qreal ymax = max(p1.y(),p2.y());
  return QRectF(xmin,ymin,xmax-xmin,ymax-ymin);
}
