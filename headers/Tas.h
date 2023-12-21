#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "const.h"
#include "tris.h"
#include "creation.h"
HeapAretes *HAVide();
Arete *creationTasArete(HeapAretes *ha, int v1, int v2, int numFace);
void ajoutTasArete(HeapAretes *ha, Face *f, int numFace);