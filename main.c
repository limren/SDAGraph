#include "const.h"
#include "creation.h"
#include "structs.h"
#include "tris.h"
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define CHK(op)                                                                \
  do {                                                                         \
    if ((op) == -1) {                                                          \
      perror(#op);                                                             \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

noreturn void raler(int syserr, const char *msg, ...) {
  va_list ap;

  va_start(ap, msg);
  vfprintf(stderr, msg, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  if (syserr == 1)
    perror("");

  exit(EXIT_FAILURE);
}

void addFace(Maillage *m, Face *f) {
  if (m->numFaces % NB_FACES == 0) {
    m->faces =
        realloc(m->faces, sizeof(struct Face) * (m->numFaces + NB_FACES));
  }
  m->faces[m->numFaces] = f;
  m->numFaces++;
}

void addVertex(Maillage *m, Vertex *v) {
  if (m->numVertices % NB_VERTICES == 0) {
    m->vertices = realloc(m->vertices, sizeof(struct Vertex) *
                                           (m->numVertices + NB_VERTICES));
  }
  m->vertices[m->numVertices] = v;
  m->numVertices++;
}
void addCentroide(GrapheDuale *m, Centroide *c) {
  if (m->numCentroides % NB_FACES == 0) {
    m->centroides = realloc(m->centroides,
                            sizeof(Centroide) * (m->numCentroides + NB_FACES));
  }
  m->centroides[m->numCentroides] = c;
  m->numCentroides++;
}

void addAreteDuale(GrapheDuale *gd, AreteDuale *a) {
  if (gd->numAretes % NB_FACES == 0) {
    gd->aretesDuales = realloc(gd->aretesDuales,
                               sizeof(AreteDuale) * (gd->numAretes + NB_FACES));
  }
  gd->aretesDuales[gd->numAretes] = a;
  gd->numAretes++;
}

void addSelectArete(SelectAretes *sa, Face *f) {

  Arete *a = creationArete(sa, f->v1, f->v2, f);
  if (a != NULL) {
    sa->aretes[sa->numAretes++] = a;
  }

  Arete *b = creationArete(sa, f->v1, f->v3, f);
  if (b != NULL) {
    sa->aretes[sa->numAretes++] = b;
  }

  Arete *c = creationArete(sa, f->v2, f->v3, f);
  if (c != NULL) {
    sa->aretes[sa->numAretes++] = c;
  }
}

Maillage *parseDualGraphSelect(char *path, SelectAretes *sa) {
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread)) {
    if (buffer[0] == 'f') {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
      addSelectArete(sa, f);
    } else if (buffer[0] == 'v') {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }
  fclose(fread);
  return m;
}

/*
Maillage *parseDualGraphHeap(char *path, HeapAretes *sa) {
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread)) {
    if (buffer[0] == 'f') {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
      addAreteHeap(m, f);
    } else if (buffer[0] == 'v') {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }
  fclose(fread);
  return m;
}*/

void writeGDuale(char *path, GrapheDuale *grapheDuale) {
  int fdWrite = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < grapheDuale->numCentroides; i++) {
    int len = snprintf(
        buffer, BUFFER_SIZE, "v %f %f %f\n", grapheDuale->centroides[i]->x,
        grapheDuale->centroides[i]->y, grapheDuale->centroides[i]->z);
    write(fdWrite, buffer, len);
  }

  for (int i = 0; i < grapheDuale->numAretes; i++) {
    int len = snprintf(buffer, BUFFER_SIZE, "l %d %d\n",
                       grapheDuale->aretesDuales[i]->indiceC1,
                       grapheDuale->aretesDuales[i]->indiceC2);
    write(fdWrite, buffer, len);
  }
}

Centroide *calculCentroide(Face *f, Vertex **v) {
  Centroide *c = emptyVertex();
  c->x = (v[f->v1 - 1]->x + v[f->v2 - 1]->x + v[f->v3 - 1]->x) / 3;
  c->y = (v[f->v1 - 1]->y + v[f->v2 - 1]->y + v[f->v3 - 1]->y) / 3;
  c->z = (v[f->v1 - 1]->z + v[f->v2 - 1]->z + v[f->v3 - 1]->z) / 3;
  return c;
}

bool aAreteCommun(struct Face *f1, struct Face *f2) {
  int communs = 0;

  if (f1->v1 == f2->v1 || f1->v1 == f2->v2 || f1->v1 == f2->v3) {
    communs++;
  }

  if (f1->v2 == f2->v1 || f1->v2 == f2->v2 || f1->v2 == f2->v3) {
    communs++;
  }

  if (f1->v3 == f2->v1 || f1->v3 == f2->v2 || f1->v3 == f2->v3) {
    communs++;
  }

  return communs == 2;
}

void creationCentroides(GrapheDuale *gd, Maillage *m) {
  for (int i = 0; i < m->numFaces; i++) {
    addCentroide(gd, calculCentroide(m->faces[i], m->vertices));
  }
}

void checkValues(Maillage *m) {
  for (int i = 0; i < m->numFaces; i++) {
    printf("Val %d : %d %d %d \n", i + 1, m->faces[i]->v1, m->faces[i]->v2,
           m->faces[i]->v3);
  }

  for (int i = 0; i < m->numVertices; i++) {
    printf("Val %d : %f %f %f \n", i + 1, m->vertices[i]->x, m->vertices[i]->y,
           m->vertices[i]->z);
  }

  printf("Number faces & vertices : %d %d \n", m->numFaces, m->numVertices);
}

void checkSelectAretes(SelectAretes *sa) {
  for (int i = 0; i < 21; i++) {
    printf("Aretes : %d %d \n", sa->aretes[i]->v1, sa->aretes[i]->v2);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    raler(0,
          "Usage: %s nom_fichier.obj nomsortant.obj type_algo [selectsort, "
          "heapsort, AVL]",
          argv[0]);
  }

  // checkValues(m);

  GrapheDuale *gd = emptyGDuale();

  //  creationCentroides(gd, m);
  // writeGDuale(argv[2], gd);
  if (strcmp(argv[3], "selectsort") == 0) {
    SelectAretes *sa = emptySA();
    Maillage *m = parseDualGraphSelect(argv[1], sa);
    checkSelectAretes(sa);
    triSelection(sa);
    printf("-------------- \n");
    checkSelectAretes(sa);
  } else if (strcmp(argv[3], "heapsort") == 0) {
    // HeapAretes *ha = emptyHA();
    // Maillage *m = parseDualGraphHeap(argv[1], );
  }
}
