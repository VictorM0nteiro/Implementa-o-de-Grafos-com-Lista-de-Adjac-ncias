#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>

typedef struct No {
    int destino;
    float peso;
    struct No* proximo;
} No;

typedef struct {
    char* label;
    No* lista_adjacencia;
} Vertice;

typedef struct {
    Vertice* vertices;
    int num_vertices, capacidade, eh_direcionado, eh_ponderado;
} Grafo;

Grafo* criarGrafo(int eh_direcionado, int eh_ponderado);
void destruirGrafo(Grafo* G);
int insereVertice(Grafo* G, char* label);
int insereAresta(Grafo* G, int u, int v, float w);
int removeAresta(Grafo* G, int u, int v);
int grau(Grafo* G, int v);
float grauMedio(Grafo* G);
int grauMax(Grafo* G, int* v);
float menorCaminhoMedio(Grafo* G);
float assortatividade(Grafo* G);
Grafo* carregarGrafoMTX(const char* arquivo);
void imprimirGrafo(Grafo* G);
int numeroComponentesConexas(Grafo* G);
void dfs(Grafo* G, int v, int* visitado);
float* bfs(Grafo* G, int origem);

#endif