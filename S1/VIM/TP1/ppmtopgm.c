#include <stdlib.h>
#include <stdio.h>
#include "Util.h"



int main(int argc, char* argv[])
    {
    FILE* ifp;
    color* redmap;
    color* greenmap;
    color* bluemap;
    int ich1, ich2, rows, cols ;
    int i,j, maxval;


    /* Test des arguments */
    if ( argc != 2 ){
      printf("\nUsage : ppmtopgm file \n\n");
      exit(0);
    }

    /* Ouverture */
    ifp = fopen(argv[1],"r");

    /* Lecture du Magic number */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error reading magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF / read error reading magic number" );
    if(ich2 != '6')
      pm_erreur(" wrong ifp format ");

    /* Lecture des dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
		maxval = pm_getint(ifp);

    /* Allocation memoire  */
    redmap = (color *) malloc(cols * rows * sizeof(color));
    greenmap = (color *) malloc(cols * rows * sizeof(color));
    bluemap = (color *) malloc(cols * rows * sizeof(color));

    /* Lecture */
 
    for(i=0; i < rows; i++){
      for(j=0; j < cols ; j++){
			  redmap[i*cols+j] =  pm_getrawbyte(ifp);
			  greenmap[i*cols+j] =  pm_getrawbyte(ifp);
			  bluemap[i*cols+j] =  pm_getrawbyte(ifp);
	  }
	}
    /* Ecriture */
   printf("P2\n");
   printf("%d %d \n", cols, rows);
   printf("%d \n",maxval);
   for(i=0; i < rows; i++){
      for(j=0; j < cols ; j++){
      	gray gris = (unsigned char) (0.2126*redmap[i * cols + j]+0.7152*greenmap[i * cols + j]+0.0722*bluemap[i * cols + j]);
				printf("%u ",gris );
			}
	}



      /* fermeture */
      fclose(ifp);
}

