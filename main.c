#include "grafo.h"

int main() {
    printf("=== Analise da Rede Inf-Power ===\n\nCarregando grafo do arquivo inf-power.mtx...\n");
    Grafo* G = carregarGrafoMTX("inf-power.mtx");
    
    if (!G) return printf("Erro ao carregar o grafo!\n"), 1;
    printf("Grafo carregado com sucesso!\n\n");
    
    printf("=== Informacoes Basicas ===\n");
    printf("Numero de vertices: %d\nTipo: %s\nPonderado: %s\n",
           G->num_vertices, G->eh_direcionado ? "Direcionado" : "Nao direcionado",
           G->eh_ponderado ? "Sim" : "Nao");

    int arestas = 0;
    for (int i = 0; i < G->num_vertices; i++)
        for (No* atual = G->vertices[i].lista_adjacencia; atual; atual = atual->proximo)
            arestas++;
    if (!G->eh_direcionado) arestas /= 2;
    printf("Numero de arestas: %d\n", arestas);

    printf("\n=== Componentes Conexas ===\nNumero de componentes conexas: %d\n",
           numeroComponentesConexas(G));

    printf("\n=== Analise de Graus ===\n");
    printf("Grau medio: %.2f\n", grauMedio(G));
    
    int v_max;
    int max_g = grauMax(G, &v_max);
    printf("Maior grau: %d (vertice %d)\n", max_g, v_max);

    printf("\n=== Menor Caminho Medio ===\n");
    printf("Calculando menor caminho medio (usando BFS para cada vertice)...\n");
    clock_t inicio = clock();
    float menor = menorCaminhoMedio(G);
    double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    
    if (menor >= 0) printf("Menor caminho medio: %.2f\n", menor);
    else printf("Nao foi possivel calcular o menor caminho medio (grafo desconexo ou erro)\n");
    printf("Tempo de calculo: %.2f segundos\n", tempo);

    printf("\n=== Assortatividade ===\nCalculando assortatividade...\n");
    inicio = clock();
    float ass = assortatividade(G);
    tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    
    printf("Assortatividade: %.4f\n", ass);
    printf("Interpretacao: Rede %s\n",
           ass > 0.1 ? "assortativa (vertices de grau similar tendem a se conectar)" :
           ass < -0.1 ? "disassortativa (vertices de grau diferente tendem a se conectar)" :
           "neutra (sem correlacao significativa entre graus)");
    printf("Tempo de calculo: %.2f segundos\n", tempo);
    
    printf("\n=== Estatisticas Adicionais ===\n");
    
    int* dist = calloc(max_g + 1, sizeof(int));
    for (int i = 0; i < G->num_vertices; i++) {
        int g = grau(G, i);
        if (g >= 0) dist[g]++;
    }
    
    printf("Distribuicao de graus (primeiros 10):\n");
    for (int i = 0; i <= 10 && i <= max_g; i++)
        printf("  Grau %d: %d vertices\n", i, dist[i]);
    
    printf("Vertices isolados (grau 0): %d\n", dist[0]);
    printf("Vertices folha (grau 1): %d\n", dist[1]);
    
    float densidade = (float)(2 * arestas) / (G->num_vertices * (G->num_vertices - 1));
    printf("Densidade do grafo: %.6f\n", densidade);
    
    printf("\n=== Limpeza ===\n");
    free(dist);
    destruirGrafo(G);
    printf("Memoria liberada com sucesso!\n");
    
    return 0;
}