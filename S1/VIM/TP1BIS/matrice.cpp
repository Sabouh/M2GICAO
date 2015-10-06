#include "matrice.h"
#include "Util.h"


QImage im;

Matrice::Matrice(char* filename){
    int ich1, ich2, rows, cols ;
    int i,j,maxval;
    gray* graymap;
    FILE* ifp;


    /* Ouverture */
    ifp = fopen(filename,"r");

    /* Lecture du Magic number */
    ich1 = getc(ifp);
    if(ich1 == EOF)
        pm_erreur("EOF / read error reading magic number");
    ich2 = getc(ifp);
    if(ich2 == EOF)
        pm_erreur("EOF / read error reading magic number");
    if(ich2 != '2')
        pm_erreur(" wrong ifp format ");

    /* Lecture des dimensions */
    cols = pm_getint(ifp);
    rows = pm_getint(ifp);
    maxval = pm_getint(ifp);


    /**/
    graymap = (gray *) malloc(cols * rows * sizeof(gray));

    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++){
            graymap[i*cols+j] = pm_getrawbyte(ifp) ;
        }
       }

   /*Creation de la QImage*/
    //QImage img(cols,rows);


      /*lecture*/
    for(i=0; i < rows; i++){
        for(j=0; j < cols ; j++){
            //img.setPixel(i,j,  pm_getrawbyte(ifp));
        }
    }

    QImage img(graymap,cols,rows,QImage::Format_Indexed8);
    fclose(ifp);

}
/*On suppose que m est de taille 3x3*/
QImage Matrice::convolution3(int** m,int cols,int rows){

    int i,j;
    int val;
    QImage res(rows-2,cols,QImage::Format_Indexed8);
    for(i=1;i<rows-1;i++){//on zappe les bords
        for(j=1;j<cols-1;j++){
            val = im.pixel(i-1,j-1)*m[i-1][j-1] + im.pixel(i-1,j)*m[i-1][j] + im.pixel(i-1,j+1)*m[i-1][j+1]
                 +im.pixel(i,j-1)*m[i][j-1] + im.pixel(i,j)*m[i][j] + im.pixel(i,j+1)*m[i][j+1]
                 +im.pixel(i+1,j-1)*m[i+1][j-1] + im.pixel(i+1,j)*m[i+1][j] + im.pixel(i+1,j+1)*m[i+1][j+1];
            res.setPixel(i,j,val);
        }
    }

    return res;
}


/*pour une matrice de taille qcque*/
QImage Matrice::adaptatif(int** m,int cols,int rows){

    int i,j;
    int val=0;
    int d;
    int k,l;

    QImage res(rows-2,cols-2,QImage::Format_Indexed8);
    for(i=1;i<rows-1;i++){//on zappe les bords
        for(j=1;j<cols-1;j++){
            for(k=-(matrows/2);k<(matrows/2);k++){
                for(k=-(matcols/2);k<(matcols/2);k++){
                    val+= im.pixel(i+k,j+l)*m[i+k,j+l];
                }
            }
            res.setPixel(i-1,j-1,val/(matrows*matcols));//a verifier
        }
    }
    return res;
}


void Matrice::histogramme(){

}


