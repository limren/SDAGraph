#pragma once
#include "const.h"
#include "structs.h"
#include "Graphe.h"
Vertex *sommetVide();
Face *faceVide();
Maillage *maillageVide();
GrapheDuale *GDualeVide();
AreteDuale *emptyArete();
int sontEquilaventes(Arete *a, Arete *b);
SelectAretes *SAVide();
AreteDuale *creationADuale(int c1, int c2);
void generationADuale(Arete **t, GrapheDuale *gd, int size, Graphe * graphe);
void ajoutAreteDuale(Arete * arete, Arete * areteEquiv, GrapheDuale * gd, Graphe * graphe);
Arete *creationArete(int v1, int v2, int numFace);