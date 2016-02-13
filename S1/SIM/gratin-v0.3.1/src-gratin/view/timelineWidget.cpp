// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/timelineWidget.h"
#include "view/graphWidget.h"
#include "view/animationWidget.h"
#include "view/timelineItems.h"
#include "view/nodeWidget.h"
#include "core/curve.h"

using namespace std;

TimelineWidget::TimelineWidget(GraphWidget *graphWidget,AnimationWidget *animWidget,QWidget *parent)
  : QGraphicsView(parent),
    _graphWidget(graphWidget),
    _animation(_graphWidget->animation()),
    _animWidget(animWidget),
    _sceneStep(1,1),
    _viewportStep(1,1),
    _mouseCoord(0,0),
    _mouseTmp(0,0),
    _interaction(NO_MOTION),
    _ctrl(false),
    _shift(false),
    _needToDeleteControlPoint(false),
    _needToAddControlPoint(false),
    _frame(new Frame(this,_animWidget,_graphWidget)),
    _boundaries(new BoundaryDrawer(this)),
    _position(new PositionDrawer(this,_animWidget->currentFrame(),_mouseCoord.x(),_mouseCoord.y())),
    _params(vector<Param *>()),
    _points(vector<vector<ControlPoint *> >()),
    _tangents(vector<vector<TangentPoint *> >()),
    _tlines(vector<vector<TangentLine *> >()),
    _curves(vector<CurveDrawer *>()) {

  _scene = new QGraphicsScene(this);
  _scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  setScene(_scene);
  setCacheMode(CacheBackground);
  setViewportUpdateMode(BoundingRectViewportUpdate);
  setRenderHint(QPainter::Antialiasing);
  setMinimumSize(500,50);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  //setDragMode(QGraphicsView::RubberBandDrag);

  _boundaries->hide();
  resetTimelineRect();
  resetSceneRect();

  _frame->updatePos();
  _position->computePos();

  _scene->addItem(_frame);
  _scene->addItem(_boundaries);
  _scene->addItem(_position);
}

TimelineWidget::~TimelineWidget() {
  clean();
  delete _frame;
  delete _boundaries;
  delete _position;
}

void TimelineWidget::updateId(const QStringList &oldId,const QStringList &newId) {
  for(unsigned int i=0;i<_params.size();++i) {

    if(_params[i]->varName==oldId) {
      _params[i]->varName = QStringList(newId);
      break;
    }
  }
}

void TimelineWidget::timelineRectChanged() {
  resetTimelineRect();
  resetSceneRect(false);
  refresh();
}

void TimelineWidget::currentFrameChanged() {
  _frame->updateFrame();
}

void TimelineWidget::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  _frame->updatePos();
  _position->computePos();
  refresh();
}

void TimelineWidget::resetTimelineRect() {
  const float left   = (float)_animation->firstFrame();
  const float right  = (float)_animation->lastFrame();
  float bottom = 0.0f;
  float top    = 1.0f;

  for(unsigned int i=0;i<_params.size();++i) {
    bottom = min(bottom,_params[i]->minVal);
    top    = max(top,_params[i]->maxVal);
  }
  
  const QRectF newRect(left,bottom,fabs(right-left),fabs(top-bottom));
  const QRectF oldRect(_timelineRect);
  _timelineRect = newRect;

  remapControlPointsInTimelineRect(oldRect,newRect);
}

void TimelineWidget::remapControlPointsInTimelineRect(const QRectF &r1,const QRectF &r2) {
  const float x1 = r1.left();
  const float x2 = r2.left();
  const float w1 = r1.width();
  const float w2 = r2.width();

  for(unsigned int i=0;i<_points.size();++i) {
    const std::vector<Vector2f> pts = _params[i]->curve->points();
    for(unsigned int j=0;j<pts.size();++j) {
      const float p = ((pts[j][0]-x1)/w1)*w2+x2;
      const QPointF cpos(p,pts[j][1]);
      _points[i][j]->setPos(cpos);
    }
  }
}

void TimelineWidget::resetSceneRect(bool transform) {
  QPointF topleft = _timelineRect.topLeft();
  QSizeF  size    = _timelineRect.size();
  QSizeF  offset  = size/2.0;
  QSizeF  total   = size+offset;

  topleft.setX(topleft.x()-offset.width()/2.0);
  topleft.setY(topleft.y()-offset.height()/2.0);

  _scene->setSceneRect(QRectF(topleft,total));

  if(transform) {
    resetTransform();
    scaleView(1.0/(total.width()/(float)width()),-1.0/(total.height()/(float)height()));
  }
}

void TimelineWidget::drawBackground(QPainter *painter, const QRectF &) {
  // dimensions for drawing the text
  const double h = 15.0;
  const double s = 4.0;
  const QRectF hrect = QRectF(QPointF(0,(double)height()-h-s*2),QSize((double)width(),h+s*2));
  const QRectF vrect = QRectF(QPointF(0,0),QSize(h+s,height()));

  // get the first point aligned with the grid
  const QPointF startScene    = closestPoint(mapToScene(QPoint(0,0)),_sceneStep);
  const QPointF endScene      = closestPoint(mapToScene(QPoint(width(),height())),_sceneStep);
  const QPointF s2            = _viewportStep/2.0;

  // disable the transformation (we draw on the entire viewport)
  painter->setWorldMatrixEnabled(false);

  // paint background 
  painter->fillRect(QRect(QPoint(0,0),QPoint(width(),height())),Qt::lightGray);

  // paint scene rectangle
  QPoint p1 = mapFromScene(_timelineRect.bottomLeft());
  QPoint p2 = mapFromScene(_timelineRect.topRight());
  p1.setX(std::max(p1.x(),0));
  p2.setX(std::min(p2.x(),width()));
  p1.setY(0);
  p2.setY(height());
  painter->fillRect(QRect(p1,p2),Qt::darkGray);

  QFont font = painter->font();
  font.setPointSize(10);
  painter->setFont(font);

  // drawing vertical lines
  double lx = startScene.x();
  while(lx<endScene.x()) {
    if(abs(lx)<1.0e-10) lx = 0.0;
    const QPointF p = mapFromScene(QPointF(lx,0));
    const QRectF  r(QPointF(p.x()-s2.x(),hrect.top()),QSize(_viewportStep.x(),hrect.height()));
    const QString t = QString::number(lx);
    painter->setPen(Qt::gray);
    painter->drawLine(QPointF(p.x(),0),QPointF(p.x(),height()));

    if(p.x()>s2.x()) {
      painter->setPen(Qt::black);
      painter->drawText(r,Qt::AlignHCenter,t);
    }

    lx += _sceneStep.x();
  }

  // drawing horizontal lines 
  double ly = startScene.y();
  while(ly>endScene.y()) {
    if(abs(ly)<1.0e-10) ly = 0.0;
    const QPointF p = mapFromScene(QPointF(0,ly));
    const QRectF  r(QPointF(vrect.left(),p.y()-hrect.height()/2.0),QSize(_viewportStep.x(),hrect.height()));
    const QString t = QString::number(ly);
    painter->setPen(Qt::gray);
    painter->drawLine(QPointF(0,p.y()),QPointF(width(),p.y()));

    if(p.y()<height()-s2.y()) {
      painter->setPen(Qt::black);
      painter->drawText(r,Qt::AlignVCenter,t);
    }

    ly -= _sceneStep.y();
  }

  // back to the original state 
  painter->setWorldMatrixEnabled(true);
}

void TimelineWidget::setCurrentFrame(const QPointF &pt) {
  double f = max(min(round(pt.x()),(double)_animation->lastFrame()),(double)_animation->firstFrame());
  _animation->setCurrentFrame((unsigned int)f);
  _animWidget->animationFrameChanged();
  refresh();
}

void TimelineWidget::wheelEvent(QWheelEvent *event) {
  const double s = 1000.0;

  if(_ctrl) 
    scaleView(pow(2.0,-event->delta()/s),1);
  else if(_shift)
    scaleView(1,pow(2.0,-event->delta()/s));
  else 
    scaleView(pow(2.0,-event->delta()/s));

  _frame->updatePos();
  _position->computePos();

  setFocus();
}

void TimelineWidget::keyPressEvent(QKeyEvent *event) {
  if(event->modifiers() & Qt::ControlModifier) {
    // control is pressed
    _ctrl = true;
  } 

  if(event->modifiers() & Qt::ShiftModifier) {
    // shift is pressed
    _shift = true;
  } 

  switch (event->key()) {
  case Qt::Key_Plus:
    zoomIn();
    break;
  case Qt::Key_Minus:
    zoomOut();
    break;
  default:
    QGraphicsView::keyPressEvent(event);
  }

  _frame->updatePos();
  _position->computePos();
  setFocus();
}

void TimelineWidget::keyReleaseEvent(QKeyEvent *event) {
  if(!(event->modifiers() & Qt::ControlModifier)) {
    _ctrl = false;
  } 
  
  if(!(event->modifiers() & Qt::ShiftModifier)) {
    _shift = false;
  } 

  _frame->updatePos();
  _position->computePos();
  setFocus();
}

void TimelineWidget::mousePressEvent(QMouseEvent *event) {
  if(_ctrl && event->button()==Qt::MidButton) {
    _needToDeleteControlPoint = true;
    _interaction = NO_MOTION;
  } if(_ctrl && event->button()==Qt::RightButton) {
    _needToAddControlPoint = true;
    _interaction = NO_MOTION;
  } else if(event->button()==Qt::MidButton) {
    // zoom in and out
    _interaction = SCALING_MOTION;
    _mouseCoord = QPointF(event->x(),event->y());
  } else if(event->button()==Qt::RightButton) {
    // translation
    setTransformationAnchor(QGraphicsView::NoAnchor);
    _interaction = OFFSET_MOTION;
    _mouseCoord = QPointF(event->x(),event->y());
  } else {
    _interaction = NO_MOTION;
  }
  
  _frame->updatePos();
  _position->computePos();

  QGraphicsView::mousePressEvent(event);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event) {
  if(_interaction==NO_MOTION && _needToDeleteControlPoint) {
    _needToDeleteControlPoint = false;
  } else if(_interaction==SCALING_MOTION) {
    const double scale = 500;

    _mouseTmp = QPointF(event->x(),event->y())-_mouseCoord;

    if(_ctrl) {
      // horizontal scaling 
      scaleView(pow(2.0,_mouseTmp.x()/scale),1);
    } else if(_shift) {
      // vertical scaling
      scaleView(1,pow(2.0,_mouseTmp.y()/scale));
    } else {
      // simple scaling
      scaleView(pow(2.0,_mouseTmp.x()/scale),pow(2.0,_mouseTmp.y()/scale));
    }
    
    _mouseCoord = QPointF(event->x(),event->y());

  } else if(_interaction==OFFSET_MOTION) {
    // background translations
    _mouseTmp = QPointF(event->x(),event->y());
    const QPoint tmp((int)_mouseTmp.x(),(int)_mouseTmp.y());
    const QPoint coord((int)_mouseCoord.x(),(int)_mouseCoord.y());
    const QPointF diff = mapToScene(tmp)-mapToScene(coord);
    QPointF t;
    if((_mouseTmp.x()-_mouseCoord.x())>0) 
      t.setX(fabs(diff.x()));
    else 
      t.setX(-fabs(diff.x()));

    if((_mouseTmp.y()-_mouseCoord.y())<0) 
      t.setY(fabs(diff.y()));
    else 
      t.setY(-fabs(diff.y()));

    translate(t.x(),t.y());
    _mouseCoord = _mouseTmp;
  }

  _frame->updatePos();
  _position->computePos();

  QGraphicsView::mouseMoveEvent(event);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event) {
  if(_needToDeleteControlPoint) {
    _needToDeleteControlPoint = false;
    ControlPoint *p = qgraphicsitem_cast<ControlPoint *>(itemAt(event->pos()));
    if(p && p->isSelected()) {
      delControlPoint(p);
    }
  }

  if(_needToAddControlPoint) {
    _needToAddControlPoint = false;
    addControlPoint(mapToScene(event->pos()));
  }
  _interaction = NO_MOTION;
  _frame->updatePos();
  _position->computePos();

  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  QGraphicsView::mouseReleaseEvent(event);
  refresh();
}

void TimelineWidget::addControlPoint(const QPointF &p) {
    for(unsigned int i=0;i<_params.size();++i) {
        if(_params[i]->selected) {
            if(p.x()>= _timelineRect.left() &&
                    p.x()<= _timelineRect.right() &&
                    p.y()>= _timelineRect.top() &&
                    p.y()<= _timelineRect.bottom()) {
                const Vector2f v(p.x(),p.y());
                _params[i]->curve->addKeyframe(v);
                controlPointsChanged(_params[i]->varName);
            }
            break;
        }
    }
}


void TimelineWidget::delControlPoint(ControlPoint *p) {
  for(unsigned int i=0;i<_params.size();++i) {
    
    if(_params[i]->varName==p->varName()) {
      _params[i]->curve->delKeyframe(p->index());
      controlPointsChanged(_params[i]->varName);
      break;
    }
  }
  refresh();
}

void TimelineWidget::addCurve(NodeWidget *widget,const QStringList &varName,Curve *curve,const QColor c,float minVal,float maxVal) {
  const unsigned int index = static_cast<unsigned int>(_params.size());

  _params.push_back(new Param());
  _params[index]->curve      = curve;
  _params[index]->nodeWidget = widget;
  _params[index]->varColor   = c;
  _params[index]->varName    = varName;
  _params[index]->minVal     = minVal;
  _params[index]->maxVal     = maxVal;
  _params[index]->selected   = false;

  timelineRectChanged();

  _curves.push_back(new CurveDrawer(this,_params[index]));
  _tangents.push_back(vector<TangentPoint *>());
  for(unsigned int i=0;i<curve->nbTangents();++i) {
      _tangents[index].push_back(new TangentPoint(this,_params[index],i,CurveInterpolator::RIGHT_TANGENT));
      _scene->addItem(_tangents[index][2*i]);
      _tangents[index].push_back(new TangentPoint(this,_params[index],i,CurveInterpolator::LEFT_TANGENT));
      _scene->addItem(_tangents[index][2*i+1]);
  }

  _points.push_back(vector<ControlPoint *>());
  for(unsigned int i=0;i<curve->nbPoints();++i) {
    TangentPoint *t1 = _tangents[index].size()>(2*i)   ? _tangents[index][2*i]   : NULL;
    TangentPoint *t2 = _tangents[index].size()>(2*i+1) ? _tangents[index][2*i+1] : NULL;
    if(t1 && t2){
        t1->setSibling(t2);
        t2->setSibling(t1);
        const Vector4f &tangent = _params[index]->curve->tangent(i);
        if(tangent.head<2>().normalized().dot(-tangent.tail<2>().normalized())>0.99){
            t1->toggleSymmetric();
            t2->toggleSymmetric();
        }
    }
    _points[index].push_back(new ControlPoint(this,_params[index],i,t1,t2));
    _scene->addItem(_points[index][i]);
  }

  _tlines.push_back(vector<TangentLine *>());
  if(curve->nbTangents()>0){
      for(unsigned int i=0;i<curve->nbPoints();++i) {
          _tlines[index].push_back(new TangentLine(_params[index],_points[index][i],_tangents[index][2*i]));
          _scene->addItem(_tlines[index][2*i]);
          _tlines[index].push_back(new TangentLine(_params[index],_points[index][i],_tangents[index][2*i+1]));
          _scene->addItem(_tlines[index][2*i+1]);
      }
  }
  
  _scene->addItem(_curves[index]);
  select(varName);
}

void TimelineWidget::delCurve(const QStringList &varName) {
  int index = -1;
  for(unsigned int i=0;i<_params.size();++i) {

    if(_params[i]->varName==varName) {
      index = i;
      break;
    }
  }

  if(index==-1) return;

  // remove control points 
  for(unsigned int j=0;j<_points[index].size();++j) {
    _scene->removeItem(_points[index][j]);
    delete _points[index][j];
  }

  // remove tangent points 
  for(unsigned int j=0;j<_tangents[index].size();++j) {
    _scene->removeItem(_tangents[index][j]);
    delete _tangents[index][j];
  }

  // remove tangent lines
  for(unsigned int j=0;j<_tlines[index].size();++j) {
    _scene->removeItem(_tlines[index][j]);
    delete _tlines[index][j];
  }

  // remove curve 
  _scene->removeItem(_curves[index]);  
  delete _curves[index];
  delete _params[index];

  // rescale arrays 
  _points.erase(_points.begin()+index);
  _tangents.erase(_tangents.begin()+index);
  _tlines.erase(_tlines.begin()+index);
  _curves.erase(_curves.begin()+index);
  _params.erase(_params.begin()+index);
}


void TimelineWidget::controlPointsChanged(const QStringList &varName) {
    for(unsigned int i=0;i<_params.size();++i) {
        if(_params[i]->varName==varName) {

            // remove all previous items
            for(unsigned int j=0;j<_points[i].size();++j) {
                _scene->removeItem(_points[i][j]);
                delete _points[i][j];
            }
            _points[i].clear();

            for(unsigned int j=0;j<_tangents[i].size();++j) {
                _scene->removeItem(_tangents[i][j]);
                delete _tangents[i][j];
            }
            _tangents[i].clear();

            for(unsigned int j=0;j<_tlines[i].size();++j) {
                _scene->removeItem(_tlines[i][j]);
                delete _tlines[i][j];
            }
            _tlines[i].clear();

            // add all items
            for(unsigned int j=0;j<_params[i]->curve->nbTangents();++j) {
                _tangents[i].push_back(new TangentPoint(this,_params[i],j,0));
                _scene->addItem(_tangents[i][_tangents[i].size()-1]);
                _tangents[i].push_back(new TangentPoint(this,_params[i],j,1));
                _scene->addItem(_tangents[i][_tangents[i].size()-1]);
            }

            for(unsigned int j=0;j<_params[i]->curve->nbPoints();++j) {
                TangentPoint *tl = _tangents[i].size()>2*j   ? _tangents[i][2*j]   : NULL;
                TangentPoint *tr = _tangents[i].size()>2*j+1 ? _tangents[i][2*j+1] : NULL;
                if(tl && tr){
                    tl->setSibling(tr);
                    tr->setSibling(tl);
                    const Vector4f &tangent = _params[i]->curve->tangent(j);
                    if(tangent.head<2>().normalized().dot(-tangent.tail<2>().normalized())>0.99){
                        tl->toggleSymmetric();
                        tr->toggleSymmetric();
                    }
                }
                _points[i].push_back(new ControlPoint(this,_params[i],j,tl,tr));
                _scene->addItem(_points[i][_points[i].size()-1]);
            }

            for(unsigned int j=0;j<min(_params[i]->curve->nbTangents(),_params[i]->curve->nbPoints());++j) {
                _tlines[i].push_back(new TangentLine(_params[i],_points[i][j],_tangents[i][2*j]));
                _scene->addItem(_tlines[i][_tlines[i].size()-1]);
                _tlines[i].push_back(new TangentLine(_params[i],_points[i][j],_tangents[i][2*j+1]));
                _scene->addItem(_tlines[i][_tlines[i].size()-1]);
            }

            break;
        }
    }
}

void TimelineWidget::select(const QStringList &varName) {
  for(unsigned int i=0;i<_params.size();++i) {    
    if(_params[i]->varName==varName) {
      _params[i]->selected = true;
    } else {
      _params[i]->selected = false;
    }
  }
}

void TimelineWidget::clean() {
  for(unsigned int i=0;i<nbCurves();++i) {
    _scene->removeItem(_curves[i]);
    delete _curves[i];
    
    for(unsigned int j=0;j<_points[i].size();++j) {
      _scene->removeItem(_points[i][j]);
      delete _points[i][j];
    }

    for(unsigned int j=0;j<_tangents[i].size();++j) {
      _scene->removeItem(_tangents[i][j]);
      delete _tangents[i][j];
    }

    for(unsigned int j=0;j<_tlines[i].size();++j) {
      _scene->removeItem(_tlines[i][j]);
      delete _tlines[i][j];
    }
  }

  for(unsigned int i=0;i<_params.size();++i) {
    delete _params[i];
  }

  _params.clear();
  _curves.clear();
  _points.clear();
  _tangents.clear();
  _tlines.clear();
}

void TimelineWidget::hideCurve(const QStringList &varName) {
  for(unsigned int i=0;i<_params.size();++i) {    
    if(_params[i]->varName==varName) {
      for(unsigned int j=0;j<_points[i].size();++j) {
	_points[i][j]->hide();
      }
      for(unsigned int j=0;j<_tangents[i].size();++j) {
	_tangents[i][j]->hide();
      }
      for(unsigned int j=0;j<_tlines[i].size();++j) {
	_tlines[i][j]->hide();
      }
      _curves[i]->hide();
      break;
    }
  }
}

void TimelineWidget::showCurve(const QStringList &varName) {
  for(unsigned int i=0;i<_params.size();++i) {    
    if(_params[i]->varName==varName) {
      for(unsigned int j=0;j<_points[i].size();++j) {
	_points[i][j]->show();
      }
      for(unsigned int j=0;j<_tangents[i].size();++j) {
	_tangents[i][j]->show();
      }
      for(unsigned int j=0;j<_tlines[i].size();++j) {
	_tlines[i][j]->show();
      }
      _curves[i]->show();
      break;
    }
  }
}

void TimelineWidget::cleanCurve(const QStringList &varName) {
  for(unsigned int i=0;i<_params.size();++i) {    
    if(_params[i]->varName==varName) {
      Curve *c = _params[i]->curve;
      const int nb = (int)(c->nbPoints())-1;
      for(int i=nb;i>0;--i) {
	c->delKeyframe(i);
      }
      controlPointsChanged(varName);
    }
  }
}


void TimelineWidget::updateScaleDependentItems() {
  // tangents are viewport-dependent
  for(unsigned int i=0;i<_tangents.size();++i) {
    for(unsigned int j=0;j<_tangents[i].size();++j) {
      _tangents[i][j]->updatePos();
    }
  }
}

void TimelineWidget::curveChanged(NodeWidget *widget) {
  //_graphWidget->nodeCurveChanged(widget->nodeIds()[0]);
  _graphWidget->nodeCurveChanged(widget->node());
  //cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << " - " << widget->nodeName().toStdString() << " -- " << widget->nodeIds()[0] << endl;
}

bool TimelineWidget::save(SceneSaver *) {
  return true;
}

bool TimelineWidget::load(SceneLoader *) {
  _ctrl                     = false;
  _shift                    = false;
  _needToAddControlPoint    = false;
  _needToDeleteControlPoint = false;
  _interaction = NO_MOTION;

  resetTimelineRect();
  resetSceneRect(false);
  _frame->updatePos();
  _position->computePos();

  return true;
}
