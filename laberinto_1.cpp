#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <unistd.h>  

using namespace std;

const char MURO = '🧱';
const char CAMINO = ' ';
const char ENTRADA = 'E';
const char SALIDA = 'S';
const char VISITADO = 'O';
const char RECORRIDO_FINAL = '*';

struct Punto {
    int x, y;
    Punto(int _x, int _y) : x(_x), y(_y) {}
};

// Función para generar un laberinto aleatorio
void generarLaberinto(vector<vector<char>>& laberinto, int ancho, int alto) {
    laberinto = vector<vector<char>>(alto, vector<char>(ancho, MURO));
    srand(time(0));

    stack<Punto> pilaCaminos;
    Punto inicio(1, 1);
    laberinto[inicio.y][inicio.x] = CAMINO;
    pilaCaminos.push(inicio);

    while (!pilaCaminos.empty()) {
        Punto actual = pilaCaminos.top();
        vector<Punto> vecinos;

        if (actual.y > 2 && laberinto[actual.y - 2][actual.x] == MURO) vecinos.push_back(Punto(actual.x, actual.y - 2));
        if (actual.y < alto - 3 && laberinto[actual.y + 2][actual.x] == MURO) vecinos.push_back(Punto(actual.x, actual.y + 2));
        if (actual.x > 2 && laberinto[actual.y][actual.x - 2] == MURO) vecinos.push_back(Punto(actual.x - 2, actual.y));
        if (actual.x < ancho - 3 && laberinto[actual.y][actual.x + 2] == MURO) vecinos.push_back(Punto(actual.x + 2, actual.y));

        if (!vecinos.empty()) {
            Punto siguiente = vecinos[rand() % vecinos.size()];
            laberinto[(actual.y + siguiente.y) / 2][(actual.x + siguiente.x) / 2] = CAMINO;
            laberinto[siguiente.y][siguiente.x] = CAMINO;
            pilaCaminos.push(siguiente);
        } else {
            pilaCaminos.pop();
        }
    }

    laberinto[1][1] = ENTRADA;
    laberinto[alto - 2][ancho - 2] = SALIDA;
}

// Función para imprimir el laberinto en la consola
void imprimirLaberinto(const vector<vector<char>>& laberinto) {
    for (const auto& fila : laberinto) {
        for (const auto& celda : fila) {
            cout << celda;
        }
        cout << endl;
    }
}

// Función para imprimir líneas vacías entre laberintos
void imprimirEspacioEntreLaberintos(int numLineas) {
    for (int i = 0; i < numLineas; ++i) {
        cout << endl;
    }
}

// Función para pausar la ejecución del programa
void pausar(int milisegundos) {
    usleep(milisegundos * 1000);  // Pausar en microsegundos
}

// Función para resolver el laberinto usando Backtracking y marcar el camino final
bool resolverLaberintoBacktracking(vector<vector<char>>& laberinto, int x, int y, vector<vector<bool>>& visitado) {
    if (x == static_cast<int>(laberinto[0].size()) - 2 && y == static_cast<int>(laberinto.size()) - 2) {
        laberinto[y][x] = RECORRIDO_FINAL;
        imprimirLaberinto(laberinto);
        pausar(500); // Pausa para ver el paso final
        return true;
    }

    static const vector<Punto> direcciones = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    for (const auto& dir : direcciones) {
        int nuevoX = x + dir.x;
        int nuevoY = y + dir.y;

        if (nuevoX >= 0 && nuevoY >= 0 && nuevoX < static_cast<int>(laberinto[0].size()) && nuevoY < static_cast<int>(laberinto.size()) &&
            (laberinto[nuevoY][nuevoX] == CAMINO || laberinto[nuevoY][nuevoX] == SALIDA) && !visitado[nuevoY][nuevoX]) {
            visitado[nuevoY][nuevoX] = true;
            laberinto[nuevoY][nuevoX] = RECORRIDO_FINAL; // Marcar como parte del recorrido final
            imprimirLaberinto(laberinto);
            imprimirEspacioEntreLaberintos(1); // Imprimir 1 líneas vacías entre laberintos
            pausar(650); // Pausa para ver el paso actual

            if (resolverLaberintoBacktracking(laberinto, nuevoX, nuevoY, visitado)) {
                return true;
            }

            laberinto[nuevoY][nuevoX] = VISITADO; // Marcar como visitado
            imprimirLaberinto(laberinto);
            imprimirEspacioEntreLaberintos(1); // Imprimir 1 líneas vacías entre laberintos
            pausar(650); // Pausa para ver el paso actual
        }
    }

    laberinto[y][x] = VISITADO;
    return false;
}

int main() {
    int ancho = 40;
    int alto = 15;
    vector<vector<char>> laberinto;

    generarLaberinto(laberinto, ancho, alto);
    imprimirLaberinto(laberinto);

    cout << "\nResolviendo laberinto usando Backtracking...\n";
    vector<vector<bool>> visitado(alto, vector<bool>(ancho, false));
    visitado[1][1] = true;
    if (resolverLaberintoBacktracking(laberinto, 1, 1, visitado)) {
        cout << "Laberinto resuelto.\n";
    } else {
        cout << "No se encontró solución usando Backtracking.\n";
    }

    return 0;
}
