#ifndef FILTRE_H
#define FILTRE_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <limits>

#include <QImage>
#include <QPainter>
#include <tgmath.h>
#include <cmath>

#include "pointh.h"
#include "points.h"
#include "noeud.h"
#include "histogramme.h"

using namespace std;

class Filtre
{
public:
    Filtre();
    ~Filtre();
    /*Filtres*/
    QImage* gaussien(QImage* img, int k);
    QImage* adaptatif(QImage* img, int k);
    QImage* median(QImage* img, int k);
    QImage* gradient(QImage* img, int k);
    QImage* gradientX(QImage* img, int k);
    QImage* gradientY(QImage* img, int k);

    /*Harris*/
    QImage* harris(QImage* img,float alpha);
    QImage* pointsInteret(QImage* img,float alpha,int n);
    QImage* correspondance(QImage* img1, QImage* img2,int nbPoints);
    PointH** pointsDInteret(QImage* img,QImage* H,int n);
    PointS** similariteSSD(QImage* img1, PointH** tabH1,int n1,QImage* img2, PointH** tabH2,int n2);
    PointS** similariteZNCC(QImage* img1, PointH** tabH1,int n1,QImage* img2, PointH** tabH2,int n2);
    PointS* calcSimZNCC(QImage* img1,QImage* img2,PointH* p,PointH** tabPH2,int n1,int n2);
    PointS* calcSimSSD(QImage* img1,QImage* img2,PointH* p,PointH** tabPH2,int n2);
    float calcSigma(QImage* img,PointH* p,int pas);

    /*Segmentation*/
    QImage* kMeans(QImage* img,int K,int n,int coul);
    QImage* kMeansRouge(QImage* img,int K,int n);
    QImage* kMeansVert(QImage* img,int K,int n);
    QImage* kMeansBleu(QImage* img,int K,int n);
    QImage* kMeansRGB(QImage* img,int K,int n);

    /*Histogramme*/
    QImage* seuiller(QImage* img,int seuil);
    QImage* negatif(QImage* img);

    /*Fonctions additionnelles*/
    QImage* appliquer(float** mat,QImage* img,int h,int k);
    int med(int* tab,int k);

    /***/
    QImage* binariser(QImage* img);
    int inserer(PointH** tab,PointH* p,int l,int n);
    QImage* afficherPoints(QImage* img, PointS** tab,int n);
    QImage* afficherPoints(QImage* img, PointH** tab,int n);
    int distEucl(QRgb pix1,QRgb pix2);


private:

};

#endif // FILTRE_H
