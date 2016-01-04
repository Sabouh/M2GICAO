// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/nodeInterfaceContainer.h"
#include "view/nodeWidget.h"
#include "core/nodeInterface.h"
#include <QStyle>
#include <QCursor>
#include <QToolTip>
#include <iostream>

using namespace std;

InterfaceTitle::InterfaceTitle(int index,const QString &title,QWidget *parent) 
  : QWidget(parent),
    _index(index) {

  _title = new QLabel(title,this);
  _helpButton = new QToolButton(this);
  _closeButton = new QToolButton(this);
  _externalButton = new QToolButton(this);
  _layout = new QHBoxLayout(this);

  //QPixmap pixc = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
  //QPixmap pixh = style()->standardPixmap(QStyle::SP_TitleBarContextHelpButton);

  //_closeButton->setIcon(pixc);
  //_helpButton->setIcon(pixh);

  _closeButton->setIcon(QIcon(":/close"));
  _helpButton->setIcon(QIcon(":/interrogation"));
  _externalButton->setIcon(QIcon(":/arrow-top-right"));

  _helpButton->setMinimumHeight(15);
  _closeButton->setMinimumHeight(15);
  _externalButton->setMinimumHeight(15);
  _title->setMinimumHeight(20);

  _layout->addWidget(_title);
  _layout->addWidget(_helpButton);
  _layout->addWidget(_externalButton);
  _layout->addWidget(_closeButton);
  
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        
  connect(_helpButton,SIGNAL(clicked()),this,SLOT(helpClicked()));
  connect(_closeButton,SIGNAL(clicked()),this,SLOT(closeClicked()));
  connect(_externalButton,SIGNAL(clicked()),this,SLOT(externalClicked()));

  setAutoFillBackground(true);
  //setBackgroundRole(QPalette::Highlight);
  
  setStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		"stop: 0 transparent, stop: 0.4 #DDDDDD,stop: 0.5 #D8D8D8, stop: 1.0 gray);"
		"border-radius: 6px;"
		"color: black;");
  
  _helpButton->setStyleSheet("QToolButton {"
			     "background: transparent;"
			     "border: 1px solid transparent;"
			     "color: black;"
			     "margin: 0px;}"
			     "QToolButton::hover {"
			     "border: 1px solid #EB3C3C;}");

  _closeButton->setStyleSheet("QToolButton {"
			      "background: transparent;"
			      "border: 1px solid transparent;"
			      "color: black;"
			      "margin: 0px;}"
			      "QToolButton::hover {"
			      "border: 1px solid #EB3C3C;}");
  
  _externalButton->setStyleSheet("QToolButton {"
				 "background: transparent;"
				 "border: 1px solid transparent;"
				 "color: black;"
				 "margin: 0px;}"
				 "QToolButton::hover {"
				 "border: 1px solid #EB3C3C;}");
  
  _title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  QFont f = _title->font();
  f.setPointSize(13);
  _title->setFont(f);
}

void InterfaceTitle::mousePressEvent(QMouseEvent *) {
  emit highlightNeeded(_index);
}

void InterfaceTitle::helpClicked() {
  emit helpNeeded(_index,QCursor::pos());
}

void InterfaceTitle::closeClicked() {
  emit closeNeeded(_index);
}

void InterfaceTitle::externalClicked() {
  emit externalNeeded(_index);
}

void InterfaceTitle::highlight(bool h) {
  QFont f = _title->font();

  if(h) {
    f.setItalic(true);
    f.setBold(true);
    f.setPointSize(15);
  } else {
    f.setItalic(false);
    f.setBold(false);
    f.setPointSize(13);
  }

  _title->setFont(f);

}

NodeExternalContainer::NodeExternalContainer(QWidget *parent)
  : QDialog(parent),
    _nodeInterface(NULL),
    _title("") {
  
  _layout = new QVBoxLayout(this);


  setWindowModality(Qt::NonModal);
  resize(500,700);
    }

void NodeExternalContainer::setWidget(QWidget *nodeInterface,const QString &title) {

  if(_nodeInterface) {
    _layout->removeWidget(_nodeInterface);
    _nodeInterface = NULL;
  }

  _nodeInterface = nodeInterface;
  _title = title;
  _layout->addWidget(nodeInterface);
  _nodeInterface->show();
  setWindowTitle(title);
}

void NodeExternalContainer::setTitle(const QString &title) {
  _title = title;
  setWindowTitle(title);
}

void NodeExternalContainer::closeEvent(QCloseEvent *) {
  emit containerClosed();
  clean();
}

void NodeExternalContainer::clean() {
  if(_nodeInterface) {
    _layout->removeWidget(_nodeInterface);
    _nodeInterface = NULL;
  }
}

NodeInterfaceContainer::NodeInterfaceContainer(QWidget *parent) 
  : QWidget(parent),
    _currentIndex(-1),
    _widgets(vector<QWidget *>()),
    _titles(vector<InterfaceTitle *>()) {
  _layout = new QVBoxLayout(this);
  _externalContainer = new NodeExternalContainer();
  connect(_externalContainer,SIGNAL(containerClosed()),this,SLOT(externalWidgetClosed()));
}

NodeInterfaceContainer::~NodeInterfaceContainer() {

  externalWidgetClosed();
  delete _externalContainer;

  int c = count();
  for(int i=c;i>=0;--i) {
    removeItem(i);
  }
}

void NodeInterfaceContainer::removeItem(int index) {
  if(index<0 || index>=count()) return;

  QWidget        *w = _widgets[index];
  InterfaceTitle *t = _titles[index];

  removeItems(t,w);

  delete t;
}

int NodeInterfaceContainer::addItem(QWidget *widget,const QString &title) {
  if(!widget) return _currentIndex;

  InterfaceTitle *barTitle = new InterfaceTitle(count(),title);

  addItems(barTitle,widget);

  connect(barTitle,SIGNAL(helpNeeded(int,const QPoint &)),this,SLOT(showHelp(int,const QPoint &)));
  connect(barTitle,SIGNAL(closeNeeded(int)),this,SLOT(closeWidget(int)));
  connect(barTitle,SIGNAL(highlightNeeded(int)),this,SLOT(setCurrentIndex(int)));
  connect(barTitle,SIGNAL(externalNeeded(int)),this,SLOT(showExternalWidget(int)));

  return _currentIndex;
}

void NodeInterfaceContainer::setCurrentIndex(int index) {
  if(index<0 || index>=count()) return;

  _currentIndex = index;
  if(count()==0) {
    _currentIndex = -1;
    return;
  }

  for(int i=0;i<count();++i) {
    if(i==_currentIndex) {
      _widgets[i]->show();
      _titles[i]->highlight(true);
    } else {
      _widgets[i]->hide();
      _titles[i]->highlight(false);
    }
  }
}

void NodeInterfaceContainer::setItemText(int index,const QString &title) {
  if(index<0 || index>=count()) return;
  _titles[index]->setTitle(title);
}

void NodeInterfaceContainer::setExternalItemText(const QString &title) {
  _externalContainer->setTitle(title);
}


void NodeInterfaceContainer::showHelp(int index,const QPoint &pos) {
  if(index<0 || index>=count()) return;

  NodeWidget *w = (NodeWidget *)_widgets[index];
  const QString help        = w->node()->help();
  const QStringList inputs  = w->node()->inputNames();
  const QStringList outputs = w->node()->outputNames();

  QString text;
  for(int i=0;i<inputs.size();++i) {
    text = text+"Input "+QString::number(i)+": "+inputs[i]+"\n";
  }
  text = text+"\n";
  for(int i=0;i<outputs.size();++i) {
    text = text+"Output "+QString::number(i)+": "+outputs[i]+"\n";
  }
  text = text+"\n"+help;

  QToolTip::showText(pos,text,this);
}

void NodeInterfaceContainer::closeWidget(int index) {
  removeItem(index);
}

void NodeInterfaceContainer::showExternalWidget(int index) {
  if(index<0 || index>=count()) return;

  QWidget *w = _externalContainer->nodeInterface();
  QString t = _externalContainer->title();

  if(w) {
    addItem(w,t);
  }

  t = _titles[index]->title();
  w = _widgets[index];

  removeItem(index);

  _externalContainer->setWidget(w,t);
  _externalContainer->show();
  _externalContainer->raise();
}

void NodeInterfaceContainer::externalWidgetClosed() {
  const QString t = _externalContainer->title();
  QWidget *w = _externalContainer->nodeInterface();

  if(w) {
    addItem(w,t);
  }

  _externalContainer->hide();
}

void NodeInterfaceContainer::removeExternalWidget() {
  _externalContainer->clean();
  _externalContainer->hide();
}

void NodeInterfaceContainer::showExternalWidget() {
  if(_externalContainer->nodeInterface()) {
    _externalContainer->show();
    _externalContainer->raise();
  }
}

void NodeInterfaceContainer::addItems(InterfaceTitle *title,QWidget *widget) {

  _layout->addWidget(title);
  _layout->addWidget(widget);

  _widgets.push_back(widget);
  _titles.push_back(title);

  setCurrentIndex(count()-1);
}

void NodeInterfaceContainer::removeItems(InterfaceTitle *title,QWidget *widget) {
  int index = title->index();

  _widgets[index] = NULL;
  _titles[index] = NULL;

  _widgets.erase(_widgets.begin()+index);
  _titles.erase(_titles.begin()+index);

  _layout->removeWidget(widget);
  _layout->removeWidget(title);

  widget->hide();
  title->hide();

  for(int i=index;i<count();++i) {
    _titles[i]->setIndex(_titles[i]->index()-1);
  }

  if(_currentIndex>=count()) {
    _currentIndex = count()-1;
  }

  setCurrentIndex(_currentIndex);
}
