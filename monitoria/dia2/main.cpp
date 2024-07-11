#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

//Dupla: Felipe Amorim e Claudio Daniel

using namespace std;

// Função de busca em profundidade (DFS)
int dfs(unordered_map<int, vector<int>>& grafo, int inicio, int total_vertices) {
    int contador = 0;
    // Vetor para marcar os vértices já visitados
    vector<int> visitados(total_vertices, 1);
    // Pilha para gerenciar os vértices a serem visitados
    stack<int> pilha;
    pilha.push(inicio);
    while (!pilha.empty()) {
        int vertice_atual = pilha.top();
        pilha.pop();
        if (visitados[vertice_atual]) {
            contador++;
            visitados[vertice_atual] = 0;
            // Adiciona todos os vizinhos do vértice atual na pilha
            for (int vizinho : grafo[vertice_atual]) {
                pilha.push(vizinho);
            }
        }
    }
    return contador;
}

int main() {
    int num_casos;
    cin >> num_casos;
    for (int caso_atual = 0; caso_atual < num_casos; ++caso_atual) {
        int vertice_inicio;
        cin >> vertice_inicio;
        int num_vertices, num_arestas;
        cin >> num_vertices >> num_arestas;
        // Grafo representado como um mapa de listas de adjacência
        unordered_map<int, vector<int>> grafo;
        for (int aresta_atual = 0; aresta_atual < num_arestas; ++aresta_atual) {
            int vertice1, vertice2;
            cin >> vertice1 >> vertice2;
            grafo[vertice1].push_back(vertice2);
            grafo[vertice2].push_back(vertice1);
        }
        // Calcula o resultado usando a função DFS e imprime
        cout << 2 * dfs(grafo, vertice_inicio, num_vertices) - 2 << endl;
    }
    return 0;
}
