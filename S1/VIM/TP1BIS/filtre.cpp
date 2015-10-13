#include "filtre.h"

Filtre::Filtre()
{

}

Filtre::~Filtre()
{

}

QImage* Filtre::gaussien(QImage* img,int k){

    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = 1.0;
        mat[0][1] = 2.0;
        mat[0][2] = 1.0;
        mat[1][0] = 2.0;
        mat[1][1] = 4.0;
        mat[1][2] = 2.0;
        mat[2][0] = 1.0;
        mat[2][1] = 2.0;
        mat[2][2] = 1.0;

    }
    return appliquer(mat,copie,(1/16),k);

}

QImage* Filtre::adaptatif(QImage* img,int k){
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = 1.0;
        mat[0][1] = 2.0;
        mat[0][2] = 1.0;
        mat[1][0] = 2.0;
        mat[1][1] = 4.0;
        mat[1][2] = 2.0;
        mat[2][0] = 1.0;
        mat[2][1] = 2.0;
        mat[2][2] = 1.0;

    }
    return appliquer(mat,copie,(1/16),k);
}

QImage* Filtre::median(QImage* img,int k){
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
   /*CAS PARTICULIER*/
}

QImage* Filtre::gradientX(QImage* img,int k){
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = -1.0;
        mat[0][1] = -2.0;
        mat[0][2] = -1.0;
        mat[1][0] = 0.0;
        mat[1][1] = 0.0;
        mat[1][2] = 0.0;
        mat[2][0] = 1.0;
        mat[2][1] = 2.0;
        mat[2][2] = 1.0;

    }
    return appliquer(mat,copie,0.25,k);

}

QImage* Filtre::gradientY(QImage* img,int k){
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = -1.0;
        mat[0][1] = 0.0;
        mat[0][2] = 1.0;
        mat[1][0] = -2.0;
        mat[1][1] = 0.0;
        mat[1][2] = 2.0;
        mat[2][0] = -1.0;
        mat[2][1] = 0.0;
        mat[2][2] = 1.0;

    }
    return appliquer(mat,copie,0.25,k);

}

QImage* Filtre::appliquer(float** noyau, QImage* img,float h,int k){
    /*********************/
    int pas = k/2;
    int npixR;
    int npixB;
    int npixG;
    QRgb pix;
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    for(int i=pas;i<img->height()-pas;i++){
        for(int j=pas;j<img->width()-pas;i++){
            for(int k1=-pas;k1<pas;k1++){
                for(int k2=-pas;k2<pas;k2++){
                    pix = img->pixel(i+k1,j+k2);
                    npixR +=qRed(pix) ;
                    npixB +=qBlue(pix);
                    npixG +=qGreen(pix);
                }
            }
            npixR = npixR/h;
            npixB = npixB/h;
            npixG = npixG/h;
            copie->setPixel(i,j,qRgb(npixR,npixG,npixB));
        }
    }
    return copie;

}

