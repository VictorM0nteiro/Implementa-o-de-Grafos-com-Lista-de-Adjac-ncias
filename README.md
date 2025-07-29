# Trabalho 01 - Estrutura de Dados II
## Implementação de Grafos com Lista de Adjacências

### Descrição
Este projeto implementa uma estrutura de dados para grafos usando lista de adjacências com encadeamento dinâmico, conforme especificado no trabalho da disciplina FACOM32402 - Estrutura de Dados II.

### Funcionalidades Implementadas

#### Funções Principais (conforme especificação):
- `insereVertice(Grafo *G, char *label)` - Insere vértice com alocação dinâmica
- `insereAresta(Grafo *G, int u, int v, float w)` - Insere aresta com peso
- `removeAresta(Grafo *G, int u, int v)` - Remove aresta
- `grau(Grafo *G, int v)` - Retorna grau do vértice
- `grauMedio(Grafo *G)` - Retorna grau médio do grafo
- `grauMax(Grafo *G, int *v)` - Retorna maior grau e vértice correspondente
- `menorCaminhoMedio(Grafo *G)` - Calcula menor caminho médio usando BFS para grafos não ponderados (e Dijkstra para ponderados)
- `assortatividade(Grafo *G)` - Calcula assortatividade da rede

#### Funções Auxiliares:
- `criarGrafo()` - Cria novo grafo
- `destruirGrafo()` - Libera memória
- `carregarGrafoMTX()` - Carrega grafo de arquivo MTX
- `numeroComponentesConexas()` - Conta componentes conexas
- `imprimirGrafo()` - Imprime informações do grafo

### Compilação e Execução

#### Pré-requisitos:
- GCC (GNU Compiler Collection)
- Make

#### Compilar:
```bash
gcc main.c grafo.c -o programa
```

#### Executar:
```bash
./programa.exe
```

#### Limpar arquivos compilados:
Remova manualmente o executável se desejar:
```bash
del programa.exe
```

### Análise da Rede Inf-Power

O programa analisa a rede "inf-power" que representa uma rede de infraestrutura de energia com:
- 4.941 vértices
- 6.594 arestas
- Grafo não direcionado e não ponderado

### Métricas Calculadas

1. **Informações Básicas:**
   - Número de vértices e arestas
   - Tipo do grafo (direcionado/não direcionado)
   - Densidade

2. **Análise de Graus:**
   - Grau médio
   - Maior grau e vértice correspondente
   - Distribuição de graus

3. **Conectividade:**
   - Número de componentes conexas
   - Vértices isolados e folhas

4. **Métricas de Rede:**
   - Menor caminho médio (usando BFS para grafos não ponderados; Dijkstra para ponderados)
   - Assortatividade (correlação entre graus de vértices conectados)

### Estrutura de Arquivos

```
trabalho01/
├── grafo.h          # Interface do TAD Grafo
├── grafo.c          # Implementação das funções
├── main.c           # Programa principal
├── Makefile         # Arquivo de compilação
├── README.md        # Este arquivo
└── inf-power/
    ├── inf-power.mtx # Arquivo de dados da rede
    └── readme.html   # Informações sobre a rede
```

### Características da Implementação

- **TAD Opaco:** A estrutura interna do grafo é oculta
- **Alocação Dinâmica:** Array de vértices aumenta em 50% quando necessário
- **Lista de Adjacências:** Implementada com encadeamento dinâmico
- **Suporte a Grafos Direcionados e Não Direcionados**
- **Suporte a Grafos Ponderados e Não Ponderados**
- **Gerenciamento de Memória:** Liberação adequada de toda memória alocada

### Algoritmos Implementados

1. **Dijkstra:** Para cálculo de menor caminho
2. **DFS:** Para encontrar componentes conexas
3. **Assortatividade:** Usando coeficiente de correlação de Pearson

### Observações

- O cálculo do menor caminho médio usa BFS para grafos não ponderados, tornando o processo muito mais rápido. Para grafos ponderados, utiliza Dijkstra.
- Os arquivos grafo_2.c e main_2.c estão ocultos e não fazem parte da entrega principal.
- Todas as funções incluem verificações de erro e validação de parâmetros 