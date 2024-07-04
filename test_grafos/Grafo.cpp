#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "Grafo.h" // Inclui os protótipos

using namespace std;

// Definição da classe Grafo
class Grafo
{
public:
    int eh_ponderado;            // Indica se o grafo é ponderado (1) ou não (0)
    int nro_vertices;            // Número de vértices no grafo
    int grau_max;                // Grau máximo (número máximo de arestas que um vértice pode ter)
    vector<vector<int>> arestas; // Matriz de inteiros que representa as arestas do grafo
    vector<vector<float>> pesos; // Matriz de floats que representa os pesos das arestas (se ponderado)
    vector<int> grau;            // Vetor de inteiros que indica o grau de cada vértice

    Grafo(int nro_vertices, int grau_max, int eh_ponderado)
    {
        this->nro_vertices = nro_vertices;
        this->grau_max = grau_max;
        this->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
        grau.resize(nro_vertices, 0);
        arestas.resize(nro_vertices, vector<int>(grau_max, 0));
        if (this->eh_ponderado)
        {
            pesos.resize(nro_vertices, vector<float>(grau_max, 0.0));
        }
    }

    ~Grafo()
    {
        // O vector irá liberar automaticamente a memória
    }

    bool insereAresta(int orig, int dest, int eh_digrafo, float peso)
    {
        if (orig < 0 || orig >= nro_vertices || dest < 0 || dest >= nro_vertices)
            return false;

        arestas[orig][grau[orig]] = dest;
        if (eh_ponderado)
            pesos[orig][grau[orig]] = peso;
        grau[orig]++;

        if (eh_digrafo == 0)
            insereAresta(dest, orig, 1, peso);

        return true;
    }

    bool removeAresta(int orig, int dest, int eh_digrafo)
    {
        if (orig < 0 || orig >= nro_vertices || dest < 0 || dest >= nro_vertices)
            return false;

        int i = 0;
        while (i < grau[orig] && arestas[orig][i] != dest)
            i++;
        if (i == grau[orig])
            return false;

        grau[orig]--;
        arestas[orig][i] = arestas[orig][grau[orig]];
        if (eh_ponderado)
            pesos[orig][i] = pesos[orig][grau[orig]];

        if (eh_digrafo == 0)
            removeAresta(dest, orig, 1);

        return true;
    }

    void buscaProfundidade(int ini, vector<int> &visitado, int cont)
    {
        visitado[ini] = cont;
        for (int i = 0; i < grau[ini]; i++)
        {
            if (!visitado[arestas[ini][i]])
            {
                buscaProfundidade(arestas[ini][i], visitado, cont + 1);
            }
        }
    }

    void buscaProfundidade_Grafo(int ini, vector<int> &visitado)
    {
        visitado.assign(nro_vertices, 0);
        buscaProfundidade(ini, visitado, 1);
    }

    void buscaLargura_Grafo(int ini, vector<int> &visitado)
    {
        visitado.assign(nro_vertices, 0);
        queue<int> fila;
        fila.push(ini);
        visitado[ini] = 1;
        int cont = 1;

        while (!fila.empty())
        {
            int vert = fila.front();
            fila.pop();
            cont++;

            for (int i = 0; i < grau[vert]; i++)
            {
                if (!visitado[arestas[vert][i]])
                {
                    fila.push(arestas[vert][i]);
                    visitado[arestas[vert][i]] = cont;
                }
            }
        }
    }

    int procuraMenorDistancia(const vector<float> &dist, const vector<int> &visitado)
    {
        int menor = -1;
        for (int i = 0; i < nro_vertices; i++)
        {
            if (dist[i] >= 0 && visitado[i] == 0)
            {
                if (menor == -1 || dist[i] < dist[menor])
                {
                    menor = i;
                }
            }
        }
        return menor;
    }

    void menorCaminho_Grafo(int ini, vector<int> &ant, vector<float> &dist)
    {
        vector<int> visitado(nro_vertices, 0);
        ant.assign(nro_vertices, -1);
        dist.assign(nro_vertices, numeric_limits<float>::infinity());
        dist[ini] = 0;
        int cont = nro_vertices;

        while (cont > 0)
        {
            int u = procuraMenorDistancia(dist, visitado);
            if (u == -1)
                break;

            visitado[u] = 1;
            cont--;

            for (int i = 0; i < grau[u]; i++)
            {
                int ind = arestas[u][i];
                if (dist[ind] > dist[u] + 1)
                {                            // ou peso da aresta
                    dist[ind] = dist[u] + 1; // ou peso da aresta
                    ant[ind] = u;
                }
            }
        }
    }
};

// Programa principal
int main()
{
    int eh_digrafo = 1;
    Grafo gr(5, 5, 0); // Cria um grafo com 5 vértices, grau máximo 5, não ponderado

    // Inserindo arestas no grafo
    gr.insereAresta(0, 1, eh_digrafo, 0);
    gr.insereAresta(1, 3, eh_digrafo, 0);
    gr.insereAresta(1, 2, eh_digrafo, 0);
    gr.insereAresta(2, 4, eh_digrafo, 0);
    gr.insereAresta(3, 0, eh_digrafo, 0);
    gr.insereAresta(3, 4, eh_digrafo, 0);
    gr.insereAresta(4, 1, eh_digrafo, 0);

    // Vetor de visitados para a busca em profundidade
    vector<int> vis(5);
    gr.buscaProfundidade_Grafo(0, vis);

    // Vetor de visitados para a busca em largura
    gr.buscaLargura_Grafo(0, vis);

    // Vetores para menor caminho
    vector<int> ant(5);
    vector<float> dist(5);
    gr.menorCaminho_Grafo(0, ant, dist);

    // Libera o grafo antes de encerrar o programa
    // (Não é necessário liberar explicitamente no C++ devido ao uso de destructors e containers de STL)

    system("pause");
    return 0;
}
