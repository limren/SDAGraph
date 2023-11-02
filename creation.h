#pragma once
#include "const.h"
#include "structs.h"

Vertex *emptyVertex();
Face *emptyFace();
Maillage *emptyMaillage();
GrapheDuale *emptyGDuale();
AreteDuale *emptyArete();
Arete *creationArete(int v1, int v2, Face *f)