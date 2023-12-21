#include "headers/Tas.h"

HeapAretes *HAVide()
{
  HeapAretes *t = malloc(sizeof(HeapAretes));
  t->noeudsAlloues = 0;
  t->numNoeuds = 0;
  t->T = malloc(sizeof(Arete *) * (3 * NB_FACES));
  // Chaque face a 3 aretes, donc on a 3 fois plus d'aretes que de faces 
  t->capacite = (3 * NB_FACES);
  return t;
}

Arete *creationTasArete(HeapAretes *ha, int v1, int v2, int numFace)
{
  if ((ha->capacite == ha->noeudsAlloues))
  {
    ha->capacite += (3 * NB_FACES);
    ha->T = realloc(ha->T, sizeof(Arete *) * ha->capacite);
  }
  Arete *a = creationArete(v1, v2, numFace);
  return a;
}

void ajoutTasArete(HeapAretes *ha, Face *f, int numFace)
{
  Arete *a = creationTasArete(ha, f->v1, f->v2, numFace);
  if (a != NULL)
  {
    insertionTas(ha, a);
  }

  Arete *b = creationTasArete(ha, f->v1, f->v3, numFace);
  if (b != NULL)
  {
    insertionTas(ha, b);
  }

  Arete *c = creationTasArete(ha, f->v2, f->v3, numFace);
  if (c != NULL)
  {
    insertionTas(ha, c);
  }
}


