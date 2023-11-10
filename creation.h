#pragma once
#include "const.h"
#include "structs.h"

Vertex *emptyVertex();
Face *emptyFace();
Maillage *emptyMaillage();
GrapheDuale *emptyGDuale();
AreteDuale *emptyArete();
Arete *creationArete(SelectAretes *sa, int v1, int v2, Face *f);
Arete *creationHeapArete(HeapAretes *ha, int v1, int v2, Face *f);
int sontEquilaventes(Arete *a, Arete *b);
SelectAretes *emptySA();
HeapAretes *emptyHA();