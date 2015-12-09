#ifndef POINTH_H
#define POINTH_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <QImage>

class PointH
{
public:
    PointH();
    PointH(int x,int y,QRgb pix);
    ~PointH();
    QRgb pix;
    int x;
    int y;
};

#endif // POINTH_H
