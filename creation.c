#include "creation.h"
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
  gd->aretesDuales = malloc(sizeof(AreteDuale) * NB_FACES);
  gd->numAretes = 0;
  gd->numCentroides = 0;
  return gd;
}

AreteDuale *emptyArete() {
  AreteDuale *a = malloc(sizeof(AreteDuale));
  return a;
}
