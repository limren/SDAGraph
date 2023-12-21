#include "headers/creation.h"
#include "headers/AVL.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
Vertex *sommetVide()
{
  Vertex *v = malloc(sizeof(struct Vertex));
  return v;
}

Face *faceVide()
{
  Face *f = malloc(sizeof(struct Face));
  return f;
}

Maillage *maillageVide()
{
  Maillage *m = malloc(sizeof(struct Maillage));
  // On met 15 au pif mais tqt on fera des tests pour savoir en moyenne lequel
  // est le plus opti
  m->faces = malloc(sizeof(struct Face) * NB_FACES);
  m->sommets = malloc(sizeof(struct Vertex) * NB_VERTICES);
  m->numSommets = 0;
  m->numFaces = 0;
  return m;
}

GrapheDuale *GDualeVide()
{
  GrapheDuale *gd = malloc(sizeof(struct GrapheDuale));
  gd->centroides = malloc(sizeof(Centroide) * NB_FACES);
  gd->aretesDuales = malloc(sizeof(AreteDuale) * (3 * NB_FACES));
  gd->numAretesDuales = 0;
  gd->numCentroides = 0;
  return gd;
}


int sontEquilaventes(Arete *a, Arete *b)
{
  if(a->indexFace == b->indexFace) return 0;
  if (a->v1 == b->v1 && a->v2 == b->v2)
  {
    return 1;
  }
  return 0;
}


Arete *creationArete(int v1, int v2, int numFace)
{
  Arete *a = malloc(sizeof(Arete));
  a->indexFace = numFace;
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
  return a;
}

AreteDuale *creationADuale(int c1, int c2)
{
  AreteDuale *a = malloc(sizeof(AreteDuale));
  a->indiceC1 = c1;
  a->indiceC2 = c2;
  return a;
}

void generationADuale(Arete **t, GrapheDuale *gd, int size, Graphe * graphe)
{
  for (int i = 0; i < size-1; i++)
  {
    if (sontEquilaventes(t[i], t[i + 1]))
    {

      if (gd->numAretesDuales % (3 * NB_FACES) == 0)
      {
        gd->aretesDuales = realloc(gd->aretesDuales,
                                   sizeof(AreteDuale) *
                                       (gd->numAretesDuales + (3 * NB_FACES)));
      }
      gd->aretesDuales[gd->numAretesDuales] = creationADuale(t[i]->indexFace, t[i + 1]->indexFace);
      gd->numAretesDuales++;
      ajoutArcGraph(graphe, t[i]->indexFace, t[i+1]->indexFace);
      ajoutArcGraph(graphe, t[i+1]->indexFace, t[i]->indexFace);
    }
  }
}