//
// Created by claud on 03/12/2025.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

#include "classes/classesBacktracking/Solucio.h"
#include "classes/classesBacktracking/Solucionador.h"
#include "classes/classesAuxiliars/Vol.h"

using namespace std;

/**
 * FUNCIONES Y ESTRUCTURAS AUXILIARES
 */
struct excepcio {
    struct entradaIncorrecta {
        string missatge;
        entradaIncorrecta(const string &m) { missatge = m; }
    };
};

struct Parametres {
    string path;
    char tipoCalculo; // puede ser v, b, m
    string Ho;
    string Ht;
    int Pr;
    int Pi;
    bool ajuda;

    Parametres() {
        tipoCalculo = 'b'; // backtracking 1 opción por defecto
        Ho = "06:00";
        Ht = "22:00";
        Pr = numeric_limits<int>::infinity();
        Pi = numeric_limits<int>::infinity();
        path = "";
        ajuda = false;
    }
};

void mostrarAjuda(const char *nomPrograma) {
    cerr << "Ús: " << nomPrograma
            << " [ -h] | [ -v|-m ] [ -r <int> ] [ -i <int> ] "
            << "[-ho <hh:mm>] [-ht <hh:mm>] fitxer" << endl << endl;

    cerr << "Opció pot ser:" << endl;

    const int indent = 4; // espacios antes de la opción
    const int width = 12; // ancho reservado para la opción antes de la descripción

    cerr << string(indent, ' ') << left << setw(width) << "-h, --help"
            << "mostra aquest missatge d'ajuda i surt" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-v"
            << "cerca ràpida amb un algoritme voraç" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-m"
            << "cerca la solució que minimitza els slots buits i maximitza el gap mínim"
            << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-r <int>"
            << "indica el nombre de portes de tipus regional disponibles" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-i <int>"
            << "indica el nombre de portes internacionals disponibles" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-ho <hh:mm>"
            << "indica l'hora (hh:mm) d'obertura de l'aeroport" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-ht <hh:mm>"
            << "indica l'hora (hh:mm) de tancament de l'aeroport" << endl;

    cerr << endl;

    cerr << string(indent, ' ') << left << setw(width) << "fitxer"
            << "fitxer de text amb tots els vols i altres dades requerides" << endl;
}

Parametres processaParametres(int argn, char **argv) {
    Parametres p;

    if (argn < 2 || argn > 7) {
        throw excepcio::entradaIncorrecta("Nombre d'arguments invàlid");
    }

    int nParams = 1;

    // Opción -h
    if (string(argv[nParams]) == "-h" || string(argv[nParams]) == "--help") {
        p.ajuda = true;
        return p;
    }

    while (nParams < argn - 1) {
        string param = argv[nParams];

        // puertas regionales o internacionales
        if (param == "-r" || param == "-i") {
            if (nParams + 1 >= argn) {
                throw excepcio::entradaIncorrecta("Error: el valor associat a l'opcio '-r' és incorrecte.");
            }

            int numPortes = stoi(argv[++nParams]);

            if (param == "-r") p.Pr = numPortes;
            else p.Pi = numPortes;
        // hora de apertura o cierre del aeropuerto
        } else if (param == "-ho" || param == "-ht") {
            // comprobar que existe valor después de param
            if (nParams + 1 >= argn) {;
                throw excepcio::entradaIncorrecta("Error: el formato de la hora es incorrecto.");
            }

            string hora = argv[++nParams];
            // comprobar formato correcto de hora
            if (hora.size() == 5) {
                if (param == "-ho") p.Ho = hora;
                else p.Ht = hora;
            } else {
                throw excepcio::entradaIncorrecta("Error: el formato de la hora es incorrecto.");
            }
        } else if (param == "-m") {
            p.tipoCalculo = 'm';
        } else if (param == "-v") {
            p.tipoCalculo = 'v';
        } else {
            // archivo de vuelos
            p.path = argv[nParams];
        }

        nParams++;
    }

    return p;
}

int llegirDades(const string& path, vector<Vol>& vols);


/**
 * PROGRAMA PRINCIPAL
 */

// TODO: calcular tiempo de cómputo para cada algoritmo

int main(int argn, char **argv) {
    try {
        Parametres p = processaParametres(argn, argv);

        if (p.ajuda) {
            mostrarAjuda(argv[0]); // mostrar ayuda con el nombre del programa
        } else {
            if (p.path.empty()) {
                throw excepcio::entradaIncorrecta("Error: falta el nom del fitxer.");
            }

            cerr << "Fitxer de entrada de vols: " << p.path << endl;

            // lectura de fichero
            vector<Vol> vols; // vector para guardar los vuelos leídos
            // TODO: implementar
            int dades = llegirDades(p.path, vols);
            cout << dades << " vols llegits.";

            Solucio sol(p.Pr, p.Pi, p.Ho, p.Ht);
            if (p.tipoCalculo == 'v') {
                cerr << "usando solucionador voraz" << endl;
                Voraz solucionadorVoraz;
                solucionadorVoraz.solucionar(sol);
            }
            else if (p.tipoCalculo == 'b') {
                cerr << "usando backtracking una solución" << endl;
                BacktrackingUnaSol solucinadorUnaSol;
                solucinadorUnaSol.solucionar(sol);
            }
            else if (p.tipoCalculo == 'm') {
                cerr << "usando backtracking mejor solución" << endl;
                BacktrackingMillorSol solucionadorMillorSol;
                solucionadorMillorSol.solucionar(sol);
            }
        }
    } catch (excepcio::entradaIncorrecta ex) {
        cerr << ex.missatge << endl;
        mostrarAjuda(argv[0]);
    }
    return 0;
}
