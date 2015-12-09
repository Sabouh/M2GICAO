#ifndef MATRICE_H
#define MATRICE_H

#include "Util.h"
#include <stdlib.h>
#include <stdio.h>
#include <QImage>

class Matrice{

public :
    Matrice(char* filename);

    //convolution avec noyau 3x3
    QImage convolution3(int** m,int cols,int rows);
    void histogramme();

private:

};

#endif //MATRICE_H
