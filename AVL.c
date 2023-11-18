#include "AVL.h"
#include "tris.h"
AVL newArbre(Arete *val, int hauteur, AVL pere) {
  AVL nouvelArbre = malloc(sizeof(struct noeud));
  if (nouvelArbre == NULL) {
    printf("Erreur lors de l'allocation de mÃ©moire.\n");
    exit(1);
  }
  nouvelArbre->val = val;
  nouvelArbre->pere = pere;
  nouvelArbre->hauteur = hauteur;
  nouvelArbre->filsGauche = NULL;
  nouvelArbre->filsDroit = NULL;
  return nouvelArbre;
}

int hauteur(AVL a) {
  if (a == NULL) {
    return -1; // La hauteur d'un nœud vide est -1
  } else {
    return a->hauteur;
  }
}

int max(int a, int b) { return (a > b) ? a : b; }

void rotationGauche(AVL a) {
  if (a != NULL && a->filsDroit != NULL) {
    AVL tmpFDG = a->filsDroit->filsGauche;
    AVL pere = a->pere;

    a->filsDroit->filsGauche = a;
    a->pere = a->filsDroit;

    if (tmpFDG != NULL) {
      tmpFDG->pere = a;
    }

    a->filsDroit = tmpFDG;

    if (pere != NULL) {
      if (pere->filsGauche == a) {
        pere->filsGauche = a->pere;
      } else {
        pere->filsDroit = a->pere;
      }
    }

    a->pere = pere;

    if (a->filsDroit != NULL) {
      a->filsDroit->hauteur = max(hauteur(a->filsDroit->filsGauche),
                                  hauteur(a->filsDroit->filsDroit)) +
                              1;
    }
    a->hauteur = max(hauteur(a->filsGauche), hauteur(a->filsDroit)) + 1;

    if (pere != NULL) {
      pere->hauteur =
          max(hauteur(pere->filsGauche), hauteur(pere->filsDroit)) + 1;
    }

    if (pere == NULL) {
      a = a->pere;
    }
  }
}
void rotationDroite(AVL a) {
  if (a != NULL && a->filsGauche != NULL) {
    AVL tmpFGD = a->filsGauche->filsDroit;
    AVL pere = a->pere;

    a->filsGauche->filsDroit = a;
    a->pere = a->filsGauche;

    if (tmpFGD != NULL) {
      tmpFGD->pere = a;
    }

    a->filsGauche = tmpFGD;

    if (pere != NULL) {
      if (pere->filsGauche == a) {
        pere->filsGauche = a->pere;
      } else {
        pere->filsDroit = a->pere;
      }
    }

    if (a->filsGauche != NULL) {
      a->filsGauche->hauteur = max(hauteur(a->filsGauche->filsGauche),
                                   hauteur(a->filsGauche->filsDroit)) +
                               1;
    }
    a->hauteur = max(hauteur(a->filsGauche), hauteur(a->filsDroit)) + 1;

    if (pere != NULL) {
      pere->hauteur =
          max(hauteur(pere->filsGauche), hauteur(pere->filsDroit)) + 1;
    }
    a->pere = pere;
    if (pere == NULL) {
      a = a->pere;
    }
  }
}

void rotationGD(AVL a) {
  if (a != NULL) {
    rotationGauche(a->filsGauche);
    rotationDroite(a);
  }
}

void rotationDG(AVL a) {
  if (a != NULL) {
    rotationDroite(a->filsDroit);
    rotationGauche(a);
  }
}
void updateHeight(AVL arbre, int offset) {
  // Si un des deux fils est nul et que l'on vient d'insérer un noeud alors ça
  // veut dire qu'il n'y avait aucun fils alors il faut mettre à jour la hauteur
  if (arbre != NULL &&
      (arbre->filsDroit == NULL || arbre->filsGauche == NULL)) {
    arbre->hauteur += offset;
    updateHeight(arbre->pere, offset);
  }
}

void insertionABR(AVL *arbre, Arete *val) {
  if (*arbre == NULL) {
    printf("is null ");
    *arbre = newArbre(val, 0, NULL);
    return;
  }

  if (estSuperieureA(val, (*arbre)->val)) {
    if ((*arbre)->filsDroit == NULL) {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsDroit = fils;
      updateHeight((*arbre), 1);
    } else {
      insertionABR(&(*arbre)->filsDroit, val);
    }
  } else {
    if ((*arbre)->filsGauche == NULL) {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsGauche = fils;
      updateHeight((*arbre), 1);
    } else {
      insertionABR(&(*arbre)->filsGauche, val);
    }
  }
}

int deseq(AVL a) {
  if (a != NULL) {
    int hFD = -1;
    int hFG = -1;
    if (a->filsDroit != NULL) {
      hFD = a->filsDroit->hauteur;
    }
    if (a->filsGauche != NULL) {
      hFG = a->filsGauche->hauteur;
    }
    return hFG - hFD;
  } else {
    return -1;
  }
}

void affichageArbre(AVL arbre) {
  if (arbre != NULL) {
    affichageArbre(arbre->filsGauche);
    affichageArbre(arbre->filsDroit);
    printf("%d %d - ", arbre->val->v1, arbre->val->v2);
  }
}

void checkDeseq(AVL a) {
  if (a != NULL) {
    int desequilibrage = deseq(a);
    printf("deseq a - %d %d : %d  \n", a->val->v1, a->val->v2, desequilibrage);
    if (a->filsDroit) {
      printf("h fd : %d \n", a->filsDroit->hauteur);
    }
    if (a->filsGauche) {
      printf("h fg : %d \n", a->filsGauche->hauteur);
    }
    if (desequilibrage >= 2 || desequilibrage <= -2) {
      if (desequilibrage >= 2) {
        if (deseq(a->filsGauche) < 0) {
          printf("rota gd\n");
          printf("hauteur : %d - noeud : %d %d \n", a->hauteur, a->val->v1,
                 a->val->v2);
          rotationGD(a);
        } else {
          printf("rota d \n");
          printf("hauteur : %d - noeud : %d %d \n", a->hauteur, a->val->v1,
                 a->val->v2);
          rotationDroite(a);
        }
      } else {
        if (deseq(a->filsDroit) > 0) {
          printf("rotation dg \n");
          printf("hauteur : %d - noeud : %d %d \n", a->hauteur, a->val->v1,
                 a->val->v2);
          rotationDG(a);
        } else {
          affichageArbre(a);
          printf("\n");
          printf("rota gauche \n");
          printf("hauteur : %d - noeud : %d %d \n", a->hauteur, a->val->v1,
                 a->val->v2);
          rotationGauche(a);
        }
      }
    } else {
      checkDeseq(a->filsDroit);
      checkDeseq(a->filsGauche);
    }
  }
}

void insertion(AVL *a, Arete *val) {
  insertionABR(a, val);
  checkDeseq(*a);
}