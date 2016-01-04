// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <QtGui/QWindow>
#include "misc/extinclude.h"

class GlWindow : public QWindow, public GlContext {
    Q_OBJECT
 public:
    GlWindow(QScreen *screen=0): QWindow(screen),_update(false) {} 
    ~GlWindow() {};
    
    virtual void render() {};
    
 public slots:
   inline void renderNow() { render(); }
   inline void renderLater() {
     if(!_update) {
       _update = true;
       QCoreApplication::postEvent(this,new QEvent(QEvent::UpdateRequest));
     }
   }
   
 protected:
   bool event(QEvent *event) {     
     switch(event->type()) {
     case QEvent::UpdateRequest:
       _update = false;
       renderNow();
       return true;
     default:
       return QWindow::event(event);
     }
   }
   
   void exposeEvent(QExposeEvent *) { if(isExposed()) renderNow(); }
   
 private:
   bool _update;
};

#endif // GL_WINDOW_H
