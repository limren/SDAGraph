#pragma once

#include "structs.h"
double calculLongueur(Arete *a);
int estSuperieureA(Arete *a, Arete *b);
void triSelection(SelectAretes *sa);
void insertionTas(HeapAretes *t, Arete *val);
void supprimerMax(HeapAretes *t);