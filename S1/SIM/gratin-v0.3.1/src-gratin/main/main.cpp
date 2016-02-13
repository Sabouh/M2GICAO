// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <qapplication.h>
#include <QString>
#include <QDir>
#include <stdio.h>
#include <stdlib.h>
#include "misc/extinclude.h"
#include "view/mainWindow.h"

using namespace std;

QString inFilename;

void getParams(int argc,char **argv) {

  for(int i=1;i<argc;i++) {    
    if(strcmp(argv[i],"-in")==0 && i<argc-1) {
      inFilename = QString(argv[i+1]);
    } 
  }
}

void createConfigDirectory() {
  QDir dir(GRATIN_CONFIG_DIR);
  if(!dir.exists()) {
    dir.mkpath(".");
  }
}

int main(int argc,char** argv) {
  QApplication application(argc,argv);
  
  QFile file(":/style");
  file.open(QFile::ReadOnly);
  QString style = QLatin1String(file.readAll());

  application.setStyleSheet(style);
  application.setWindowIcon(QIcon(":/gratin-logo"));
 
  getParams(argc,argv);
  createConfigDirectory();
  MainWindow window;

  window.setWindowTitle(GRATIN_NAME);
  window.show();

  if(!inFilename.isEmpty())
    window.loadFile(inFilename);

  return application.exec();
}

