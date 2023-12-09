#pragma once
#include "const.h"
#include "structs.h"
#include "Graph.h"
Vertex *emptyVertex();
Face *emptyFace();
Maillage *emptyMaillage();
GrapheDuale *emptyGDuale();
AreteDuale *emptyArete();
Arete *creationSelectArete(SelectAretes *sa, int v1, int v2, int numFace);
Arete *creationHeapArete(HeapAretes *ha, int v1, int v2, int numFace);
int sontEquilaventes(Arete *a, Arete *b);
SelectAretes *emptySA();
HeapAretes *emptyHA();
void generationADuale(Arete **t, GrapheDuale *gd, int size, Graph *g);
Arete *creationArete(int v1, int v2, int numFace);