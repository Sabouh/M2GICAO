// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/helpWidget.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

HelpWidget *HelpWidget::_instance = NULL;

HelpWidget::HelpWidget(QWidget *parent) 
  : QDialog(parent) { 
  _list  = new QListWidget();
  _help  = new QLabel();
  _title = new QLabel("<font color='black' size='12'> Help </font>");
  _title->setAlignment(Qt::AlignHCenter | Qt::AlignTop);


  QHBoxLayout *l1 = new QHBoxLayout();
  QVBoxLayout *l2 = new QVBoxLayout();

  QScrollArea *lscrollbar = new QScrollArea();
  QScrollArea *hscrollbar = new QScrollArea();
  lscrollbar->setWidgetResizable(true);
  hscrollbar->setWidgetResizable(true);

  lscrollbar->setWidget(_list);
  hscrollbar->setWidget(_help);

  QWidget *w1  = new QWidget();
  QWidget *w2  = new QWidget();
  QLayout *wl1 = new QVBoxLayout();
  QLayout *wl2 = new QVBoxLayout();
  wl1->addWidget(lscrollbar);
  wl2->addWidget(hscrollbar);
  w1->setLayout(wl1);
  w2->setLayout(wl2);

  l1->addWidget(w1);
  l1->addWidget(w2);

  l2->addWidget(_title);
  l2->addItem(l1);

  setLayout(l2);

  setWindowModality(Qt::NonModal);
  connect(_list,SIGNAL(currentRowChanged(int)),this,SLOT(itemChanged(int)));

  resize(800,600);
  w1->setMaximumWidth(width()/4);
}

HelpWidget::~HelpWidget() {
  _names.clear();
  _helps.clear();
}

void HelpWidget::add(const QString &name,const QString &help) {
  _names.push_back(name);
  _helps.push_back(help);
  _list->addItem(name);
}

void HelpWidget::itemChanged(int index) {
  const QString title = "<font color='black' size='12'>"+_list->currentItem()->text()+"</font>";
  _title->setText(title);
  _help->setText(_helps[index]);
}
