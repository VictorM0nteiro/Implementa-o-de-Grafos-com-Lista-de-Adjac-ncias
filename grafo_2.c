#include "grafo.h"
#include <math.h>

Grafo* criarGrafo(int eh_direcionado, int eh_ponderado) {
    Grafo* G = (Grafo*)malloc(sizeof(Grafo));
    if (!G) return NULL;
    
    G->capacidade = 10;
    G->num_vertices = 0;
    G->eh_direcionado = eh_direcionado;
    G->eh_ponderado = eh_ponderado;
    G->vertices = (Vertice*)calloc(G->capacidade, sizeof(Vertice));
    
    if (!G->vertices) {
        free(G);
        return NULL;
    }
    return G;
}

void destruirGrafo(Grafo* G) {
    if (!G) return;
    
    for (int i = 0; i < G->num_vertices; i++) {
        No* atual = G->vertices[i].lista_adjacencia;
        while (atual) {
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
    

    for (int i = 0; i < G->num_vertices; i++) {
        if (G->vertices[i].label && strcmp(G->vertices[i].label, label) == 0) {
            return 0;
        }
    }
    
    if (G->num_vertices >= G->capacidade) {
        int nova_capacidade = G->capacidade + (G->capacidade / 2);
        Vertice* novo_array = (Vertice*)realloc(G->vertices, nova_capacidade * sizeof(Vertice));
        if (!novo_array) return 0;
        
        G->vertices = novo_array;
        G->capacidade = nova_capacidade;
        
        for (int i = G->num_vertices; i < nova_capacidade; i++) {
            G->vertices[i].label = NULL;
            G->vertices[i].lista_adjacencia = NULL;
        }
    }
    
    G->vertices[G->num_vertices].label = strdup(label);
    G->vertices[G->num_vertices].lista_adjacencia = NULL;
    G->num_vertices++;
    
    return 1;
}

int insereAresta(Grafo* G, int u, int v, float w) {
    if (!G || u < 0 || v < 0 || u >= G->num_vertices || v >= G->num_vertices) return 0;
    
    No* atual = G->vertices[u].lista_adjacencia;
    while (atual) {
        if (atual->destino == v) return 0; 
        atual = atual->proximo;
    }
    
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) return 0;
    
    novo->destino = v;
    novo->peso = w;
    novo->proximo = G->vertices[u].lista_adjacencia;
    G->vertices[u].lista_adjacencia = novo;
    
    if (!G->eh_direcionado && u != v) {
        atual = G->vertices[v].lista_adjacencia;
        while (atual) {
            if (atual->destino == u) break; // Aresta já existe
            atual = atual->proximo;
        }
        
        if (!atual) {
            No* novo2 = (No*)malloc(sizeof(No));
            if (!novo2) return 0;
            
            novo2->destino = u;
            novo2->peso = w;
            novo2->proximo = G->vertices[v].lista_adjacencia;
            G->vertices[v].lista_adjacencia = novo2;
        }
    }
    
    return 1;
}

int removeAresta(Grafo* G, int u, int v) {
    if (!G || u < 0 || v < 0 || u >= G->num_vertices || v >= G->num_vertices) return 0;
    
    int removida = 0;
    No* atual = G->vertices[u].lista_adjacencia;
    No* anterior = NULL;
    
    while (atual) {
        if (atual->destino == v) {
            if (anterior) {
                anterior->proximo = atual->proximo;
            } else {
                G->vertices[u].lista_adjacencia = atual->proximo;
            }
            free(atual);
            removida = 1;
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (!G->eh_direcionado && u != v) {
        atual = G->vertices[v].lista_adjacencia;
        anterior = NULL;
        
        while (atual) {
            if (atual->destino == u) {
                if (anterior) {
                    anterior->proximo = atual->proximo;
                } else {
                    G->vertices[v].lista_adjacencia = atual->proximo;
                }
                free(atual);
                break;
            }
            anterior = atual;
            atual = atual->proximo;
        }
    }
    
    return removida;
}

int grau(Grafo* G, int v) {
    if (!G || v < 0 || v >= G->num_vertices) return -1;
    
    int grau_vertice = 0;
    No* atual = G->vertices[v].lista_adjacencia;
    
    while (atual) {
        grau_vertice++;
        atual = atual->proximo;
    }
    
    return grau_vertice;
}

float grauMedio(Grafo* G) {
    if (!G || G->num_vertices == 0) return -1;
    
    int soma_graus = 0;
    for (int i = 0; i < G->num_vertices; i++) {
        soma_graus += grau(G, i);
    }
    return (float)soma_graus / G->num_vertices;
}

int grauMax(Grafo* G, int* v) {
    if (!G || !v || G->num_vertices == 0) return -1;
    
    int max_grau = -1;
    *v = -1;
    
    for (int i = 0; i < G->num_vertices; i++) {
        int grau_vertice = grau(G, i);
        if (grau_vertice > max_grau) {
            max_grau = grau_vertice;
            *v = i;
        }
    }
    
    return max_grau;
}

float* bfs(Grafo* G, int origem) {
    if (!G || origem < 0 || origem >= G->num_vertices) return NULL;

    // 1. Inicialização
    float* distancia = (float*)malloc(G->num_vertices * sizeof(float));
    if (!distancia) return NULL;

    for (int i = 0; i < G->num_vertices; i++) {
        distancia[i] = FLT_MAX; // Usamos FLT_MAX para indicar "infinito" ou "não alcançável"
    }
    distancia[origem] = 0;

    int* fila = (int*)malloc(G->num_vertices * sizeof(int));
    if (!fila) {
        free(distancia);
        return NULL;
    }
    int cabeca = 0;
    int cauda = 0;

    fila[cauda++] = origem;

    while (cabeca < cauda) {
        int u = fila[cabeca++];

        No* vizinho = G->vertices[u].lista_adjacencia;
        while (vizinho) {
            int v = vizinho->destino;
            
            if (distancia[v] == FLT_MAX) {
                distancia[v] = distancia[u] + 1;
                fila[cauda++] = v; // Enfileira o vizinho
            }
            vizinho = vizinho->proximo;
        }
    }

    free(fila);
    return distancia;
}


float menorCaminhoMedio(Grafo* G) {
    if (!G || G->num_vertices <= 1) return 0.0;

    double soma_distancias = 0.0;    
    long long num_pares = 0;

    for (int i = 0; i < G->num_vertices; i++) {
        float* dists = bfs(G, i);
        if (!dists) {
            printf("Erro: Falha de alocacao de memoria na BFS.\n");
            return -1; // Indica um erro
        }
        for (int j = i + 1; j < G->num_vertices; j++) {
            if (dists[j] != FLT_MAX) {
                soma_distancias += dists[j];
                num_pares++;
            }
        }
        
        free(dists);
    }
    return (num_pares > 0) ? (float)(soma_distancias / num_pares) : 0.0;
}

float assortatividade(Grafo* G) {
    if (!G || G->num_vertices == 0) return 0;
    
    int num_arestas = 0;
    float soma_xy = 0, soma_x = 0, soma_y = 0, soma_x2 = 0, soma_y2 = 0;
    
    for (int i = 0; i < G->num_vertices; i++) {
        No* atual = G->vertices[i].lista_adjacencia;
        int grau_i = grau(G, i);
        
        while (atual) {
            int j = atual->destino;
            int grau_j = grau(G, j);
            
            if (!G->eh_direcionado && i > j) {
                atual = atual->proximo;
                continue;
            }
            
            soma_xy += grau_i * grau_j;
            soma_x += grau_i;
            soma_y += grau_j;
            soma_x2 += grau_i * grau_i;
            soma_y2 += grau_j * grau_j;
            num_arestas++;
            
            atual = atual->proximo;
        }
    }
    
    if (num_arestas == 0) return 0;
    
    float media_x = soma_x / num_arestas;
    float media_y = soma_y / num_arestas;
    float media_xy = soma_xy / num_arestas;
    float var_x = (soma_x2 / num_arestas) - (media_x * media_x);
    float var_y = (soma_y2 / num_arestas) - (media_y * media_y);
    
    if (var_x == 0 || var_y == 0) return 0;
    
    return (media_xy - media_x * media_y) / sqrt(var_x * var_y);
}

void dfs(Grafo* G, int v, int* visitado) {
    visitado[v] = 1;
    
    No* atual = G->vertices[v].lista_adjacencia;
    while (atual) {
        if (!visitado[atual->destino]) {
            dfs(G, atual->destino, visitado);
        }
        atual = atual->proximo;
    }
}


int numeroComponentesConexas(Grafo* G) {
    if (!G || G->num_vertices == 0) return 0;
    
    int* visitado = (int*)calloc(G->num_vertices, sizeof(int));
    if (!visitado) return -1;
    
    int componentes = 0;
    
    for (int i = 0; i < G->num_vertices; i++) {
        if (!visitado[i]) {
            dfs(G, i, visitado);
            componentes++;
        }
    }
    free(visitado);
    return componentes;
}

Grafo* carregarGrafoMTX(const char* arquivo) {
    FILE* file = fopen(arquivo, "r");
    if (!file) return NULL;
    
    char linha[256];
    int num_vertices, num_arestas;
    int eh_simetrico = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        if (linha[0] == '%') {
            if (strstr(linha, "symmetric")) {
                eh_simetrico = 1;
            }
            continue;
        }
        
        if (linha[0] != '%') {
            sscanf(linha, "%d %d %d", &num_vertices, &num_vertices, &num_arestas);
            break;
        }
    }
    
    Grafo* G = criarGrafo(!eh_simetrico, 0);
    if (!G) {
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < num_vertices; i++) {
        char label[32];
        sprintf(label, "%d", i + 1);
        insereVertice(G, label);
    }
    
    int u, v;
    while (fscanf(file, "%d %d", &u, &v) == 2) {
        insereAresta(G, u - 1, v - 1, 1.0);
    }
    fclose(file);
    return G;
}

void imprimirGrafo(Grafo* G) {
    if (!G) return;
    
    printf("Grafo com %d vertices\n", G->num_vertices);
    printf("Tipo: %s\n", G->eh_direcionado ? "Direcionado" : "Nao direcionado");
    printf("Ponderado: %s\n", G->eh_ponderado ? "Sim" : "Nao");
    
    for (int i = 0; i < G->num_vertices; i++) {
        printf("Vertice %d (%s): ", i, G->vertices[i].label ? G->vertices[i].label : "sem label");
        
        No* atual = G->vertices[i].lista_adjacencia;
        while (atual) {
            printf("-> %d", atual->destino);
            if (G->eh_ponderado) {
                printf("(%.2f)", atual->peso);
            }
            atual = atual->proximo;
        }
        printf("\n");
    }
} 