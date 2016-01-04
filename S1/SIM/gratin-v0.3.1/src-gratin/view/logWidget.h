// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include "misc/extinclude.h"
#include <QAction>

class LIBRARY_EXPORT LogWidget : public QAction {
  Q_OBJECT

public:
  ~LogWidget();
  static LogWidget *instance() {
    if(!_instance) {
      _instance = new LogWidget();
    }
    return _instance;
  }

private:
  LogWidget();
  static LogWidget *_instance;
};


#endif // LOGWIDGET_H
