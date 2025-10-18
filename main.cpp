#include <iostream>
using namespace std;

// Clase genérica del simulador 2D
template <typename T>
class Simulador2D {
private:
    T **_grid;
    int _filas, _columnas;

    T *_fuentes;
    int _numFuentes;
    int _capacidadFuentes;

    float _constantes[3]; // [0] difusión, [1] deltaX, [2] deltaT

public:
    // Constructor
    Simulador2D(int f, int c) {
        _filas = f;
        _columnas = c;

        // Crear grid dinámico
        _grid = new T*[f];
        for (int i = 0; i < f; i++) {
            _grid[i] = new T[c];
            for (int j = 0; j < c; j++)
                _grid[i][j] = 0;
        }

        // Crear vector de fuentes
        _capacidadFuentes = 2;
        _fuentes = new T[_capacidadFuentes];
        _numFuentes = 0;

        // Constantes físicas
        _constantes[0] = 0.1f; // Coef. difusión
        _constantes[1] = 1.0f; // DeltaX
        _constantes[2] = 1.0f; // DeltaT
    }

    // Destructor
    ~Simulador2D() {
        for (int i = 0; i < _filas; i++)
            delete[] _grid[i];
        delete[] _grid;
        delete[] _fuentes;
    }

    // Agregar una fuente
    void agregarFuente(T valor) {
        if (_numFuentes >= _capacidadFuentes) {
            // duplicar capacidad
            _capacidadFuentes *= 2;
            T *nuevo = new T[_capacidadFuentes];
            for (int i = 0; i < _numFuentes; i++)
                nuevo[i] = _fuentes[i];
            delete[] _fuentes;
            _fuentes = nuevo;
        }
        _fuentes[_numFuentes++] = valor;
        cout << "Agregar Fuente " << _numFuentes
             << " (Valor " << valor << "): Éxito." << endl;
    }

    // Redimensionar grid
    void redimensionarGrid(int nuevaF, int nuevaC) {
        T **nuevo = new T*[nuevaF];
        for (int i = 0; i < nuevaF; i++) {
            nuevo[i] = new T[nuevaC];
            for (int j = 0; j < nuevaC; j++) {
                if (i < _filas && j < _columnas)
                    nuevo[i][j] = _grid[i][j];
                else
                    nuevo[i][j] = 0;
            }
        }

        for (int i = 0; i < _filas; i++)
            delete[] _grid[i];
        delete[] _grid;

        _grid = nuevo;
        _filas = nuevaF;
        _columnas = nuevaC;

        cout << "\nRedimensionando Grid a " << nuevaF << "x" << nuevaC << ". Datos copiados.\n";
    }

    // Asignar valor a una celda específica
    void asignarValor(int f, int c, T valor) {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas)
            _grid[f][c] = valor;
    }

    // Mostrar grid
    void mostrarGrid() {
        for (int i = 0; i < _filas; i++) {
            for (int j = 0; j < _columnas; j++) {
                cout << "| " << _grid[i][j] << " ";
            }
            cout << "|\n";
        }
    }

    // Simular un paso de difusión
    void simularPaso() {
        T **nuevo = new T*[_filas];
        for (int i = 0; i < _filas; i++) {
            nuevo[i] = new T[_columnas];
            for (int j = 0; j < _columnas; j++)
                nuevo[i][j] = _grid[i][j];
        }

        // Promedio de los vecinos (excepto bordes)
        for (int i = 1; i < _filas - 1; i++) {
            for (int j = 1; j < _columnas - 1; j++) {
                nuevo[i][j] = (_grid[i - 1][j] + _grid[i + 1][j] +
                               _grid[i][j - 1] + _grid[i][j + 1]) / 4.0;
            }
        }

        // Copiar resultados al grid original
        for (int i = 0; i < _filas; i++) {
            for (int j = 0; j < _columnas; j++)
                _grid[i][j] = nuevo[i][j];
            delete[] nuevo[i];
        }
        delete[] nuevo;

        cout << "\nSimulando Propagación...\n";
    }
};

// ===============================
// Programa principal
// ===============================
int main() {
    cout << "--- Simulador Genérico de Fluidos (Difusión) ---\n\n";

    cout << ">> Inicializando Sistema (Tipo FLOAT) <<\n";
    Simulador2D<float> sim(5, 5);
    cout << "Creando Grid (FLOAT) de 5x5...\n\n";

    cout << ">> Agregando Fuentes de Concentración <<\n";
    sim.agregarFuente(100.0f);
    sim.agregarFuente(50.0f);

    cout << "\n>> Aplicando Valores Iniciales <<\n";
    sim.asignarValor(2, 2, 100.0f);
    sim.asignarValor(4, 0, 50.0f);
    sim.mostrarGrid();

    cout << "\n--- Grid Inicial (Paso 0) ---\n";
    sim.mostrarGrid();

    cout << "\nOpción: Simular 1 Paso\n";
    sim.simularPaso();

    cout << "\n--- Grid Después del Paso 1 ---\n";
    sim.mostrarGrid();

    cout << "\nOpción: Redimensionar\n";
    sim.redimensionarGrid(6, 6);

    cout << "\nOpción: Salir\n";
    cout << "Liberando memoria del Grid y Fuentes... Sistema cerrado.\n";
    return 0;
}

