#include "headers/AVL.h"
#include "headers/const.h"
#include "headers/creation.h"
#include "headers/structs.h"
#include "headers/tris.h"
#include "headers/Graphe.h"
#include "headers/Selection.h"
#include "headers/Tas.h"
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
// Définition de la taille maximale de la key (on imagine que c'est impossible d'avoir deux arêtes ayant une somme de leurs chiffres faisant 255)
#define MAX_SIZE 255


void ajoutFace(Maillage *m, Face *f)
{
  if (m->numFaces % NB_FACES == 0)
  {
    m->faces =
        realloc(m->faces, sizeof(struct Face) * (m->numFaces + NB_FACES));
  }
  m->faces[m->numFaces] = f;
  m->numFaces++;
}

void ajoutSommet(Maillage *m, Vertex *v)
{
  if (m->numSommets % NB_VERTICES == 0)
  {
    m->sommets = realloc(m->sommets, sizeof(struct Vertex) *
                                           (m->numSommets + NB_VERTICES));
  }
  m->sommets[m->numSommets] = v;
  m->numSommets++;
}
void ajoutCentroide(GrapheDuale *m, Centroide *c)
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


Maillage *parseDualGraphSelect(char *path, SelectAretes *sa)
{
  Maillage *m = maillageVide();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = faceVide();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      ajoutFace(m, f);
      ajoutAreteSelection(sa, f, m->numFaces - 1);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = sommetVide();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      ajoutSommet(m, v);
    }
  }
  fclose(fread);
  return m;
}

Maillage *parseDualGraphHeap(char *path)
{
  Maillage *m = maillageVide();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = faceVide();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      ajoutFace(m, f);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = sommetVide();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      ajoutSommet(m, v);
    }
  }
  fclose(fread);
  return m;
}

Maillage *parseDualGraphAVL(char *path)
{
  Maillage *m = maillageVide();
  FILE *fread;
  fread = fopen(path, "r");
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, fread))
  {
    if (buffer[0] == 'f')
    {
      Face *f = faceVide();
      sscanf(buffer, "f %d %d %d", &f->v1, &f->v2, &f->v3);
      ajoutFace(m, f);
    }
    else if (buffer[0] == 'v')
    {
      Vertex *v = sommetVide();
      sscanf(buffer, "v %f %f %f", &v->x, &v->y, &v->z);
      ajoutSommet(m, v);
    }
  }
  fclose(fread);
  return m;
}

void ecritureGDuale(char *path, GrapheDuale *grapheDuale, Graphe * g, int plusGrandeDistance)
{
  int fdWrite = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < grapheDuale->numCentroides; i++)
  {
    NoeudGraphe * noeud = g->listAdjacents[i];
    // Il y a des cas très rares, uniquement dans le dragon.obj où le centroide n'a pas de voisin, on le skip
    // Calcul de la couleur, ici on prend la plus grande distance ayant 1 comme couleur et ainsi on fait un produit en croix
    float ratio = 0;
    if(plusGrandeDistance != 0 && noeud != NULL)
    {
      ratio = (float)noeud->distance/plusGrandeDistance;
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
  Centroide *c = sommetVide();
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
    ajoutCentroide(gd, calculCentroide(m->faces[i], m->sommets));
  }
}


void ajoutEntreeHashmap(struct hashmap_s * hashmap, Arete * arete, GrapheDuale * gd, Graphe * graphe)
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
  // Si une entrée correspond à la clée utilisée, on récupère l'arête correspondante
  Arete * areteEquiv = hashmap_get(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)));
  if(areteEquiv != NULL)
  {
    // Comme hashmap_get peut renvoyer une valeur même si l'entrée n'existe pas (conflit probable), on vérifie que les deux arêtes sont bien équivalentes
    if(sontEquilaventes(arete, areteEquiv)){
      ajoutAreteDuale(arete, areteEquiv, gd, graphe);
    } else {
      // Si les deux arêtes ne sont pas équivalentes, on ajoute la key avec l'arête dans la hashmap
      hashmap_put(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)), arete);
    }
  } else {
    // Si l'entrée n'existe pas, on ajoute la key avec l'arête dans la hashmap
    hashmap_put(hashmap, key, HASHMAP_CAST(unsigned, strlen(key)), arete);
  }
}

int main(int argc, char *argv[])
{
  // Utilisation du programme (cf README.md)
  if (argc < 4)
  {
    perror(
          "Usage: ./main nom_fichier.obj nomsortant.obj type_algo [selectsort, "
          "heapsort, AVL, hashmap]");
  }
  clock_t start, end;
  double cpu_time_used;
  
  // Initialisation des graphes 
  GrapheDuale *gd = GDualeVide();
  Graphe *graphe = grapheVide();
 
  /*
    * Ici, on choisit l'algorithme de tri à utiliser selon l'utilisateur, afin de générer, dans un but final, le graphe duale.
    * Les étapes seront globalement les mêmes pour chaque algorithme, à savoir :
    * - On parse le fichier .obj pour récupérer les faces et les sommets
    * - On crée les centroides
    ** Génération du graphe duale
    * - On génère les arêtes duales en fonction de l'algorithme de tri choisi
    * - On ajoute les arêtes duales au graphe duale
    * - On génère le graphe duale
    ** Génération des couleurs
    * - On parcourt le graphe duale en largeur pour trouver la plus grande distance entre deux centroides
    * - On génère les couleurs en fonction de la plus grande distance
    ** Ecriture du fichier .obj
    * - On écrit les centroides dans le fichier .obj
    * - On écrit les arêtes duales dans le fichier .obj
    ** Libération de la mémoire
    * - On libère la mémoire allouée selon les structures de données utilisées
  */
  if (strcmp(argv[3], "selectsort") == 0)
  {
    SelectAretes *sa = SAVide();
    Maillage *m = parseDualGraphSelect(argv[1], sa);
    start = clock();
    triSelection(sa);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    creationCentroides(gd, m);
    generationADuale(sa->aretes, gd, sa->numAretes, graphe);
    int plusGrandeDistance = parcoursLargeur(graphe);
    ecritureGDuale(argv[2], gd, graphe, plusGrandeDistance);
  }
  else if (strcmp(argv[3], "heapsort") == 0)
  {
    HeapAretes *ha = HAVide();
    Maillage *m = parseDualGraphHeap(argv[1]);
    start = clock();
    for(int i = 0; i<m->numFaces; i++){
      ajoutTasArete(ha, m->faces[i], i);
    }
    while (ha->noeudsAlloues > 0)
    {
      supprimerMax(ha);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    creationCentroides(gd, m);
    generationADuale(ha->T, gd, ha->numNoeuds+1, graphe);
    int plusGrandeDistance = parcoursLargeur(graphe);
    ecritureGDuale(argv[2], gd, graphe, plusGrandeDistance);
  }
  else if (strcmp(argv[3], "AVL") == 0)
  {
    AVL a = NULL;
    Maillage *m = parseDualGraphAVL(argv[1]);
    geneADuales(m, &a, gd, graphe);
    affichageArbre(a);
    int plusGrandeDistance = parcoursLargeur(graphe);
    ecritureGDuale(argv[2], gd, graphe, plusGrandeDistance);
  } else if(strcmp(argv[3], "hashmap") == 0)
  {
    SelectAretes *sa = SAVide();
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
      ajoutEntreeHashmap(&hashmap, sa->aretes[i], gd, graphe);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    int distanceLaPlusGrande = parcoursLargeur(graphe);
    ecritureGDuale(argv[2], gd, graphe, distanceLaPlusGrande);
    hashmap_destroy(&hashmap);
  }
}
