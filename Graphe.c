#include "headers/Graphe.h"

Graphe *grapheVide()
{
    Graphe *g = malloc(sizeof(NoeudGraphe));
    g->nbGraphNodes = 0;
    g->capacity = NB_ADJA;
    g->listAdjacents = malloc(sizeof(NoeudGraphe *) * NB_ADJA);
    return g;
}

NoeudGraphe *creationNoeudGraphe(int numC, int distance)
{
    NoeudGraphe *node = malloc(sizeof(NoeudGraphe));
    node->numCentroid = numC;
    node->nbNexts = 0;
    node->nexts = malloc(sizeof(NoeudGraphe *) *  NB_VOISINS);
    node->distance = distance;
    return node;
}

// Ajout d'un centroïde dans la liste d'adjacence
void ajoutNoeudGraphe(Graphe *g, int numC, int distance)
{
    NoeudGraphe *node = creationNoeudGraphe(numC, distance);
    g->listAdjacents[numC] = node;
    g->nbGraphNodes++;
}

// Important à noter que l'on a préféré jouer avec les indices des centroïdes plutôt que de faire des boucles à chaque fois pour trouver le centroïde
void ajoutArcGraph(Graphe *g, int numC1, int numC2)
{
    // Ajustement des capacités supposées, ici on peut conclure que si le numéro du centroïde est supérieur à la capacité, alors on double la capacité jusqu'à ce que la capacité soit environ égale au numéro de centroïde.
    if(numC1 >= g->capacity)
    {
        while(numC1 >= g->capacity)
        {
            g->capacity *= 2;
        }
        g->listAdjacents = realloc(g->listAdjacents, sizeof(NoeudGraphe *) * (g->capacity));
    }
    if(numC2 >= g->capacity)
    {
        while(numC2 >= g->capacity)
        {
            g->capacity *= 2;
        }
        g->listAdjacents = realloc(g->listAdjacents, sizeof(NoeudGraphe *) * (g->capacity));
    }
    NoeudGraphe * n = g->listAdjacents[numC1];
    
    if(n == NULL)
    {
        ajoutNoeudGraphe(g, numC1, 0);
        n = g->listAdjacents[numC1];
    }

    NoeudGraphe *nc2 = g->listAdjacents[numC2];
    if (nc2 == NULL)
    {
        ajoutNoeudGraphe(g, numC2, 0);
        nc2 = g->listAdjacents[numC2];
    }
    n->nexts[n->nbNexts++] = nc2;
}

/*
    parcoursLargeur :
        - Nous avons décidé d'effectuer un parcours en largeur pour colorier le graphe, car cela nous permet de calculer 
        la distance de chaque arête par rapport à l'origine. De ce fait, déduire la couleur avec une produit en croix depuis
        l'arête la plus lointaine de l'origine. D'où le calcule de la distance la plus grande.
*/
int parcoursLargeur(Graphe *graphe) {
    // Table de marquage des arêtes où l'on est déjà passé dans le graphe.
    int *marque = malloc(sizeof(int) * graphe->nbGraphNodes);
    // Table de distances, servant à calculer la distance des arêtes depuis l'origine (choisi arbitrairement, ici on prend l'arête se trouvant à l'indice 0).
    int *distance = malloc(sizeof(int) * graphe->nbGraphNodes);
    int distanceLaPlusGrande = 0;
    for (int i = 0; i < graphe->nbGraphNodes; i++) {
        marque[i] = 0;
        distance[i] = -1; // Initialisez toutes les distances à -1 (non atteintes)
    }
    /* 
        Nous avons décidé de créer une file "manuellement", pour éviter d'utiliser une bibliothèque externe et/ou de devoir implémenter une structure de file.
        Le concept reste bien évidemment le même. 
    */
   NoeudGraphe **file = malloc(sizeof(NoeudGraphe *) * graphe->nbGraphNodes);
    int tete = 0;
    int queue = 0;

    file[queue++] = graphe->listAdjacents[0];
    marque[0] = 1;
    distance[0] = 0;

    while (tete != queue) {
        NoeudGraphe *sommet = file[tete++];

        for (int i = 0; i < sommet->nbNexts; i++) {
            NoeudGraphe *voisin = sommet->nexts[i];
            /* 
               Si on est jamais passé par cette arrête, on la marque et on calcule sa distance par rapport à 
               l'origine et on l'ajoute à la file afin de
               traiter ses voisins.
            */
            if (marque[voisin->numCentroid] == 0) {
                marque[voisin->numCentroid] = 1;
                distance[voisin->numCentroid] = distance[sommet->numCentroid] + 1;
                voisin->distance = distance[voisin->numCentroid];
                if(voisin->distance > distanceLaPlusGrande)
                {
                    distanceLaPlusGrande = voisin->distance;
                }
                file[queue++] = voisin;
            }
        }
    }
    // Libération de la mémoire des tableaux.
    free(marque);
    free(distance);
    free(file);
    return distanceLaPlusGrande;
}
