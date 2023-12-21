#pragma once

struct Vertex
{
  // Coordonnées des sommets
  float x;
  float y;
  float z;
};

typedef struct Vertex Vertex;

struct Face
{
  // On conserve l'indice des vertex
  int v1;
  int v2;
  int v3;
};

typedef struct Face Face;

struct Arete
{
  int v1;
  int v2;
  int indexFace;
};

typedef struct Arete Arete;

struct Maillage
{
  int numSommets;
  Vertex **sommets;

  int numFaces;
  Face **faces;
};

struct SelectAretes
{
  int numAretes;
  Arete **aretes;
};

typedef struct SelectAretes SelectAretes;
// struct AVLAretes{};

typedef struct Maillage Maillage;

typedef Vertex Centroide;

struct AreteDuale
{
  int indiceC1;
  int indiceC2;
};

typedef struct AreteDuale AreteDuale;

struct GrapheDuale
{
  int numCentroides;
  Centroide **centroides;
  int numAretesDuales;
  AreteDuale **aretesDuales;
};

typedef struct GrapheDuale GrapheDuale;
struct HeapAretes
{
  int capacite;
  int noeudsAlloues;
  int numNoeuds;
  Arete **T;
};

typedef struct HeapAretes HeapAretes;
