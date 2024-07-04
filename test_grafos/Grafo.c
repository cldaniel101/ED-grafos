#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h" //inclui os Protótipos

// Definição do tipo Grafo
typedef struct grafo
{
    int eh_ponderado;
    int nro_vertices;
    int grau_max;
    int **arestas;
    float **pesos;
    int *grau;
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

void libera_Grafo(Grafo *gr)
{
    if (gr != NULL)
    {
        int i;
        for (i = 0; i < gr->nro_vertices; i++)
        {
            free(gr->arestas[i]);
            if (gr->eh_ponderado)
            {
                free(gr->pesos[i]);
            }
        }
        free(gr->arestas);
        free(gr->pesos);
        free(gr->grau);
        free(gr);
    }
}

// Programa principal
int main()
{
    Grafo *gr;
    gr = cria_Grafo(10, 7, 0);
    // Resto do seu código...
    libera_Grafo(gr); // Libera o grafo antes de encerrar o programa
    system("pause");
    return 0;
}