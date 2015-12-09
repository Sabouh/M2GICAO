#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main()
{
    //TODO :
    //recuperer donnees de stations.csv et de synop.2015110912.csv et les mettre en relation
    //sous quelle forme ? Vecteur pr chaque parametre
    //avec 1 vecteur pr noms des parametres
    //Se concentrer sur temperatures de pression !!!

    FILE* f;
    f = fopen("../Donnees/stations.csv","r");

    fclose(f);
    return 0;
}

