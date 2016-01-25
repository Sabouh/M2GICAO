#include "point.h"

Point::Point()
{
    x = 0;
    y = 0;
    z = 0;
}

Point::Point(float x,float y,float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}


float Point::getX() const{
    return x;
}

float Point::getY() const{
    return y;
}

float Point::getZ() const{
    return z;
}

void Point::setX(float x){
    this->x = x;
}

void Point::setY(float y){
    this->y = y;
}

void Point::setZ(float z){
    this->z = z;
}

float Point::distance(Point p1,Point p2){
    return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2) +pow(p1.z-p2.z,2));
}

float Point::distance2(Point p1,Point p2){
    return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

void Point::afficher(){
    cout <<"(x: "<<x<<",y: "<<y<<",z: "<<z<<" )"<<endl;
}
