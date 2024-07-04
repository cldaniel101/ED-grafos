#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// Estrutura do Grafo
struct Grafo {
    int eh_ponderado;
    int nro_vertices;
    int grau_max;
    vector<vector<int>> arestas;
    vector<vector<float>> pesos;
    vector<int> grau;
};

// Função para criar o grafo
Grafo* criaGrafo(int nro_vertices, int grau_max, int eh_ponderado) {
    Grafo* gr = new Grafo;
    gr->nro_vertices = nro_vertices;
    gr->grau_max = grau_max;
    gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
    gr->grau.resize(nro_vertices, 0);
    gr->arestas.resize(nro_vertices, vector<int>(grau_max, 0));
    if (gr->eh_ponderado) {
        gr->pesos.resize(nro_vertices, vector<float>(grau_max, 0.0));
    }
    return gr;
}

// Função para liberar o grafo
void liberaGrafo(Grafo* gr) {
    delete gr;
}

// Função para inserir aresta no grafo
bool insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso) {
    if (orig < 0 || orig >= gr->nro_vertices || dest < 0 || dest >= gr->nro_vertices) return false;

    gr->arestas[orig][gr->grau[orig]] = dest;
    if (gr->eh_ponderado) gr->pesos[orig][gr->grau[orig]] = peso;
    gr->grau[orig]++;

    if (eh_digrafo == 0) insereAresta(gr, dest, orig, 1, peso);

    return true;
}

// Função para remover aresta do grafo
bool removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo) {
    if (orig < 0 || orig >= gr->nro_vertices || dest < 0 || dest >= gr->nro_vertices) return false;

    int i = 0;
    while (i < gr->grau[orig] && gr->arestas[orig][i] != dest) i++;
    if (i == gr->grau[orig]) return false;

    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if (gr->eh_ponderado) gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];

    if (eh_digrafo == 0) removeAresta(gr, dest, orig, 1);

    return true;
}

// Função de busca em profundidade
void buscaProfundidade(Grafo* gr, int ini, vector<int>& visitado, int cont) {
    visitado[ini] = cont;
    for (int i = 0; i < gr->grau[ini]; i++) {
        if (!visitado[gr->arestas[ini][i]]) {
            buscaProfundidade(gr, gr->arestas[ini][i], visitado, cont + 1);
        }
    }
}

// Função para inicializar a busca em profundidade no grafo
void buscaProfundidade_Grafo(Grafo* gr, int ini, vector<int>& visitado) {
    visitado.assign(gr->nro_vertices, 0);
    buscaProfundidade(gr, ini, visitado, 1);
}

// Função de busca em largura
void buscaLargura_Grafo(Grafo* gr, int ini, vector<int>& visitado) {
    visitado.assign(gr->nro_vertices, 0);
    queue<int> fila;
    fila.push(ini);
    visitado[ini] = 1;
    int cont = 1;

    while (!fila.empty()) {
        int vert = fila.front();
        fila.pop();
        cont++;

        for (int i = 0; i < gr->grau[vert]; i++) {
            if (!visitado[gr->arestas[vert][i]]) {
                fila.push(gr->arestas[vert][i]);
                visitado[gr->arestas[vert][i]] = cont;
            }
        }
    }
}

// Função para procurar a menor distância
int procuraMenorDistancia(const vector<float>& dist, const vector<int>& visitado, int nro_vertices) {
    int menor = -1;
    for (int i = 0; i < nro_vertices; i++) {
        if (dist[i] >= 0 && visitado[i] == 0) {
            if (menor == -1 || dist[i] < dist[menor]) {
                menor = i;
            }
        }
    }
    return menor;
}

// Função para encontrar o menor caminho no grafo
void menorCaminho_Grafo(Grafo* gr, int ini, vector<int>& ant, vector<float>& dist) {
    vector<int> visitado(gr->nro_vertices, 0);
    ant.assign(gr->nro_vertices, -1);
    dist.assign(gr->nro_vertices, numeric_limits<float>::infinity());
    dist[ini] = 0;
    int cont = gr->nro_vertices;

    while (cont > 0) {
        int u = procuraMenorDistancia(dist, visitado, gr->nro_vertices);
        if (u == -1) break;

        visitado[u] = 1;
        cont--;

        for (int i = 0; i < gr->grau[u]; i++) {
            int ind = gr->arestas[u][i];
            if (dist[ind] > dist[u] + 1) { // ou peso da aresta
                dist[ind] = dist[u] + 1; // ou peso da aresta
                ant[ind] = u;
            }
        }
    }
}

// Programa principal
int main() {
    int eh_digrafo = 1;
    Grafo* gr = criaGrafo(5, 5, 0); // Cria um grafo com 5 vértices, grau máximo 5, não ponderado

    // Inserindo arestas no grafo
    insereAresta(gr, 0, 1, eh_digrafo, 0);
    insereAresta(gr, 1, 3, eh_digrafo, 0);
    insereAresta(gr, 1, 2, eh_digrafo, 0);
    insereAresta(gr, 2, 4, eh_digrafo, 0);
    insereAresta(gr, 3, 0, eh_digrafo, 0);
    insereAresta(gr, 3, 4, eh_digrafo, 0);
    insereAresta(gr, 4, 1, eh_digrafo, 0);

    // Vetor de visitados para a busca em profundidade
    vector<int> vis(5);
    buscaProfundidade_Grafo(gr, 0, vis);

    // Vetor de visitados para a busca em largura
    buscaLargura_Grafo(gr, 0, vis);

    // Vetores para menor caminho
    vector<int> ant(5);
    vector<float> dist(5);
    menorCaminho_Grafo(gr, 0, ant, dist);

    // Libera o grafo antes de encerrar o programa
    liberaGrafo(gr);

    system("pause");
    return 0;
}
