#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define NB_VERTICES 80
#define NB_FACES 80
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

struct Vertex {
  // Coordonnées des sommets
  float x;
  float y;
  float z;
};

typedef struct Vertex Vertex;

struct Face {
  // On conserve l'indice des vertex
  int v1;
  int v2;
  int v3;
};

typedef struct Face Face;

struct Maillage {
  int numVertices;
  Vertex **vertices;
  int numFaces;
  Face **faces;
};

typedef struct Maillage Maillage;

typedef Vertex Centroide;

struct AreteDuale {
  int indiceC1;
  int indiceC2;
};

typedef struct AreteDuale AreteDuale;

struct GrapheDuale {
  int numCentroides;
  Centroide **centroides;
  int numAretes;
  AreteDuale **aretesDuales;
};

typedef struct GrapheDuale GrapheDuale;

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
  m->faces = malloc(sizeof(struct Face) * 30);
  m->vertices = malloc(sizeof(struct Vertex) * 15);
  m->numVertices = 0;
  m->numFaces = 0;

  return m;
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

/*Maillage *parseDualGraph(char *path) {
  Maillage *m = emptyMaillage();
  int fdRead;
  CHK(fdRead = open(path, O_RDONLY));
  int readBytes;
  char *buffer[BUFFER_SIZE];
  int nbVertices = 0;
  int nbFaces = 0;
  // Si dans vertex = on continue en légende dans la vertex sinon on continue
  // dans la face
  bool wasInVertex = true;
  Vertex *currVertex;
  Face *currFace;
  // 0 - x | 1 - y | 2 - z
  Vertex *new;
  Face *new;

  int currCoord = 0;
  bool hasDone = true;
  while ((readBytes = read(fdRead, buffer, BUFFER_SIZE)) > 0) {
    if (wasInVertex) {
      if (hasDone) {
        new = emptyVertex(nbVertices);
        nbVertices++;
        currCoord = 0;
      } else {
        new = currVertex;
      }
    } else {
      if (hasDone) {
        new = emptyFace();
        nbFaces++;
        currCoord = 0;
      } else {
        new = currFace;
      }
    }
    for(int i = 0; i<readBytes; i++)
    {

    }
  }

  CHK(close(fdRead));
}
*/

Maillage *parseDualGraph(char *path) {
  Maillage *m = emptyMaillage();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread)) {
    if (buffer[0] == 'f') {
      Face *f = emptyFace();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      addFace(m, f);
    } else if (buffer[0] == 'v') {
      Vertex *v = emptyVertex();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      addVertex(m, v);
    }
  }

  fclose(fread);
  return m;
}

void writeGDuale(char *path, GrapheDuale *grapheDuale) {
  int fdWrite = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < grapheDuale->numCentroides; i++) {
    snprintf(buffer, BUFFER_SIZE, "v %f %f %f\n", grapheDuale->centroides[i]->x,
             grapheDuale->centroides[i]->y, grapheDuale->centroides[i]->z);
    write(fdWrite, buffer, BUFFER_SIZE);
  }
  for (int i = 0; i < grapheDuale->numAretes; i++) {
    snprintf(buffer, BUFFER_SIZE, "l %d %d\n",
             grapheDuale->aretesDuales[i]->indiceC1,
             grapheDuale->aretesDuales[i]->indiceC2);
    write(fdWrite, buffer, BUFFER_SIZE);
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

int main(int argc, char *argv[]) {
  if (argc < 3) {
    raler(0, "Usage: %s nom_fichier.obj nomsortant.obj", argv[0]);
  }

  Maillage *m = parseDualGraph(argv[1]);
  checkValues(m);
}