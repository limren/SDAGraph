#include "headers/AVL.h"
#include "headers/tris.h"
#include "headers/creation.h"
AVL newArbre(Arete *val, int hauteur, AVL pere)
{
  AVL nouvelArbre = malloc(sizeof(struct noeud));
  if (nouvelArbre == NULL)
  {
    printf("Erreur lors de l'allocation de mémoire.\n");
    exit(1);
  }
  nouvelArbre->val = val;
  nouvelArbre->pere = pere;
  nouvelArbre->hauteur = hauteur;
  nouvelArbre->filsGauche = NULL;
  nouvelArbre->filsDroit = NULL;
  return nouvelArbre;
}

int hauteur(AVL a)
{
  if (a == NULL)
  {
    return -1; // La hauteur d'un noeud vide est -1
  }
  else
  {
    return a->hauteur;
  }
}

int max(int a, int b) { return (a > b) ? a : b; }

void rotationGauche(AVL *a)
{
  // On récupère le noeud à pivoter
  AVL arbre = *a;
  if (arbre != NULL && arbre->filsDroit != NULL)
  {
    // On récupère les pointeurs des noeuds concernés
    AVL filsDroit = arbre->filsDroit;
    AVL filsGaucheFD = filsDroit->filsGauche;
    AVL pere = arbre->pere;

     // Réarrangement des pointeurs pour effectuer la rotation gauche
    arbre->filsDroit = filsGaucheFD;
    if (filsGaucheFD != NULL)
    {
      filsGaucheFD->pere = arbre;
    }

    filsDroit->filsGauche = arbre;
    arbre->pere = filsDroit;

    // Réajustement de la hauteur des noeuds après rotation
    arbre->hauteur = max(hauteur(arbre->filsGauche), hauteur(arbre->filsDroit)) + 1;
    filsDroit->hauteur = max(hauteur(filsDroit->filsGauche), hauteur(filsDroit->filsDroit)) + 1;

    // Mise à jour du lien du père du noeud pivoté
    if (pere != NULL)
    {
      if (pere->filsGauche == arbre)
      {
        pere->filsGauche = filsDroit;
      }
      else
      {
        // Si le noeud pivoté était la racine, mettez à jour la racine de l'arbre
        pere->filsDroit = filsDroit;
      }
    }
    else
    {
      *a = filsDroit;
    }
    // Mise à jour du lien du père du noeud filsDroit
    filsDroit->pere = pere;
  }
}

void rotationDroite(AVL *a)
{
  // Même principe que rotation gauche
  AVL arbre = *a;
  if (arbre != NULL && arbre->filsGauche != NULL)
  {
    AVL filsGauche = arbre->filsGauche;
    AVL filsDroitFG = filsGauche->filsDroit;
    AVL pere = arbre->pere;

    arbre->filsGauche = filsDroitFG;
    if (filsDroitFG != NULL)
    {
      filsDroitFG->pere = arbre;
    }

    filsGauche->filsDroit = arbre;
    arbre->pere = filsGauche;

    arbre->hauteur = max(hauteur(arbre->filsGauche), hauteur(arbre->filsDroit)) + 1;
    filsGauche->hauteur = max(hauteur(filsGauche->filsGauche), hauteur(filsGauche->filsDroit)) + 1;

    if (pere != NULL)
    {
      if (pere->filsGauche == arbre)
      {
        pere->filsGauche = filsGauche;
      }
      else
      {
        pere->filsDroit = filsGauche;
      }
    }
    else
    {
      *a = filsGauche;
    }
    filsGauche->pere = pere;
  }
}

void rotationGD(AVL *a)
{
  if (*a != NULL)
  {
    rotationGauche(&(*a)->filsGauche);
    rotationDroite(a);
  }
}

void rotationDG(AVL *a)
{
  if (*a != NULL)
  {
    rotationDroite(&(*a)->filsDroit);
    rotationGauche(a);
  }
}

void updateHeight(AVL *arbre)
{
  if (*arbre != NULL)
  {
    int hFG = ((*arbre)->filsGauche != NULL) ? (*arbre)->filsGauche->hauteur : -1;
    int hFD = ((*arbre)->filsDroit != NULL) ? (*arbre)->filsDroit->hauteur : -1;
    // La hauteur du père est basé sur la plus grande de ses fils (droit et gauche en l'occurence)
    (*arbre)->hauteur = max(hFG, hFD) + 1;
    // On doit ajuster la hauteur des pères puisque la hauteur de ses fils a changé
    updateHeight(&(*arbre)->pere);
  }
}

void insertionAVL(AVL *arbre, Arete *val, GrapheDuale * gd, Graphe * graphe)
{
  if (*arbre == NULL)
  {
    *arbre = newArbre(val, 0, NULL);
    return;
  }
  // Si l'arête est équivalente à une autre, on ne l'insère pas dans l'arbre et on la traite comme si elle était déjà présente
  if(sontEquilaventes(val, (*arbre)->val))
  {
    // Ajout de l'arête dans le graphe duale
    ajoutAreteDuale(val, (*arbre)->val, gd, graphe);
    // On ne l'insère pas dans l'arbre, son équivalent est déjà présent (évite d'avoir un arbre trop complexe)
    return;
  }
  // Insertion comme dans un ABR, or, ici, on ajuste la hauteur et on regarde les déséquilibres afin de limiter la hauteur de l'arbre
  if (estSuperieureA(val, (*arbre)->val))
  {
    // Si fils droit libre, alors on insère l'arête
    if ((*arbre)->filsDroit == NULL)
    {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsDroit = fils;
      updateHeight(arbre);
      checkDeseq(&(*arbre)->filsDroit);
    }
    else
    {
      insertionAVL(&(*arbre)->filsDroit, val, gd, graphe);
    }
  }
  else
  {
    // Si fils gauche libre, alors on insère l'arête
    if ((*arbre)->filsGauche == NULL)
    {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsGauche = fils;
      updateHeight(arbre);
      checkDeseq(&(*arbre)->filsGauche);
    }
    else
    {
      insertionAVL(&(*arbre)->filsGauche, val, gd, graphe);
    }
  }
}

int deseq(AVL a)
{
  if (a != NULL)
  {
    int hFD = -1;
    int hFG = -1;
    if (a->filsDroit != NULL)
    {
      hFD = a->filsDroit->hauteur;
    }
    if (a->filsGauche != NULL)
    {
      hFG = a->filsGauche->hauteur;
    }
    return hFG - hFD;
  }
  else
  {
    return -1;
  }
}

void affichageArbre(AVL arbre)
{
  if (arbre != NULL)
  {
    printf("%d %d - ", arbre->val->v1, arbre->val->v2);
    affichageArbre(arbre->filsGauche);
    affichageArbre(arbre->filsDroit);
  }
}

void addAVLAretes(AVL *avl, Face *f, int numFace, GrapheDuale * gd, Graphe * graphe)
{
  Arete *a = creationArete(f->v1, f->v2, numFace);
  if (a != NULL)
  {
    insertionAVL(avl, a, gd, graphe);
  }

  Arete *b = creationArete(f->v1, f->v3, numFace);
  if (b != NULL)
  {
    insertionAVL(avl, b, gd, graphe);
  }

  Arete *c = creationArete(f->v2, f->v3, numFace);
  if (c != NULL)
  {
    insertionAVL(avl, c, gd, graphe);
  }
}

void geneADuales(Maillage *m, AVL *arbre, GrapheDuale * gd, Graphe * graphe)
{
  if (arbre == NULL)
  {
    arbre = malloc(sizeof(AVL));
  }

  for (int i = 0; i < m->numFaces; i++)
  {
    addAVLAretes(arbre, m->faces[i], i, gd, graphe);
  }
}

void checkDeseq(AVL * a)
{
  if (a != NULL)
  {
    /* 
      Calcul du déséquilibre du noeud courant et rotation si nécessaire
      Si déséquilibre >= 2 ou <= -2, alors on doit faire une rotation comme vu en cours
      C'est-à-dire :
        - Si déséquilibre >= 2, alors on regarde le déséquilibre du fils gauche
          - Si déséquilibre du fils gauche < 0, alors on fait une rotation gauche-droite
          - Sinon, on fait une rotation droite
        - Si déséquilibre <= -2, alors on regarde le déséquilibre du fils droit
          - Si déséquilibre du fils droit > 0, alors on fait une rotation droite-gauche
          - Sinon, on fait une rotation gauche
      Ensuite, on vérifie bien que les pères sont équilibrés aussi, sinon on recommence
    */
    int desequilibrage = deseq(*a);
    if (desequilibrage >= 2 || desequilibrage <= -2)
    {
      if (desequilibrage >= 2)
      {
        if (deseq((*a)->filsGauche) < 0)
        {
          rotationGD(a);
        }
        else
        {
          rotationDroite(a);
        }
      }
      else
      {
        if (deseq((*a)->filsDroit) > 0)
        {
          rotationDG(a);
        }
        else
        {
          rotationGauche(a);
        }
      }
    }
    else
    {
      if ((*a)->pere != NULL) {
        checkDeseq(&((*a)->pere));
      }
    }
  }
}


void freeAVL(AVL *a)
{
  if (*a != NULL)
  {
    freeAVL(&(*a)->filsGauche);
    freeAVL(&(*a)->filsDroit);
    free(*a);
  }
}