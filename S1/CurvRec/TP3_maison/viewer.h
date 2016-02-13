#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>

class Viewer : public QMainWindow {
  Q_OBJECT

    public:
  Viewer(const QImage &img,const QString &title="Image");
  
  private slots:
  void zoomIn();
  void zoomOut();
  void normalSize();
  void setImage(const QImage &img);

 protected:
  void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent * event);

 private:
  void scaleImage(double factor);
  void adjustScrollBar(QScrollBar *scrollBar, double factor);
  
  QLabel      *_image;
  QScrollArea *_scroll;
  double       _scale;
};

#endif // VIEWER
