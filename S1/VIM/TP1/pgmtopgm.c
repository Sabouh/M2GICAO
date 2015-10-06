#include <stdlib.h>
#include <stdio.h>
#include "Util.h"



int main(int argc, char* argv[])
    {
    FILE* ifp;
    gray* graymap;
    int ich1, ich2, rows, cols, maxval, pgmraw ;
    int i,j;



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
    if(ich2 != '2' && ich2 != '5')
      pm_erreur(" wrong ifp format ");
    else
      if(ich2 == '5')
	pgmraw = 1;
      else pm_erreur(" wrong format ");
    
    /* Lecture des dimensions */

	if(pgmraw){ //si P5->P2
		cols = pm_getint(ifp);
		rows = pm_getint(ifp);
		maxval = pm_getint(ifp);
		if(maxval > 255){
			pm_erreur("maxvalue >255 non traité");
		}
	}else{
	}
    /*
     *  A COMPLETER
     */

    /* Allocation memoire  */
    graymap = (gray *) malloc(cols * rows * sizeof(gray));

    /* Lecture */
    
	if(pgmraw){  //si P5 -> P2
		for(i=0;i<rows;i++){
			for(j=0;j<cols;j++){
		  		graymap[i*cols+j] = pm_getrawbyte(ifp) ;
			}
		}
	
	}else{
	}
	

    /*
     *  A COMPLETER
     */


    /* Ecriture */
    if(pgmraw)
      {
	printf("P2\n");
	printf("%d %d \n", cols, rows);
	printf("%d\n",maxval);
      }
      
      
	for(i=0; i < rows; i++){
		for(j=0; j < cols ; j++){
			printf("%u ",graymap[i * cols + j] );
		}
		printf("\n");
	}
   	
	

    /*
     *  A COMPLETER
     */


      /* fermeture */
      fclose(ifp);
}
