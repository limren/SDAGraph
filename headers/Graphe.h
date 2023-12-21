#pragma once
#include <stdio.h>
#include <stdlib.h>

#define NB_VOISINS 3

#define NB_ADJA 1024

typedef struct NoeudGraphe
{
    int numCentroid;
    // Distance par rapport au premier centroïde
    int distance;
    struct NoeudGraphe **nexts;
    int nbNexts;
} NoeudGraphe;

typedef struct
{
    int nbGraphNodes;
    int capacity;
    NoeudGraphe **listAdjacents;
} Graphe;

Graphe *grapheVide();
void ajoutNoeudGraphe(Graphe *g, int numC, int distance);
NoeudGraphe *creationNoeudGraphe(int numC, int distance);
void ajoutArcGraph(Graphe *g, int numC1, int numC2);
int parcoursLargeur(Graphe * graphe);