#include "tris.h"

struct tas {
  int capacite;
  int noeudsAlloues;
  Arete **T;
};

typedef struct tas tas;

double sommeIndex(Arete *a) { return a->v1 + a->v2; }

int estSuperieureA(Arete *a, Arete *b) {
  double coordsA = sommeIndex(a);
  double coordsB = sommeIndex(b);
  if (coordsA > coordsB) {
    return 1;
  }
  return 0;
}

void echange(Arete **a, int i, int j) {
  Arete *tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void triSelection(Arete **a, int size) {
  for (int i = size - 1; i > 0; i--) {
    int indiceMax = 0;
    for (int j = 1; j <= i; j++) {
      if (estSuperieureA(a[j], a[i])) {
        indiceMax = j;
      }
    }
    echange(a, indiceMax, i);
  }
}

int filsGauche(tas *t, int i) { return 2 * i + 1; }
int filsDroit(tas *t, int i) { return 2 * i + 2; }
int pere(tas *t, int i) { return (i - 1) / 2; }

void insertionTas(tas *t, int val) {
  if (t->capacite <= t->noeudsAlloues) {
    printf("Votre heap est pleine \n");
    return;
  }
  int i = t->noeudsAlloues;
  t->T[t->noeudsAlloues] = val;
  while (i > 0 && estSuperieureA(val, t->T[pere(t, i)])) {
    Arete *tmp = t->T[pere(t, i)];
    t->T[(i - 1) / 2] = val;
    t->T[i] = tmp;
    i = (i - 1) / 2;
  }
  t->noeudsAlloues++;
}

void afficherTas(tas *t) {
  for (int i = 0; i < t->noeudsAlloues; i++) {
    printf("%d ", t->T[i]);
  }
  printf("\n");
}

void supprimerMax(tas *t) {
  printf("Max : %d \n", t->T[0]);
  int i = t->noeudsAlloues - 1;
  t->T[0] = t->T[i];
  i = 0;
  while (2 * i + 2 < t->noeudsAlloues &&
         (estSuperieureA(t->T[i], t->T[filsGauche(t, i)]) ||
          t->T[i] < t->T[filsDroit(t, i)])) {
    if (estSuperieureA(t->T[filsGauche(t, i)], t->T[filsDroit(t, i)])) {
      Arete *tmp = t->T[2 * i + 1];
      t->T[2 * i + 1] = t->T[i];
      t->T[i] = tmp;
      i = 2 * i + 1;
    } else {
      Arete *tmp = t->T[2 * i + 2];
      t->T[2 * i + 2] = t->T[i];
      t->T[i] = tmp;
      i = 2 * i + 2;
    }
  }
  t->noeudsAlloues--;
  afficherTas(t);
}
