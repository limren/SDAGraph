#pragma once

#include "structs.h"
#include "creation.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct noeud noeud;
typedef noeud *AVL;

struct noeud
{
  Arete *val;
  AVL filsGauche;
  AVL filsDroit;
  AVL pere;
  int hauteur;
};

AVL newArbre(Arete *val, int hauteur, AVL pere);
void rotationGauche(AVL *a);
void rotationDroite(AVL *a);
void rotationGD(AVL *a);
void rotationDG(AVL *a);
void insertionAVL(AVL *arbre, Arete *val, GrapheDuale * gd, Graphe * graphe);
int deseq(AVL a);
void affichageArbre(AVL arbre);
void checkDeseq(AVL * a);
void geneADuales(Maillage *m, AVL *arbre, GrapheDuale * gd, Graphe * graphe);
void addAVLAretes(AVL *avl, Face *f, int numFace, GrapheDuale * gd, Graphe * graphe);