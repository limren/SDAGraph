#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "const.h"

SelectAretes *SAVide();
Arete *creationAreteSelection(SelectAretes *sa, int v1, int v2, int numFace);
void ajoutAreteSelection(SelectAretes *sa, Face *f, int numFace);
