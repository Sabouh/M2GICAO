#include "filtre.h"

Filtre::Filtre()
{

}

Filtre::~Filtre()
{

}

QImage* Filtre::gaussien(QImage* img,int k){

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
    return appliquer(mat,img,16,k);

}

QImage* Filtre::adaptatif(QImage* img,int k){
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    /*CAS PARTICULIER */
}

QImage* Filtre::median(QImage* img){
    int k = 3;
   /*CAS PARTICULIER*/
    QImage *copie = new QImage(img->width(), img->height(), img->format() );
    copie = img->copy(0,0,img->width(),img->height());
    for(int x =0;x<img->width();x++){
        for(int y =0; y<img->height();y++){

        }
    }

}

QImage* Filtre::gradientX(QImage* img,int k){

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
    return appliquer(mat,img,4,k);

}

QImage* Filtre::gradientY(QImage* img,int k){
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
    return appliquer(mat,img,4,k);

}

QImage* Filtre::appliquer(float** noyau, QImage* img,int h,int k){
    /*********************/
    int pas = k/2;
    int npixR;
    int npixB;
    int npixG;
    QRgb pix;
    QImage *copie = new QImage(img->width(), img->height(), img->format() );

    for(int x=pas;x <( img->width()-pas);x++){
        for(int y=pas;y < (img->height()-pas);y++){
            npixR = 0;
            npixB = 0;
            npixG = 0;
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){
                    pix = img->pixel(x+k1,y+k2);
                    npixR +=(qRed(pix)* noyau[pas+k1][pas+k2]);
                    npixB +=(qBlue(pix)* noyau[pas+k1][pas+k2]);
                    npixG +=(qGreen(pix)* noyau[pas+k1][pas+k2]);

                }
            }
             npixR = npixR*1/h;
             npixB = npixB*1/h;
             npixG = npixG*1/h;
             pix = qRgb(npixR,npixG,npixB);
             copie->setPixel(x,y,pix);

        }
    }
    return copie;

}

