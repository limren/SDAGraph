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
    node->nexts = malloc(sizeof(GraphNode *) * NB_VOISINS);
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
            g->capacity += NB_ADJA;
        }
        g->listAdjacents = realloc(g->listAdjacents,sizeof(GraphNode *) * (g->capacity + NB_ADJA));
    }
    if(numC2 >= g->capacity)
    {
        while(numC2 >= g->capacity)
        {
            g->capacity += NB_ADJA;
        }
        g->listAdjacents = realloc(g->listAdjacents,sizeof(GraphNode *) * (g->capacity + NB_ADJA));
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


void parcoursLargeur(Graph * graphe){
    printf("parcours largeur \n");
    int * marque = malloc(sizeof(int) * graphe->nbGraphNodes);
    for(int i = 0; i<graphe->nbGraphNodes; i++){
        marque[i] = 0;
    }
    GraphNode ** file = malloc(sizeof(GraphNode*) * graphe->nbGraphNodes);
    int tete = 0;
    int queue = 0;
    file[queue++] = graphe->listAdjacents[0];
    marque[0] = 1;
    while(tete != queue){
        GraphNode * sommet = file[tete++];
        printf("sommet : %d \n", sommet->numCentroid);
        for(int i = 0; i<graphe->listAdjacents[sommet->numCentroid]->nbNexts; i++){
            GraphNode * voisin = graphe->listAdjacents[sommet->numCentroid]->nexts[i];
            if(marque[voisin->numCentroid] == 0){
                marque[voisin->numCentroid] = 1;
                file[queue++] = voisin;
            }
        }
    }
}