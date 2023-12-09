#include "Graph.h"

Graph *initGraph()
{
    Graph *g = malloc(sizeof(GraphNode));
    g->nbGraphNodes = 0;
    g->listAdjacents = malloc(sizeof(GraphNode *) * NB_ADJA);
    return g;
}

GraphNode *createGraphNode(int numC, int distance)
{
    GraphNode *node = malloc(sizeof(GraphNode));
    node->numCentroid = numC;
    node->nbNexts = 0;
    node->nexts = malloc(sizeof(GraphNode *) * NB_VOISINS);
    node->distance = distance;
    return node;
}

// Ajout d'un centroïde dans la liste d'adjacence
void addGraphNode(Graph *g, int numC, int distance)
{
    GraphNode *node = createGraphNode(numC, distance);
    if (g->nbGraphNodes % NB_ADJA == 0)
    {

        g->listAdjacents = realloc(g->listAdjacents, g->nbGraphNodes + NB_ADJA);
    }
    g->listAdjacents[g->nbGraphNodes++] = node;
}

void addArcGraph(Graph *g, int numC1, int numC2)
{
    GraphNode *nc2 = NULL;
    for (int i = 0; i < g->nbGraphNodes; i++)
    {
        if (g->listAdjacents[i]->numCentroid == numC2)
        {
            nc2 = g->listAdjacents[i];
        }
    }
    if (nc2 == NULL)
    {
        printf("erreur le sang");
    }
    for (int i = 0; i < g->nbGraphNodes; i++)
    {
        GraphNode *n = g->listAdjacents[i];
        if (n->numCentroid == numC1)
        {

            if (n->nbNexts == 2)
            {
                printf("erreur ici");
            }
            n->nexts[n->nbNexts++] = nc2;
        }
    }
}
