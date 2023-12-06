#include <stdio.h>
#include <stdlib.h>

#define NB_VOISINS 3

#define NB_ADJA 1024

typedef struct Noeud
{
    int numCentroide;
    int distanceInit;
    struct Noeud **suivants;
    int nbSuivants;
} Noeud;

typedef struct
{
    int nombreNoeuds;
    Noeud **listesAdjacence;
} grapheListe;
