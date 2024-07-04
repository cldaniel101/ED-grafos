#include <iostream>
#include <cstring>
#include <queue>
#include <unordered_map>

using namespace std;

int numero_inicial, numero_final;
unordered_map<int, int> distancias;

void bfs(int inicio);

int main()
{
    int num_testes;
    cin >> num_testes;

    while (num_testes--)
    {
        distancias.clear();
        cin >> numero_inicial >> numero_final;

        bfs(numero_inicial);

        cout << distancias[numero_final] << endl;
    }

    return 0;
}

void bfs(int inicio)
{
    queue<int> fila;
    fila.push(inicio);
    distancias[inicio] = 0;

    while (!fila.empty())
    {
        int atual = fila.front();
        fila.pop();

        if (atual == numero_final)
        {
            return;
        }

        // Inverte os dígitos
        int invertido = 0, temp = atual;
        while (temp)
        {
            invertido = invertido * 10 + temp % 10;
            temp /= 10;
        }

        // Se o número invertido não foi visitado
        if (distancias.find(invertido) == distancias.end())
        {
            distancias[invertido] = distancias[atual] + 1;
            fila.push(invertido);
        }

        // Incrementa 1
        int proximo = atual + 1;
        if (distancias.find(proximo) == distancias.end())
        {
            distancias[proximo] = distancias[atual] + 1;
            fila.push(proximo);
        }
    }
}
