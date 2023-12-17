#include "tris.h"
#include <stdio.h>

double sommeIndex(Arete *a) { return a->v1 + a->v2; }

int estSuperieureA(Arete *a, Arete *b)
{
  if (a->v1 > b->v1 || (a->v1 == b->v1 && a->v2 > b->v2))
  {
    return 1;
  }
  return 0;
}

void echange(Arete **a, int i, int j)
{
  Arete *tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void triSelection(SelectAretes *sa)
{
  for (int i = sa->numAretes - 1; i > 0; i--)
  {
    int indiceMax = 0;
    for (int j = 1; j <= i; j++)
    {
      if (estSuperieureA(sa->aretes[j], sa->aretes[indiceMax]))
      {
        indiceMax = j;
      }
    }
    echange(sa->aretes, indiceMax, i);
  }
}

int filsGauche(int i) { return 2 * i + 1; }
int filsDroit(int i) { return 2 * i + 2; }
int pere(int i) { return (i - 1) / 2; }

void insertionTas(HeapAretes *t, Arete *val)
{
  int i = t->noeudsAlloues;
  t->T[t->noeudsAlloues] = val;
  while (i > 0 && estSuperieureA(val, t->T[pere(i)]))
  {
    Arete *tmp = t->T[pere(i)];
    t->T[(i - 1) / 2] = val;
    t->T[i] = tmp;
    i = (i - 1) / 2;
  }
  t->noeudsAlloues++;
  t->numNoeuds++;
}

void afficherTas(HeapAretes *t)
{
  for (int i = 0; i < t->noeudsAlloues; i++)
  {
    printf("%d %d ", t->T[i]->v1, t->T[i]->v2);
  }
  printf("\n");
}

void supprimerMax(HeapAretes *t)
{
  int i = t->noeudsAlloues - 1;
  Arete *suppr = t->T[0];
  t->T[0] = t->T[i];
  i = 0;
  while (2 * i + 2 < t->noeudsAlloues &&
         (estSuperieureA(t->T[filsGauche(i)], t->T[i]) || estSuperieureA(t->T[filsDroit(i)], t->T[i])))
  {
    if (estSuperieureA(t->T[filsGauche(i)], t->T[filsDroit(i)]))
    {
      Arete *tmp = t->T[2 * i + 1];
      t->T[2 * i + 1] = t->T[i];
      t->T[i] = tmp;
      i = 2 * i + 1;
    }
    else
    {
      Arete *tmp = t->T[2 * i + 2];
      t->T[2 * i + 2] = t->T[i];
      t->T[i] = tmp;
      i = 2 * i + 2;
    }
  }
  t->T[t->noeudsAlloues] = suppr;
  t->noeudsAlloues--;
}
