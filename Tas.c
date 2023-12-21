#include "headers/Tas.h"

HeapAretes *HAVide()
{
  HeapAretes *t = malloc(sizeof(HeapAretes));
  t->noeudsAlloues = 0;
  t->numNoeuds = 0;
  t->T = malloc(sizeof(Arete *) * (3 * NB_FACES));
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
  Arete *a = malloc(sizeof(Arete));
  if (v1 > v2)
  {
    a->v1 = v2;
    a->v2 = v1;
  }
  else
  {
    a->v1 = v1;
    a->v2 = v2;
  }
  a->indexFace = numFace;
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


