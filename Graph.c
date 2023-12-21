#include "Graph.h"

Graph *initGraph()
{
    Graph *g = malloc(sizeof(GraphNode));
    g->nbGraphNodes = 0;
    g->capacity = NB_ADJA;
    g->listAdjacents = malloc(sizeof(GraphNode *) * NB_ADJA);
    return g;
}

GraphNode *createGraphNode(int numC, int distance)
{
    GraphNode *node = malloc(sizeof(GraphNode));
    node->numCentroid = numC;
    node->nbNexts = 0;
    node->nexts = malloc(sizeof(GraphNode *) *  NB_VOISINS);
    node->distance = distance;
    return node;
}

// Ajout d'un centroïde dans la liste d'adjacence
void addGraphNode(Graph *g, int numC, int distance)
{
    GraphNode *node = createGraphNode(numC, distance);
    g->listAdjacents[numC] = node;
    g->nbGraphNodes++;
}


// Important à noter que j'ai préféré jouer avec les indices des centroïdes plutôt que de faire des boucles à chaque fois pour trouver le centroïde
void addArcGraph(Graph *g, int numC1, int numC2)
{
    if(numC1 >= g->capacity)
    {
        while(numC1 >= g->capacity)
        {
            g->capacity *= 2;
        }
        g->listAdjacents = realloc(g->listAdjacents, sizeof(GraphNode *) * (g->capacity));
    }
    if(numC2 >= g->capacity)
    {
        while(numC2 >= g->capacity)
        {
            g->capacity *= 2;
        }
        g->listAdjacents = realloc(g->listAdjacents, sizeof(GraphNode *) * (g->capacity));
    }
    GraphNode * n = g->listAdjacents[numC1];
    
    if(n == NULL)
    {
        addGraphNode(g, numC1, 0);
        n = g->listAdjacents[numC1];
    }

    GraphNode *nc2 = g->listAdjacents[numC2];
    if (nc2 == NULL)
    {
        addGraphNode(g, numC2, 0);
        nc2 = g->listAdjacents[numC2];
    }
    n->nexts[n->nbNexts++] = nc2;
}


int parcoursLargeur(Graph *graphe) {
    int *marque = malloc(sizeof(int) * graphe->nbGraphNodes);
    int *distance = malloc(sizeof(int) * graphe->nbGraphNodes);
    int largestDistance = 0;
    for (int i = 0; i < graphe->nbGraphNodes; i++) {
        marque[i] = 0;
        distance[i] = -1; // Initialisez toutes les distances à -1 (non atteintes)
    }

    GraphNode **file = malloc(sizeof(GraphNode *) * graphe->nbGraphNodes);
    int tete = 0;
    int queue = 0;

    file[queue++] = graphe->listAdjacents[0];
    marque[0] = 1;
    distance[0] = 0;

    while (tete != queue) {
        GraphNode *sommet = file[tete++];

        for (int i = 0; i < sommet->nbNexts; i++) {
            GraphNode *voisin = sommet->nexts[i];
            if (marque[voisin->numCentroid] == 0) {
                marque[voisin->numCentroid] = 1;
                distance[voisin->numCentroid] = distance[sommet->numCentroid] + 1;
                voisin->distance = distance[voisin->numCentroid];
                if(voisin->distance > largestDistance)
                {
                    largestDistance = voisin->distance;
                }
                file[queue++] = voisin;
            }
        }
    }

    printf("largest distance : %d \n", largestDistance);
    free(marque);
    free(distance);
    free(file);

    return largestDistance;
}
