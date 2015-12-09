#include "histogramme.h"


Histogramme::Histogramme() {

}

Histogramme::~Histogramme()
{

}

int* Histogramme::getHistoR(){
    return histR;
}

int* Histogramme::getHistoB(){
    return histB;
}

int* Histogramme::getHistoG(){
    return histG;
}

bool Histogramme::val()
{
    int i=0;
    while(i<256 &&(histG[i]==0 && histB[i]==0 && histR[i]==0)){
        i++;
    }
    valmin = i;
    i=255;
    while(i>0 && (histG[i]==0 && histB[i]==0 && histR[i]==0)){
        i--;
    }
    valmax = i;
    cout <<" valmin : "<<valmin<< " valmax : "<<valmax<<" "<<endl;
    return true;
}

QImage* Histogramme::etalement(int vmin, int vmax, QImage* img){

    QImage *copie = new QImage(img->width(), img->height(),img->format());
    QRgb pix;
    double rPix;
    double gPix;
    double bPix;
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            pix = img->pixel(x,y);
            rPix = qRed(pix);
            gPix = qGreen(pix);
            bPix = qBlue(pix);
            rPix = (rPix- valmin) *(vmax-vmin)/(valmax-valmin) +vmin;
            gPix = (gPix - valmin) *(vmax-vmin)/(valmax-valmin)+vmin;
            bPix = (bPix-  valmin) *(vmax-vmin)/(valmax-valmin)+vmin;

            copie->setPixel(x,y,qRgb(rPix,gPix,bPix));
        }
    }

   return copie;
}

QImage* Histogramme::inverser(QImage* img)
{
    int* hR = new int[256];
    int* hG = new int[256];
    int* hB = new int[256];

    for(int i=0;i<256;i++){
        hR[i] = histR[255-i];
        hG[i] = histG[255-i];
        hB[i] = histB[255-i];
    }
    histR = hR;
    histG = hG;
    histB = hB;

    QRgb pix;
    QImage* copie = new QImage(img->width(), img->height(),img->format());
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            pix = img->pixel(x,y);
            copie->setPixel(x,y,qRgb(valmax-qRed(pix),valmax-qGreen(pix),valmax-qBlue(pix)));
        }
    }
    return copie;
}

bool Histogramme::histo(QImage* img)
{
    histR = new int[256];
    histG = new int[256];
    histB = new int[256];

    for(int i =0;i<256;i++){
        histR[i]=0;
        histG[i]=0;
        histB[i]=0;
    }

    QRgb pix;
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            pix = img->pixel(x,y);
            histR[qRed(pix)] ++;
            histG[qGreen(pix)]++;
            histB[qBlue(pix)]++;
        }
    }
    return true;
}

int* Histogramme::intensitePredomR(int k){
    int* couleurs = new int[k];
    int* tmp = new int[256];
    for(int i=0;i<256;i++){
        tmp[i] = histR[i];
    }
    sort(tmp,tmp+256);

    for(int i =0;i<k;i++){
        couleurs[i] = tmp[255-i];
        int j=0;
        while(j<255 && histR[j]!=couleurs[i]){
            j++;
        }
        couleurs[i] = j;
    }
    return couleurs;
}

int* Histogramme::intensitePredomG(int k){
    int* couleurs = new int[k];
    int* tmp = new int[256];
    for(int i=0;i<256;i++){
        tmp[i] = histG[i];
    }
    sort(tmp,tmp+256);

    for(int i =0;i<k;i++){
        couleurs[i] = tmp[255-i];
        int j=0;
        while(j<255 && histG[j]!=couleurs[i]){
            j++;
        }
        couleurs[i] = j;
    }
    return couleurs;
}

int* Histogramme::intensitePredomB(int k){
    int* couleurs = new int[k];
    int* tmp = new int[256];
    for(int i=0;i<256;i++){
        tmp[i] = histB[i];
    }
    sort(tmp,tmp+256);

    for(int i =0;i<k;i++){
        couleurs[i] = tmp[255-i];
        int j=0;
        while(j<255 && histB[j]!=couleurs[i]){
            j++;
        }
        couleurs[i] = j;
    }
    return couleurs;
}

QRgb* Histogramme::intensitePredomRGB(int k){
    int* rouge = intensitePredomR(k);
    int* vert = intensitePredomG(k);
    int* bleu = intensitePredomB(k);
    QRgb* rgb = new QRgb[k];
    for(int i=0;i<k;i++){
        rgb[i] = qRgb(rouge[i],vert[i],bleu[i]);
    }
    return rgb;
}

bool Histogramme::show(){
    HistoDialog *fen = new HistoDialog();
    fen->histB = this->histB;
    fen->histG = this->histG;
    fen->histR = this->histR;
    fen->show();
    return true;

}
