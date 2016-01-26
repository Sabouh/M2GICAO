#include <QApplication>
#include <QImage>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#include "viewer.h"
#include "imgFilter.h"
#include "mlsRec.h"

using namespace std;

float noise = 0.5f;
float sigma = 1.0f;
//QString filename ("test.jpg");
QString filename ("cute1.jpg");

void loadParams(int argc, char *argv[]) {
  // sigma 
  if(argc>=4) {
    QString n(argv[3]);
    sigma = n.toFloat();
  }
  
  // noise 
  if(argc>=3) {
    QString n(argv[2]);
    noise = n.toFloat();
  }
  
  // filename 
  if(argc>=2) {
    filename = QString(argv[1]);
  }
}

int main(int argc, char *argv[]) {
  // usage: ./imageMLS filename noise sigma
  // filename = path to image (png/jpg/...)
  // noise = value in [0,1] that define the number of missing data 
  // sigma = value used for the gaussian weight in the MLS reconstruction

  QApplication app(argc, argv);
  srand(time(NULL));

  loadParams(argc,argv);

  cout << "Input: " << filename.toStdString() << " (" << (int)(noise*100.0f) << " % noise) " << endl;

  // load original image 
  QImage origImg(filename);

  // noisy image 
  QImage noisyImg(NoiseFilter::applyWhiteNoise(origImg,noise));

  // squared missing data
  //QImage noisyImg(SquareFilter::applySquare(origImg,(int)(noise*50.0f)));

  // reconstruct using MLS
  QImage recImg(MLSRec::apply(noisyImg,sigma));

  // display all 
  Viewer viewer1(origImg,"Original Img");
  viewer1.show();

  Viewer viewer2(noisyImg,"Filtered Img");
  viewer2.show();

  Viewer viewer3(recImg,"Reconstructed Img");
  viewer3.show();
  
  return app.exec();
}
