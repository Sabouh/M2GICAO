#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "filtre.h"
#include "histodialog.h"
#include <QImage>
using namespace std;

class Histogramme
{
public:
    Histogramme();
    ~Histogramme();

    /*fonctions*/
    bool val();
    QImage* inverser(QImage* img);
    bool histo(QImage* img);
    bool show();
    QImage* etalement(int vmin, int vmax, QImage* img);

    /**/
    int* getHistoR();
    int* getHistoG();
    int* getHistoB();
    int* intensitePredomR(int k);
    int* intensitePredomG(int k);
    int* intensitePredomB(int k);
    QRgb* intensitePredomRGB(int k);


private:
    int* histR;
    int* histG;
    int* histB;
    int valmin;
    int valmax;

};


#endif // HISTOGRAMME_H
