#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include <QDockWidget>
#include <QCheckBox>
#include <QComboBox>
#include "pointsToSurface.h"

class EventWidget : public QDockWidget {
  Q_OBJECT
    
    public:
  EventWidget(PointsToSurface &pts,QWidget *parent=NULL);
  ~EventWidget();

  bool showInputPoints()         const {return _inputPoints->checkState()==Qt::Checked;}
  bool showNonOrientedNormals()  const {return _nonOrientedNormals->checkState()==Qt::Checked;}
  bool showOrientedNormals()     const {return _orientedNormals->checkState()==Qt::Checked;}
  bool showMinimalSpanningTree() const {return _minSpanningTree->checkState()==Qt::Checked;}
  bool showSurface()             const {return _surface->checkState()==Qt::Checked;}
  bool invertNormals()           const {return _invertNormals->checkState()==Qt::Checked;}
  bool box()                     const {return _box->checkState()==Qt::Checked;}
  int  mode()                    const {return _mode->currentIndex();}

 private:
  PointsToSurface _pts;

  QCheckBox *_inputPoints;
  QCheckBox *_nonOrientedNormals;
  QCheckBox *_orientedNormals;
  QCheckBox *_minSpanningTree;
  QCheckBox *_surface;
  QCheckBox *_invertNormals;
  QCheckBox *_box;
  QComboBox *_mode;

  QWidget *_w;
};

#endif // EVENT_WIDGET_H
