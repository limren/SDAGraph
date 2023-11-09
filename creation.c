#include "creation.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
Vertex *emptyVertex() {
  Vertex *v = malloc(sizeof(struct Vertex));
  return v;
}

Face *emptyFace() {
  Face *f = malloc(sizeof(struct Face));
  return f;
}

Maillage *emptyMaillage() {
  Maillage *m = malloc(sizeof(struct Maillage));
  // On met 15 au pif mais tqt on fera des tests pour savoir en moyenne lequel
  // est le plus opti
  m->faces = malloc(sizeof(struct Face) * NB_FACES);
  m->vertices = malloc(sizeof(struct Vertex) * NB_VERTICES);
  m->numVertices = 0;
  m->numFaces = 0;
  return m;
}

GrapheDuale *emptyGDuale() {
  GrapheDuale *gd = malloc(sizeof(struct GrapheDuale));
  // m->aretesDuales = malloc
  gd->centroides = malloc(sizeof(Centroide) * NB_FACES);
  // On prend le pire des cas pour le moment où on estime qu'il y a 3 arêtes
  // communes
  gd->aretesDuales = malloc(sizeof(AreteDuale) * (3 * NB_FACES));
  gd->numAretes = 0;
  gd->numCentroides = 0;
  return gd;
}

AreteDuale *emptyAreteDuale() {
  AreteDuale *a = malloc(sizeof(AreteDuale));
  return a;
}
int sontEquilaventes(Arete *a, Arete *b) {
  if ((a->v1 == b->v2 || a->v1 == b->v1) &&
      (a->v2 == b->v1 || a->v2 == b->v2)) {
    return 1;
  }
  return 0;
}

SelectAretes *emptySA() {
  SelectAretes *sa = malloc(sizeof(SelectAretes));
  sa->aretes = malloc(sizeof(Arete) * 3 * (NB_FACES));
  sa->numAretes = 0;
  return sa;
}

Arete *creationArete(SelectAretes *sa, int v1, int v2, Face *f) {
  if ((sa->numAretes % (3 * (NB_FACES))) == 0) {
    sa->aretes =
        realloc(sa->aretes, sizeof(Arete) * (sa->numAretes + (3 * (NB_FACES))));
  }
  bool hasEquivalente = false;

  Arete *a = malloc(sizeof(Arete));
  a->v1 = v1;
  a->v2 = v2;
  a->f = f;
  for (int i = 0; i < sa->numAretes && !hasEquivalente; i++) {
    if (sontEquilaventes(sa->aretes[i], a)) {
      hasEquivalente = true;
    }
  }
  if (hasEquivalente) {
    free(a);
    return NULL;
  }
  return a;
}

HeapAretes *emptyHA() {
  HeapAretes *t = malloc(sizeof(HeapAretes));
  t->noeudsAlloues = 0;
  t->T = malloc(sizeof(struct Arete) * (3 * NB_FACES));
  t->capacite = sizeof(struct Arete) * (3 * NB_FACES);
  return t;
}
