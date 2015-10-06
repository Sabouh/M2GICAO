#include "matrice.h"

#include <stdlib.h>
#include <stdio.h>
#include <QImage>

QImage im;

Matrice::Matrice(FILE *ifp){
    int ich1, ich2, rows, cols ;
    int i,j,maxval;
    gray* resmap;
    /* Test des arguments */
    if ( argc != 2 ){
        printf("\nUsage : gaussToPgm file \n\n");
        exit(0);
    }

    /* Ouverture */
    ifp = fopen(argv[1],"r");

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


   /*Creation de la QImage*/
    QImage img(cols,rows);


      /*lecture*/
    for(i=0; i < rows; i++){
        for(j=0; j < cols ; j++){
            img.setPixel(i,j,  pm_getrawbyte(ifp));
        }
    }
    fclose(ifp);
}

Matrice::convolution(){

}

Matrice::histogramme(){

}


