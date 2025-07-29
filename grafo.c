#include "grafo.h"

Grafo* criarGrafo(int eh_direcionado, int eh_ponderado) {
    Grafo* G = malloc(sizeof(Grafo));
    if (!G) return NULL;
    *G = (Grafo){calloc(10, sizeof(Vertice)), 0, 10, eh_direcionado, eh_ponderado};
    return G->vertices ? G : (free(G), NULL);
}

void destruirGrafo(Grafo* G) {
    if (!G) return;
    for (int i = 0; i < G->num_vertices; i++) {
        for (No* atual = G->vertices[i].lista_adjacencia; atual;) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        free(G->vertices[i].label);
    }
    free(G->vertices);
    free(G);
}

int insereVertice(Grafo* G, char* label) {
    if (!G || !label) return 0;
    for (int i = 0; i < G->num_vertices; i++)
        if (G->vertices[i].label && !strcmp(G->vertices[i].label, label)) return 0;
    
    if (G->num_vertices >= G->capacidade) {
        int nova = G->capacidade + G->capacidade/2;
        Vertice* novo = realloc(G->vertices, nova * sizeof(Vertice));
        if (!novo) return 0;
        G->vertices = novo;
        G->capacidade = nova;
        for (int i = G->num_vertices; i < nova; i++)
            G->vertices[i] = (Vertice){NULL, NULL};
    }
    G->vertices[G->num_vertices] = (Vertice){strdup(label), NULL};
    return ++G->num_vertices, 1;
}

int insereAresta(Grafo* G, int u, int v, float w) {
    if (!G || u < 0 || v < 0 || u >= G->num_vertices || v >= G->num_vertices) return 0;
    
    for (No* atual = G->vertices[u].lista_adjacencia; atual; atual = atual->proximo)
        if (atual->destino == v) return 0;
    
    No* novo = malloc(sizeof(No));
    if (!novo) return 0;
    *novo = (No){v, w, G->vertices[u].lista_adjacencia};
    G->vertices[u].lista_adjacencia = novo;
    
    if (!G->eh_direcionado && u != v) {
        for (No* atual = G->vertices[v].lista_adjacencia; atual; atual = atual->proximo)
            if (atual->destino == u) return 1;
        No* novo2 = malloc(sizeof(No));
        if (!novo2) return 1;
        *novo2 = (No){u, w, G->vertices[v].lista_adjacencia};
        G->vertices[v].lista_adjacencia = novo2;
    }
    return 1;
}

int removeAresta(Grafo* G, int u, int v) {
    if (!G || u < 0 || v < 0 || u >= G->num_vertices || v >= G->num_vertices) return 0;
    
    int removida = 0;
    for (No **ptr = &G->vertices[u].lista_adjacencia; *ptr; ptr = &(*ptr)->proximo) {
        if ((*ptr)->destino == v) {
            No* temp = *ptr;
            *ptr = (*ptr)->proximo;
            free(temp);
            removida = 1;
            break;
        }
    }
    
    if (!G->eh_direcionado && u != v) {
        for (No **ptr = &G->vertices[v].lista_adjacencia; *ptr; ptr = &(*ptr)->proximo) {
            if ((*ptr)->destino == u) {
                No* temp = *ptr;
                *ptr = (*ptr)->proximo;
                free(temp);
                break;
            }
        }
    }
    return removida;
}

int grau(Grafo* G, int v) {
    if (!G || v < 0 || v >= G->num_vertices) return -1;
    int g = 0;
    for (No* atual = G->vertices[v].lista_adjacencia; atual; atual = atual->proximo) g++;
    return g;
}

float grauMedio(Grafo* G) {
    if (!G || !G->num_vertices) return -1;
    int soma = 0;
    for (int i = 0; i < G->num_vertices; i++) soma += grau(G, i);
    return (float)soma / G->num_vertices;
}

int grauMax(Grafo* G, int* v) {
    if (!G || !v || !G->num_vertices) return -1;
    int max = -1;
    *v = -1;
    for (int i = 0; i < G->num_vertices; i++) {
        int g = grau(G, i);
        if (g > max) max = g, *v = i;
    }
    return max;
}

float* bfs(Grafo* G, int origem) {
    if (!G || origem < 0 || origem >= G->num_vertices) return NULL;
    
    float* dist = malloc(G->num_vertices * sizeof(float));
    int* fila = malloc(G->num_vertices * sizeof(int));
    if (!dist || !fila) return free(dist), free(fila), NULL;
    
    for (int i = 0; i < G->num_vertices; i++) dist[i] = FLT_MAX;
    dist[origem] = 0;
    
    int inicio = 0, fim = 0;
    fila[fim++] = origem;
    
    while (inicio < fim) {
        int u = fila[inicio++];
        for (No* atual = G->vertices[u].lista_adjacencia; atual; atual = atual->proximo) {
            int v = atual->destino;
            if (dist[v] == FLT_MAX) {
                dist[v] = dist[u] + 1;
                fila[fim++] = v;
            }
        }
    }
    return free(fila), dist;
}

float menorCaminhoMedio(Grafo* G) {
    if (!G || G->num_vertices <= 1) return 0.0;
    
    double soma = 0.0;
    long long pares = 0;
    
    for (int i = 0; i < G->num_vertices; i++) {
        float* dists = bfs(G, i);
        if (!dists) return -1;
        for (int j = i + 1; j < G->num_vertices; j++)
            if (dists[j] != FLT_MAX) soma += dists[j], pares++;
        free(dists);
    }
    return pares > 0 ? (float)(soma / pares) : 0.0;
}

float assortatividade(Grafo* G) {
    if (!G || !G->num_vertices) return 0;
    
    int num = 0;
    float sxy = 0, sx = 0, sy = 0, sx2 = 0, sy2 = 0;
    
    for (int i = 0; i < G->num_vertices; i++) {
        int gi = grau(G, i);
        for (No* atual = G->vertices[i].lista_adjacencia; atual; atual = atual->proximo) {
            int j = atual->destino, gj = grau(G, j);
            if (!G->eh_direcionado && i > j) continue;
            sxy += gi * gj; sx += gi; sy += gj;
            sx2 += gi * gi; sy2 += gj * gj; num++;
        }
    }
    
    if (!num) return 0;
    float mx = sx/num, my = sy/num, vx = sx2/num - mx*mx, vy = sy2/num - my*my;
    return (vx == 0 || vy == 0) ? 0 : (sxy/num - mx*my) / sqrt(vx * vy);
}

void dfs(Grafo* G, int v, int* visitado) {
    visitado[v] = 1;
    for (No* atual = G->vertices[v].lista_adjacencia; atual; atual = atual->proximo)
        if (!visitado[atual->destino]) dfs(G, atual->destino, visitado);
}

int numeroComponentesConexas(Grafo* G) {
    if (!G || !G->num_vertices) return 0;
    int* visitado = calloc(G->num_vertices, sizeof(int));
    if (!visitado) return -1;
    int comp = 0;
    for (int i = 0; i < G->num_vertices; i++)
        if (!visitado[i]) dfs(G, i, visitado), comp++;
    return free(visitado), comp;
}

Grafo* carregarGrafoMTX(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) return NULL;
    
    char linha[256];
    int nv, na, simetrico = 0;
    
    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == '%') {
            if (strstr(linha, "symmetric")) simetrico = 1;
            continue;
        }
        sscanf(linha, "%d %d %d", &nv, &nv, &na);
        break;
    }
    
    Grafo* G = criarGrafo(!simetrico, 0);
    if (!G) return fclose(f), NULL;
    
    for (int i = 0; i < nv; i++) {
        char label[32];
        sprintf(label, "%d", i + 1);
        insereVertice(G, label);
    }
    
    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2)
        insereAresta(G, u - 1, v - 1, 1.0);
    
    return fclose(f), G;
}

void imprimirGrafo(Grafo* G) {
    if (!G) return;
    printf("Grafo com %d vertices\nTipo: %s\nPonderado: %s\n",
           G->num_vertices, G->eh_direcionado ? "Direcionado" : "Nao direcionado",
           G->eh_ponderado ? "Sim" : "Nao");
    
    for (int i = 0; i < G->num_vertices; i++) {
        printf("Vertice %d (%s): ", i, G->vertices[i].label ?: "sem label");
        for (No* atual = G->vertices[i].lista_adjacencia; atual; atual = atual->proximo) {
            printf("-> %d", atual->destino);
            if (G->eh_ponderado) printf("(%.2f)", atual->peso);
        }
        printf("\n");
    }
}