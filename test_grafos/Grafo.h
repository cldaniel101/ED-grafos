#ifndef GRAFO_H
#define GRAFO_H

// Definição do tipo Grafo
typedef struct grafo Grafo;

// Protótipos das funções
Grafo *cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado);
void libera_Grafo(Grafo *gr);
int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso);
int removeAresta(Grafo *gr, int orig, int dest, int eh_digrafo);
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado);
void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado);
int procuraMenorDistancia(float *dist, int *visitado, int NV);
void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist);

#endif // GRAFO_H
