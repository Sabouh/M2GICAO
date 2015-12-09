#ifndef NOEUD_H
#define NOEUD_H


#include <iostream>
#include <stdlib.h>
#include <stdio.h>

class Noeud
{
public:
    Noeud(int etiq);
    Noeud();

    Noeud* id;
    int etiq;
    int rank;
};

#endif // NOEUD_H
