
#include "grafo.h"
#include <time.h>

int main() {
    printf("=== Analise da Rede Inf-Power ===\n\n");
    printf("Carregando grafo do arquivo inf-power.mtx...\n");
    Grafo* G = carregarGrafoMTX("inf-power/inf-power.mtx");
    
    if (!G) {
        printf("Erro ao carregar o grafo!\n");
        return 1;
    }
    
    printf("Grafo carregado com sucesso!\n\n");

    printf("=== Informacoes Basicas ===\n");
    printf("Numero de vertices: %d\n", G->num_vertices);
    printf("Tipo: %s\n", G->eh_direcionado ? "Direcionado" : "Nao direcionado");
    printf("Ponderado: %s\n", G->eh_ponderado ? "Sim" : "Nao");
    
    int num_arestas = 0;
    for (int i = 0; i < G->num_vertices; i++) {
        No* atual = G->vertices[i].lista_adjacencia;
        while (atual) {
            num_arestas++;
            atual = atual->proximo;
        }
    }
    if (!G->eh_direcionado) {
        num_arestas /= 2;
    }
    printf("Numero de arestas: %d\n", num_arestas);
    printf("\n=== Componentes Conexas ===\n");
    int componentes = numeroComponentesConexas(G);
    printf("Numero de componentes conexas: %d\n", componentes);
    
    printf("\n=== Analise de Graus ===\n");
    float grau_medio = grauMedio(G);
    printf("Grau medio: %.2f\n", grau_medio);
    
    int vertice_max_grau;
    int max_grau = grauMax(G, &vertice_max_grau);
    printf("Maior grau: %d (vertice %d)\n", max_grau, vertice_max_grau);

    printf("\n=== Menor Caminho Medio ===\n");

    printf("Calculando menor caminho medio (usando BFS para cada vertice)...\n"); 
    clock_t inicio = clock();
    float menor_caminho = menorCaminhoMedio(G);
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    if (menor_caminho >= 0){
        printf("Menor caminho medio: %.2f\n", menor_caminho);
    } else {
        printf("Nao foi possivel calcular o menor caminho medio (grafo desconexo ou erro)\n");
    }
    printf("Tempo de calculo: %.2f segundos\n", tempo);

    printf("\n=== Assortatividade ===\n");
    printf("Calculando assortatividade...\n");
    inicio = clock();
    float ass = assortatividade(G);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Assortatividade: %.4f\n", ass);
    if (ass > 0.1) {
        printf("Interpretacao: Rede assortativa (vertices de grau similar tendem a se conectar)\n");
    } else if (ass < -0.1) {
        printf("Interpretacao: Rede disassortativa (vertices de grau diferente tendem a se conectar)\n");
    } else {
        printf("Interpretacao: Rede neutra (sem correlacao significativa entre graus)\n");
    }
    printf("Tempo de calculo: %.2f segundos\n", tempo);
    
    printf("\n=== Estatisticas Adicionais ===\n");
    
    int* distribuicao_graus = (int*)calloc(max_grau + 1, sizeof(int));
    for (int i = 0; i < G->num_vertices; i++) {
        int g = grau(G, i);
        if (g >= 0) {
            distribuicao_graus[g]++;
        }
    }
    
    printf("Distribuicao de graus (primeiros 10):\n");
    for (int i = 0; i <= 10 && i <= max_grau; i++) {
        printf("  Grau %d: %d vertices\n", i, distribuicao_graus[i]);
    }
    
    printf("Vertices isolados (grau 0): %d\n", distribuicao_graus[0]);
    
    printf("Vertices folha (grau 1): %d\n", distribuicao_graus[1]);
    
    free(distribuicao_graus);
    
    float densidade = (float)(2 * num_arestas) / (G->num_vertices * (G->num_vertices - 1));
    printf("Densidade do grafo: %.6f\n", densidade);
    
    printf("\n=== Limpeza ===\n");
    destruirGrafo(G);
    printf("Memoria liberada com sucesso!\n");
    
    return 0;
} 