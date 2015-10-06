#include <stdlib.h>
#include <stdio.h>
#include "Util.h"



int main(int argc, char* argv[])
    {
    FILE* ifp;
    bit* bitmap;
    int ich1, ich2, rows, cols ;
    int i,j ;


    /* Test des arguments */
    if ( argc != 2 ){
      printf("\nUsage : pbmtopgm file \n\n");
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
    if(ich2 != '1')
      pm_erreur(" wrong ifp format ");

    /* Lecture des dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );

    /* Allocation memoire  */
    bitmap = (bit *) malloc(cols * rows * sizeof(bit));

    /* Lecture */
    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
				bitmap[i * cols + j] = pm_getbit(ifp);

    /* Ecriture */
   printf("P2\n");
   printf("%d %d \n", cols, rows);
   printf("1\n");
   for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
	printf("%u ",bitmap[i * cols + j] );



      /* fermeture */
      fclose(ifp);
}
