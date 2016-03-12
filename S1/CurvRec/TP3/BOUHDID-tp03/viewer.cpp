#include <QtGui>

#include "viewer.h"

Viewer::Viewer(const QImage &img,const QString &title) {
  _image = new QLabel;
  _image->setBackgroundRole(QPalette::Base);
  _image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _image->setScaledContents(true);

  _scroll = new QScrollArea;
  _scroll->setBackgroundRole(QPalette::Dark);
  _scroll->setWidget(_image);
  setCentralWidget(_scroll);

  img.convertToFormat(QImage::Format_ARGB32);
  setImage(img);

  setWindowTitle(title);
  resize(512,512);
}

void Viewer::setImage(const QImage &img) {
  _image->setPixmap(QPixmap::fromImage(img));
  _scale = 1.0;
  _image->adjustSize();
}

 
void Viewer::zoomIn() {
  scaleImage(1.25);
}

void Viewer::zoomOut() {
  scaleImage(0.8);
}

void Viewer::normalSize() {
  _image->adjustSize();
  _scale = 1.0;
}

void Viewer::scaleImage(double factor) {
  Q_ASSERT(_image->pixmap());
  _scale *= factor;
  _image->resize(_scale * _image->pixmap()->size());

  adjustScrollBar(_scroll->horizontalScrollBar(),factor);
  adjustScrollBar(_scroll->verticalScrollBar(),factor);
}

void Viewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
  scrollBar->setValue(int(factor * scrollBar->value()
			  + ((factor - 1) * scrollBar->pageStep()/2)));
}

void Viewer::wheelEvent(QWheelEvent *event) {
  int numDegrees = event->delta() / 8;
  int numSteps = numDegrees / 15;

  if(numSteps<0) {
    for(int i=0;i<-numSteps;++i) {
      zoomOut();
    }
  } else {
    for(int i=0;i<numSteps;++i) {
      zoomIn();
    }
  }
  event->accept();
}

void Viewer::mouseDoubleClickEvent(QMouseEvent * event) {
  _scale = 1.0;
  _image->adjustSize();
  event->accept();
}
