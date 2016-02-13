#ifndef POINT_H
#define POINT_H

#include "iostream"
#include "math.h"
using namespace std;
class Point
{
public:
    /**/
    Point();
    Point(float x,float y,float z);
    /**/
    float getX() const;
    float getY() const;
    float getZ() const;
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    /**/
    float distance(Point p1,Point p2);
    //calcule la distance entre p1 et p2 en ne prenant pas en compte l'altitude
    float distance2(Point p1,Point p2);
    void afficher();

private:
    float x,y,z;
//signals:

//public slots:
};

#endif // POINT_H
