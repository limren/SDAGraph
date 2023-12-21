#include "AVL.h"
#include "tris.h"
#include "creation.h"
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
  AVL arbre = *a;
  if (arbre != NULL && arbre->filsDroit != NULL)
  {
    AVL filsDroit = arbre->filsDroit;
    AVL filsGaucheFD = filsDroit->filsGauche;
    AVL pere = arbre->pere;

    arbre->filsDroit = filsGaucheFD;
    if (filsGaucheFD != NULL)
    {
      filsGaucheFD->pere = arbre;
    }

    filsDroit->filsGauche = arbre;
    arbre->pere = filsDroit;

    arbre->hauteur = max(hauteur(arbre->filsGauche), hauteur(arbre->filsDroit)) + 1;
    filsDroit->hauteur = max(hauteur(filsDroit->filsGauche), hauteur(filsDroit->filsDroit)) + 1;

    if (pere != NULL)
    {
      if (pere->filsGauche == arbre)
      {
        pere->filsGauche = filsDroit;
      }
      else
      {
        pere->filsDroit = filsDroit;
      }
    }
    else
    {
      *a = filsDroit;
    }
    filsDroit->pere = pere;
  }
}
void rotationDroite(AVL *a)
{
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

void updateHeight(AVL arbre)
{
  if (arbre != NULL)
  {
    int hFG = (arbre->filsGauche != NULL) ? arbre->filsGauche->hauteur : -1;
    int hFD = (arbre->filsDroit != NULL) ? arbre->filsDroit->hauteur : -1;

    arbre->hauteur = max(hFG, hFD) + 1;
    updateHeight(arbre->pere);
  }
}

// void updateHeight(AVL arbre)
// {
//   AVL current = arbre;
//   while (current != NULL)
//   {
//     int hFG = (current->filsGauche != NULL) ? current->filsGauche->hauteur : -1;
//     int hFD = (current->filsDroit != NULL) ? current->filsDroit->hauteur : -1;

//     current->hauteur = max(hFG, hFD) + 1;
//     current = current->pere;
//   }
// }


void insertionAVL(AVL *arbre, Arete *val)
{
  if (*arbre == NULL)
  {
    *arbre = newArbre(val, 0, NULL);
    return;
  }

  if (estSuperieureA(val, (*arbre)->val))
  {
    if ((*arbre)->filsDroit == NULL)
    {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsDroit = fils;
      updateHeight((*arbre));
      checkDeseq(&fils);
    }
    else
    {
      insertionAVL(&(*arbre)->filsDroit, val);
    }
  }
  else
  {
    if ((*arbre)->filsGauche == NULL)
    {
      AVL fils = newArbre(val, 0, *arbre);
      (*arbre)->filsGauche = fils;
      updateHeight((*arbre));
      checkDeseq(&fils);
    }
    else
    {
      insertionAVL(&(*arbre)->filsGauche, val);
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

void addAVLAretes(AVL *avl, Face *f, int numFace)
{
  Arete *a = creationArete(f->v1, f->v2, numFace);
  if (a != NULL)
  {
    insertionAVL(avl, a);
  }

  Arete *b = creationArete(f->v1, f->v3, numFace);
  if (b != NULL)
  {
    insertionAVL(avl, b);
  }

  Arete *c = creationArete(f->v2, f->v3, numFace);
  if (c != NULL)
  {
    insertionAVL(avl, c);
  }
}

void parcoursAVL(AVL arbre)
{
  if (arbre->filsDroit != NULL && arbre->filsGauche != NULL)
  {
    if (sontEquilaventes(arbre->val, arbre->filsGauche->val) || sontEquilaventes(arbre->val, arbre->filsDroit->val))
    {
      printf("good");
    }
    parcoursAVL(arbre->filsDroit);
    parcoursAVL(arbre->filsGauche);
  }
}

void geneADuales(Maillage *m, AVL *arbre)
{
  if (arbre == NULL)
  {
    arbre = malloc(sizeof(AVL));
  }

  for (int i = 0; i < m->numFaces; i++)
  {
    addAVLAretes(arbre, m->faces[i], i);
  }
}

void checkDeseq(AVL * a)
{
  if (a != NULL)
  {
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

