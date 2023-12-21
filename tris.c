#include "headers/tris.h"
#include <stdio.h>


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


/*
  triSelection :
  - sa : structure contenant les arêtes à trier
  - numAretes : nombre d'arêtes dans la structure
  -- On parcourt le tableau et on échange les valeurs si la valeur de droite est plus petite que celle de gauche
*/

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

/*
  insertionTas :
  - t : tas dans lequel on insère
  - val : valeur à insérer
  - numNoeuds : nombre de noeuds dans le tas
  -- On insère la valeur à la fin du tas et on remonte la valeur jusqu'à ce qu'elle soit à la bonne place
  2i+2 = fils droit
  2i+1 = fils gauche
  (i-1)/2 = père
*/

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

/*
  supprimerMax :
  - t : tas dans lequel on supprime
  - numNoeuds : nombre de noeuds dans le tas
  -- On supprime la racine et on remplace la racine par la dernière valeur du tas
  On fait descendre la valeur jusqu'à ce qu'elle soit à la bonne place
*/
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
