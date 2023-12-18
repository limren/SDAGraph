#pragma once
#include <stdio.h>
#include <stdlib.h>

#define NB_VOISINS 3

#define NB_ADJA 1024

typedef struct GraphNode
{
    int numCentroid;
    // Distance par rapport au premier centroïde
    int distance;
    struct GraphNode **nexts;
    int nbNexts;
} GraphNode;

typedef struct
{
    int nbGraphNodes;
    int capacity;
    GraphNode **listAdjacents;
} Graph;

Graph *initGraph();
void addGraphNode(Graph *g, int numC, int distance);
GraphNode *createGraphNode(int numC, int distance);
void addArcGraph(Graph *g, int numC1, int numC2);
int parcoursLargeur(Graph * graphe);