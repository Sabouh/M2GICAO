/* Entete des utilitaires de PBM+ */
/* E.B.  12/98                   */

typedef unsigned char bit;
typedef unsigned char gray;
typedef unsigned char color;

char pm_getc(FILE* file);
bit pm_getbit(FILE* file);
unsigned char pm_getrawbyte(FILE* file);
int pm_getint( FILE* file);

void pm_erreur(char *);
