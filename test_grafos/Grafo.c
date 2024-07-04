#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h" // Inclui os protótipos

// Definição do tipo Grafo
typedef struct grafo
{
    int eh_ponderado; // Indica se o grafo é ponderado (1) ou não (0)
    int nro_vertices; // Número de vértices no grafo
    int grau_max;     // Grau máximo (número máximo de arestas que um vértice pode ter)
    int **arestas;    // Matriz de inteiros que representa as arestas do grafo
    float **pesos;    // Matriz de floats que representa os pesos das arestas (se ponderado)
    int *grau;        // Vetor de inteiros que indica o grau de cada vértice
} Grafo;

// Protótipo da função cria_Grafo
Grafo *cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado)
{
    Grafo *gr = (Grafo *)malloc(sizeof(struct grafo));
    if (gr != NULL)
    {
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
        gr->grau = (int *)calloc(nro_vertices, sizeof(int));
        gr->arestas = (int **)malloc(nro_vertices * sizeof(int *));
        for (i = 0; i < nro_vertices; i++)
        {
            gr->arestas[i] = (int *)malloc(grau_max * sizeof(int));
        }
        if (gr->eh_ponderado)
        {
            gr->pesos = (float **)malloc(nro_vertices * sizeof(float *));
            for (i = 0; i < nro_vertices; i++)
            {
                gr->pesos[i] = (float *)malloc(grau_max * sizeof(float));
            }
        }
    }
    return gr;
}

int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso)
{
    if (gr == NULL)
        return 0;
    if (orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if (dest < 0 || dest >= gr->nro_vertices)
        return 0;

    gr->arestas[orig][gr->grau[orig]] = dest;
    if (gr->eh_ponderado)
        gr->pesos[orig][gr->grau[orig]] = peso;
    gr->grau[orig]++;

    if (eh_digrafo == 0)
        insereAresta(gr, dest, orig, 1, peso);

    return 1;
}

void libera_Grafo(Grafo *gr)
{
    if (gr != NULL)
    {
        int i;
        for (i = 0; i < gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);
        if (gr->eh_ponderado)
        {
            for (i = 0; i < gr->nro_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int removeAresta(Grafo *gr, int orig, int dest, int eh_digrafo)
{
    if (gr == NULL)
        return 0;
    if (orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if (dest < 0 || dest >= gr->nro_vertices)
        return 0;

    // Verifica se a aresta existe
    int i = 0;
    while (i < gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;
    if (i == gr->grau[orig]) // Elemento não encontrado
        return 0;

    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if (gr->eh_ponderado)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];

    // Remove outra aresta se não for dígrafo
    if (eh_digrafo == 0)
        removeAresta(gr, dest, orig, 1);

    return 1;
}

// Função auxiliar
void buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont)
{
    int i;
    visitado[ini] = cont;
    for (i = 0; i < gr->grau[ini]; i++)
    {
        if (!visitado[gr->arestas[ini][i]])
        {
            buscaProfundidade(gr, gr->arestas[ini][i], visitado, cont + 1);
        }
    }
}

// Função principal: faz a interface com o usuário
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado)
{
    int i, cont = 1;

    // Marca vértices como NÃO visitados
    for (i = 0; i < gr->nro_vertices; i++)
    {
        visitado[i] = 0;
    }

    // Inicia a busca em profundidade
    buscaProfundidade(gr, ini, visitado, cont);
}

void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado)
{
    int i, vert, NV, cont = 1, IF = 0, FF = 0;

    // Marca vértices como não visitados
    for (i = 0; i < gr->nro_vertices; i++)
        visitado[i] = 0;

    NV = gr->nro_vertices;
    int *fila = (int *)malloc(NV * sizeof(int)); // Cria fila

    // Visita e insere "ini" na fila
    fila[FF] = ini;
    visitado[ini] = cont;

    while (IF != FF)
    {
        vert = fila[IF];
        IF = (IF + 1) % NV;
        cont++;

        for (i = 0; i < gr->grau[vert]; i++)
        {
            if (!visitado[gr->arestas[vert][i]])
            {
                FF = (FF + 1) % NV;
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;
            }
        }
    }

    free(fila); // Libera a memória alocada para a fila
}

int procuraMenorDistancia(float *dist, int *visitado, int NV)
{
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < NV; i++)
    {
        if (dist[i] >= 0 && visitado[i] == 0)
        {
            if (primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if (dist[menor] > dist[i])
                {
                    menor = i;
                }
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist)
{
    int i, cont, NV, ind, *visitado, u;
    NV = gr->nro_vertices;
    visitado = (int *)malloc(NV * sizeof(int));

    for (i = 0; i < NV; i++)
    {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;
    cont = NV;

    while (cont > 0)
    {
        u = procuraMenorDistancia(dist, visitado, NV);
        if (u == -1)
            break;

        visitado[u] = 1;
        cont--;

        for (i = 0; i < gr->grau[u]; i++)
        {
            ind = gr->arestas[u][i];
            if (dist[ind] < 0)
            {
                dist[ind] = dist[u] + 1; // ou peso da aresta
                ant[ind] = u;
            }
            else if (dist[ind] > dist[u] + 1)
            {
                dist[ind] = dist[u] + 1; // ou peso da aresta
                ant[ind] = u;
            }
        }
    }
    free(visitado);
}


// Programa principal
int main()
{
    int eh_digrafo = 1;
    Grafo *gr = cria_Grafo(5, 5, 0); // Cria um grafo com 5 vértices, grau máximo 5, não ponderado

    // Inserindo arestas no grafo
    insereAresta(gr, 0, 1, eh_digrafo, 0);
    insereAresta(gr, 1, 3, eh_digrafo, 0);
    insereAresta(gr, 1, 2, eh_digrafo, 0);
    insereAresta(gr, 2, 4, eh_digrafo, 0);
    insereAresta(gr, 3, 0, eh_digrafo, 0);
    insereAresta(gr, 3, 4, eh_digrafo, 0);
    insereAresta(gr, 4, 1, eh_digrafo, 0);

    // Vetor de visitados para a busca em profundidade
    int vis[5];
    buscaProfundidade_Grafo(gr, 0, vis);

    // Vetor de visitados para a busca em largura
    buscaLargura_Grafo(gr, 0, vis);

    // Vetores para menor caminho
    int ant[5];
    float dist[5];
    menorCaminho_Grafo(gr, 0, ant, dist);

    // Libera o grafo antes de encerrar o programa
    libera_Grafo(gr);
    system("pause");
    return 0;
}
