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
  // 3 * NB_FACES car on a 3 arêtes par face et donc au pire des cas, 3 arêtes duales.
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


/*
  Création des arêtes :
    - On a décidé d'attribuer l'indice des vertex selon un ordre bien précis afin de pouvoir ordonner
    nos arêtes. On a décidé de toujours mettre le plus petit indice en premier. Comme ça, il suffit de 
    comparer les deux premiers indices pour savoir si deux arêtes sont équivalentes.
*/

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

/*
  Création arête duale :
    - Nous avons simplement mis les indices des centroïdes qui sont réliés d'une manière à ce qu'à la génération,
    on puisse accéder à la face correspondante en un temps constant. 
*/
AreteDuale *creationADuale(int c1, int c2)
{
  AreteDuale *a = malloc(sizeof(AreteDuale));
  a->indiceC1 = c1;
  a->indiceC2 = c2;
  return a;
}

/*
  generationADuale :
    - On parcourt le tableau d'arêtes et on compare les arêtes deux à deux. Si elles sont équivalentes, alors
    on les ajoute dans le graphe duale. Cela veut simplement dire qu'elles relient deux centroïdes. Donc forme une arête duale.
*/
void generationADuale(Arete **t, GrapheDuale *gd, int size, Graphe * graphe)
{
  for (int i = 0; i < size-1; i++)
  {
    if (sontEquilaventes(t[i], t[i + 1]))
    {

     ajoutAreteDuale(t[i], t[i + 1], gd, graphe);
    }
  }
}

/*
  ajoutAreteDuale :
    - On ajoute les arêtes duales au graphe duale
    - On ajoute les arêtes duales au graphe
    Dès que l'on ajoute une arête duale, il faut également l'ajouter au graphe - dans les deux sens puisque le 
    graphe est non orienté.
*/
void ajoutAreteDuale(Arete * arete, Arete * areteEquiv, GrapheDuale * gd, Graphe * graphe)
{
  // Ajout de l'arête dans le graphe duale
     if (gd->numAretesDuales % (3 * NB_FACES) == 0)
      {
        gd->aretesDuales = realloc(gd->aretesDuales,
                                   sizeof(AreteDuale) *
                                       (gd->numAretesDuales + (3 * NB_FACES)));
      }
      gd->aretesDuales[gd->numAretesDuales] = creationADuale(arete->indexFace, areteEquiv->indexFace);
      gd->numAretesDuales++;
      // Ajout de l'arête dans le graphe non orientée, pour le parcours en largeur - donc on doit ajouter (a,b) et (b,a) 
      ajoutArcGraph(graphe, arete->indexFace, areteEquiv->indexFace);
      ajoutArcGraph(graphe, areteEquiv->indexFace, arete->indexFace);
}