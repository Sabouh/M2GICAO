#include "eventWidget.h"
#include <QVBoxLayout>

EventWidget::EventWidget(PointsToSurface &pts,QWidget *parent)
  : QDockWidget("Options",parent),
    _pts(pts),
    _w(new QWidget()) {

  QVBoxLayout *l = new QVBoxLayout();
  _inputPoints = new QCheckBox("Show input points");
  _nonOrientedNormals = new QCheckBox("Show non-oriented normals");
  _orientedNormals = new QCheckBox("Show oriented normals");
  _minSpanningTree = new QCheckBox("Show minimal spanning tree");
  _surface = new QCheckBox("Show output surface");
  _invertNormals = new QCheckBox("invert normals");
  _box = new QCheckBox("Show box");
  _mode = new QComboBox();

  _mode->addItem("Smooth rend");
  _mode->addItem("Flat rend");
  _mode->addItem("Wire rend");

  _inputPoints->setCheckState(Qt::Checked);

  l->addWidget(_inputPoints);
  l->addWidget(_nonOrientedNormals);
  l->addWidget(_orientedNormals);
  l->addWidget(_minSpanningTree);
  l->addWidget(_surface);
  l->addWidget(_invertNormals);
  l->addWidget(_box);
  l->addWidget(_mode);

  delete _w->layout();
  _w->setLayout(l);

  setWidget(_w);
}

EventWidget::~EventWidget() {

}
