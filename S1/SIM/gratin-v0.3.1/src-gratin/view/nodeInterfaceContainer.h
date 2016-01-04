// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_INTERFACE_CONTAINER_H
#define NODE_INTERFACE_CONTAINER_H

#include <QWidget>
#include <QDialog>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPoint>
#include <vector>

class InterfaceTitle : public QWidget {
 Q_OBJECT

   public:
  InterfaceTitle(int index,const QString &title,QWidget *parent=NULL);

  void setTitle(const QString &title) { _title->setText(title); }
  void setIndex(int index) { _index = index; }
  int index()     const { return _index; }
  QString title() const { return _title->text(); }
  void highlight(bool h);

 signals:
  void helpNeeded(int,const QPoint &);
  void closeNeeded(int);
  void highlightNeeded(int);
  void externalNeeded(int);

  public slots:
  void helpClicked();
  void closeClicked();
  void externalClicked();

 protected:
  void mousePressEvent(QMouseEvent *);

 private:
  int          _index;
  QLabel      *_title;
  QToolButton *_helpButton;
  QToolButton *_closeButton;
  QToolButton *_externalButton;
  QHBoxLayout *_layout;
};

class NodeExternalContainer : public QDialog {
  Q_OBJECT
    
    public:
  NodeExternalContainer(QWidget *parent=NULL);

  void setWidget(QWidget *nodeInterface,const QString &title);
  QWidget *nodeInterface() {return _nodeInterface;}
  QString title() const { return _title;}
  void setTitle(const QString &title);
  void clean();

 signals:
  void containerClosed();

 protected:
  void closeEvent(QCloseEvent * event);

 private:
  QWidget *_nodeInterface;
  QString  _title;
  QVBoxLayout *_layout;
};

class NodeInterfaceContainer : public QWidget {
  Q_OBJECT
    
    public:
  NodeInterfaceContainer(QWidget *parent=NULL);
  ~NodeInterfaceContainer();

  inline int count                () const   { return (int)_widgets.size(); }
  inline int currentIndex         () const   { return _currentIndex; }
  inline QWidget *currentWidget   ()         { return currentIndex()>=0 ? _widgets[currentIndex()] : NULL; }
  inline QWidget *widget(int index)          { return index<count() && index>=0 ? _widgets[index] : NULL; }
  inline QWidget *externalWidget  ()         { return _externalContainer->nodeInterface();}
 
  void removeItem(int index);
  void removeExternalWidget();
  void showExternalWidget();
  int  addItem(QWidget *widget,const QString &title);
  void setItemText(int index,const QString &title);
  void setExternalItemText(const QString &title);

  public slots:
  void setCurrentIndex(int);

  private slots:
  void showHelp(int,const QPoint &);
  void closeWidget(int);
  void showExternalWidget(int);
  void externalWidgetClosed();

 private:
  void addItems(InterfaceTitle *title,QWidget *widget);
  void removeItems(InterfaceTitle *title,QWidget *widget);

  int _currentIndex;
  std::vector<QWidget *>        _widgets;
  std::vector<InterfaceTitle *> _titles;
  QVBoxLayout *_layout;

  NodeExternalContainer *_externalContainer;
};

#endif // NODE_INTERFACE_CONTAINER_H
