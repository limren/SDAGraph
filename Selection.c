#include "headers/Selection.h"

SelectAretes *SAVide()
{
  SelectAretes *sa = malloc(sizeof(SelectAretes));
  sa->aretes = malloc(sizeof(Arete) * 3 * (NB_FACES));
  sa->numAretes = 0;
  return sa;
}

Arete *creationAreteSelection(SelectAretes *sa, int v1, int v2, int numFace)
{
  if ((sa->numAretes % (3 * (NB_FACES))) == 0)
  {
    sa->aretes =
        realloc(sa->aretes, sizeof(Arete) * (sa->numAretes + (3 * (NB_FACES))));
  }
  Arete *a = creationArete(v1, v2, numFace);
  return a;
}

void ajoutAreteSelection(SelectAretes *sa, Face *f, int numFace)
{

  Arete *a = creationAreteSelection(sa, f->v1, f->v2, numFace);
  if (a != NULL)
  {
    sa->aretes[sa->numAretes++] = a;
  }

  Arete *b = creationAreteSelection(sa, f->v1, f->v3, numFace);
  if (b != NULL)
  {
    sa->aretes[sa->numAretes++] = b;
  }

  Arete *c = creationAreteSelection(sa, f->v2, f->v3, numFace);
  if (c != NULL)
  {
    sa->aretes[sa->numAretes++] = c;
  }
}


