// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HELP_WIDGET
#define HELP_WIDGET

#include "misc/extinclude.h"

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QDialog>

class HelpWidget : public QDialog {
 Q_OBJECT

 public:
  // this widget is a singleton
  static HelpWidget *instance(QWidget *parent=NULL) {
    if(!_instance) {
      _instance = new HelpWidget(parent);
    }
    return _instance;
  } 


  ~HelpWidget();

  void add(const QString &name,const QString &help);
  inline bool empty() {return _names.empty();}

  public slots:
  void itemChanged(int);

 private:
  HelpWidget(QWidget *parent=NULL);
  static HelpWidget *_instance;

  std::vector<QString> _names;
  std::vector<QString> _helps;

  QListWidget *_list;
  QLabel      *_title;
  QLabel      *_help;

};

#endif // HELP_WIDGET
