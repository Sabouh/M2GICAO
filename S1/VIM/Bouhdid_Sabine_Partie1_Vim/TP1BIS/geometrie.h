#ifndef GEOMETRIE_H
#define GEOMETRIE_H

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
#include "filtre.h"

using namespace std;
class Geometrie
{
public:
    Geometrie();
    ~Geometrie();

    /*Morpho*/
    QImage* etiqueter(QImage* img,int connectivite);
    QImage* compterElements(QImage* img, QImage* img1,QImage* img2, QImage* img3,int s1,int s2,int s3);

    /*Union-find*/

    void MakeSet(Noeud* x);
    Noeud* Find(Noeud* x);
    void Union(Noeud* x, Noeud* y);

    /*fonctions additionnelles**/
    bool regarderVoisins(QImage* img,int x, int y, int pas ,int connectivite);
    QPoint minVoisin(int **etiquettes,int x ,int y,int pas, int connectivite);
    int** etiqueterVoisins(int **etiquettes,int x,int y,int pas,int etiqV,int connectivite, vector< Noeud > *etiqRef);
};

#endif // GEOMETRIE_H
