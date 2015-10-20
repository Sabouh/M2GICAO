#ifndef FILTRE_H
#define FILTRE_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <QImage>
using namespace std;

class Filtre
{
public:
    Filtre();
    ~Filtre();
    QImage* gaussien(QImage* img, int k);
    QImage* adaptatif(QImage* img, int k);
    QImage* median(QImage* img, int k);
    QImage* gradientX(QImage* img, int k);
    QImage* gradientY(QImage* img, int k);
    QImage* appliquer(float** mat,QImage* img,int h,int k);

private:

};

#endif // FILTRE_H
