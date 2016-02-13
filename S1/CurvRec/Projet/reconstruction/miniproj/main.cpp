#include "viewer.h"
#include <qapplication.h>
#include <iostream>

using namespace std;

QString getFilename(int argc, char** argv) {
  if(argc<=1) {
    cout << "Usage: " << argv[0] << " filename" << endl;
    exit(0);
  }

  return QString(argv[1]);
}

int main(int argc, char** argv) {
  // Read command lines arguments.
  QApplication application(argc,argv);
  
  QString filename = getFilename(argc,argv);

  // Instantiate the viewer.
  Viewer viewer(filename);

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("Reconstruction");
#endif

  // Make the viewer window visible on screen.
  viewer.show();

  // Run main loop.
  return application.exec();
}

