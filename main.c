#include "AVL.h"
#include "const.h"
#include "creation.h"
#include "structs.h"
#include "tris.h"
#include "Graph.h"
#include "hashmap/hashmap.h"
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
#include <time.h>
#include <unistd.h>
#define MAX_SIZE 255
#define CHK(op)     \
  do                \
  {                 \
    if ((op) == -1) \
    {               \
      perror(#op);  \
      exit(1);      \
    }               \
  } while (0)

noreturn void raler(int syserr, const char *msg, ...)
{
  va_list ap;

  va_start(ap, msg);
  vfprintf(stderr, msg, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  if (syserr == 1)
    perror("");

  exit(EXIT_FAILURE);
}

void addFace(Maillage *m, Face *f)
{
  if (m->numFaces % NB_FACES == 0)
  {
    m->faces =
        realloc(m->faces, sizeof(struct Face) * (m->numFaces + NB_FACES));
  }
  m->faces[m->numFaces] = f;
  m->numFaces++;
}

void addVertex(Maillage *m, Vertex *v)
{
  if (m->numVertices % NB_VERTICES == 0)
  {
    m->vertices = realloc(m->vertices, sizeof(struct Vertex) *
                                           (m->numVertices + NB_VERTICES));
  }
  m->vertices[m->numVertices] = v;
  m->numVertices++;
}
void addCentroide(GrapheDuale *m, Centroide *c)
{
  if (m->numCentroides % NB_FACES == 0)
  {
    m->centroides = realloc(m->centroides,
                            sizeof(Centroide) * (m->numCentroides + NB_FACES));
  }
  m->centroides[m->numCentroides] = c;
  m->numCentroides++;
}

void addAreteDuale(GrapheDuale *gd, AreteDuale *a)
{
  if (gd->numAretesDuales % NB_FACES == 0)
  {
    gd->aretesDuales =
        realloc(gd->aretesDuales,
                sizeof(AreteDuale) * (gd->numAretesDuales + NB_FACES));
  }
  gd->aretesDuales[gd->numAretesDuales] = a;
  gd->numAretesDuales++;
}

void addSelectArete(SelectAretes *sa, Face *f, int numFace)
{

  Arete *a = creationSelectArete(sa, f->v1, f->v2, numFace);
  if (a != NULL)
  {
    sa->aretes[sa->numAretes++] = a;
  }

  Arete *b = creationSelectArete(sa, f->v1, f->v3, numFace);
  if (b != NULL)
  {
    sa->aretes[sa->numAretes++] = b;
  }

  Arete *c = creationSelectArete(sa, f->v2, f->v3, numFace);
  if (c != NULL)
  {
    sa->aretes[sa->numAretes++] = c;
  }
}

Maillage *parseDualGraphSelect(char *path, SelectAretes *sa)
{
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
      addSelectArete(sa, f, m->numFaces - 1);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }
  fclose(fread);
  return m;
}

void addHeapArete(HeapAretes *ha, Face *f, int numFace)
{
  Arete *a = creationHeapArete(ha, f->v1, f->v2, numFace);
  if (a != NULL)
  {
    insertionTas(ha, a);
  }

  Arete *b = creationHeapArete(ha, f->v1, f->v3, numFace);
  if (b != NULL)
  {
    insertionTas(ha, b);
  }

  Arete *c = creationHeapArete(ha, f->v2, f->v3, numFace);
  if (c != NULL)
  {
    insertionTas(ha, c);
  }
}

Maillage *parseDualGraphHeap(char *path, HeapAretes *ha)
{
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
      addHeapArete(ha, f, m->numFaces - 1);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }
  fclose(fread);
  return m;
}

Maillage *parseDualGraphAVL(char *path)
{
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }
  fclose(fread);
  return m;
}

void writeGDuale(char *path, GrapheDuale *grapheDuale, Graph * g, int largestDistance)
{
  int fdWrite = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < grapheDuale->numCentroides; i++)
  {
    GraphNode * node = g->listAdjacents[i];
    // Calcule de la couleur, ici on prend largest distance ayant 1 comme couleur et ainsi on fait un produit en croix
    float ratio = 0;
    if(largestDistance != 0)
    {
      ratio = (float)node->distance/largestDistance;
    }
    int len = snprintf(
        buffer, BUFFER_SIZE, "v %f %f %f %f %f %f\n", grapheDuale->centroides[i]->x,
        grapheDuale->centroides[i]->y, grapheDuale->centroides[i]->z, ratio, ratio, ratio);
    write(fdWrite, buffer, len);
  }

  for (int i = 0; i < grapheDuale->numAretesDuales; i++)
  {
    int len = snprintf(buffer, BUFFER_SIZE, "l %d %d\n",
                       grapheDuale->aretesDuales[i]->indiceC1 + 1,
                       grapheDuale->aretesDuales[i]->indiceC2 + 1);
    write(fdWrite, buffer, len);
  }
}

Centroide *calculCentroide(Face *f, Vertex **v)
{
  Centroide *c = emptyVertex();
  c->x = (v[f->v1 - 1]->x + v[f->v2 - 1]->x + v[f->v3 - 1]->x) / 3;
  c->y = (v[f->v1 - 1]->y + v[f->v2 - 1]->y + v[f->v3 - 1]->y) / 3;
  c->z = (v[f->v1 - 1]->z + v[f->v2 - 1]->z + v[f->v3 - 1]->z) / 3;
  return c;
}

bool aAreteCommun(struct Face *f1, struct Face *f2)
{
  int communs = 0;

  if (f1->v1 == f2->v1 || f1->v1 == f2->v2 || f1->v1 == f2->v3)
  {
    communs++;
  }

  if (f1->v2 == f2->v1 || f1->v2 == f2->v2 || f1->v2 == f2->v3)
  {
    communs++;
  }

  if (f1->v3 == f2->v1 || f1->v3 == f2->v2 || f1->v3 == f2->v3)
  {
    communs++;
  }

  return communs == 2;
}

void creationCentroides(GrapheDuale *gd, Maillage *m)
{
  for (int i = 0; i < m->numFaces; i++)
  {
    addCentroide(gd, calculCentroide(m->faces[i], m->vertices));
  }
}

void addEntryHashmap(struct hashmap_s * hashmap, Arete * arete, GrapheDuale * gd, Graph * graphe)
{
  char key[MAX_SIZE];
  int nb = snprintf(key, MAX_SIZE, "%d-%d", arete->v1, arete->v2);
  if(nb >= MAX_SIZE)
  {
    perror("Erreur snprintf key");
    exit(EXIT_FAILURE);
  }
  if(hashmap == NULL)
  {
    perror("Erreur hashmap null");
    exit(EXIT_FAILURE);
  }
  Arete * areteEquiv = hashmap_get(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)));
  if(areteEquiv != NULL)
  {
    // comme hashmap_get peut renvoyer une valeur même si l'entrée n'existe pas, on vérifie que les deux arêtes sont bien équivalentes
    if(sontEquilaventes(arete, areteEquiv)){
      if (gd->numAretesDuales % (3 * NB_FACES) == 0)
      {
        gd->aretesDuales = realloc(gd->aretesDuales, sizeof(AreteDuale) * (gd->numAretesDuales + (3 * NB_FACES)));
      }
      gd->aretesDuales[gd->numAretesDuales] = creationADuale(areteEquiv->indexFace, arete->indexFace);
      gd->numAretesDuales++;
      addArcGraph(graphe, areteEquiv->indexFace, arete->indexFace);
      addArcGraph(graphe, arete->indexFace, areteEquiv->indexFace);
    } else {
      hashmap_put(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)), arete);
    }
  } else {
    hashmap_put(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)), arete);
  }
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    raler(0,
          "Usage: %s nom_fichier.obj nomsortant.obj type_algo [selectsort, "
          "heapsort, AVL, hashmap]",
          argv[0]);
  }
  clock_t start, end;
  double cpu_time_used;
  GrapheDuale *gd = emptyGDuale();
  Graph *graphe = initGraph();
 
  if (strcmp(argv[3], "selectsort") == 0)
  {
    SelectAretes *sa = emptySA();
    Maillage *m = parseDualGraphSelect(argv[1], sa);

    start = clock();
    triSelection(sa);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    creationCentroides(gd, m);
    generationADuale(sa->aretes, gd, sa->numAretes, graphe);
    int largestDistance = parcoursLargeur(graphe);
    writeGDuale(argv[2], gd, graphe, largestDistance);
  }
  else if (strcmp(argv[3], "heapsort") == 0)
  {
    HeapAretes *ha = emptyHA();
    Maillage *m = parseDualGraphHeap(argv[1], ha);
    start = clock();
    while (ha->noeudsAlloues > 0)
    {
      supprimerMax(ha);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    creationCentroides(gd, m);
    generationADuale(ha->T, gd, ha->numNoeuds+1, graphe);
    int largestDistance = parcoursLargeur(graphe);
    writeGDuale(argv[2], gd, graphe, largestDistance);
  }
  else if (strcmp(argv[3], "AVL") == 0)
  {
    AVL a = NULL;
    Maillage *m = parseDualGraphAVL(argv[1]);
    geneADuales(m, &a);
    affichageArbre(a);
  } else if(strcmp(argv[3], "hashmap") == 0)
  {
    SelectAretes *sa = emptySA();
    Maillage *m = parseDualGraphSelect(argv[1], sa);
    creationCentroides(gd, m);
    printf("size num aretes : %d\n", sa->numAretes);
    const unsigned initial_size = sa->numAretes*10;
    struct hashmap_s hashmap;
    if (0 != hashmap_create(initial_size, &hashmap)) {
      exit(EXIT_FAILURE);
    }
    start = clock();
    for(int i = 0; i<sa->numAretes; i++)
    {
      addEntryHashmap(&hashmap, sa->aretes[i], gd, graphe);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    int largestDistance = parcoursLargeur(graphe);
    writeGDuale(argv[2], gd, graphe, largestDistance);
    hashmap_destroy(&hashmap);
  }
}
