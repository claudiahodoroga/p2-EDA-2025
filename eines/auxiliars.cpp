//
// Created by claud on 04/12/2025.
//

#include "auxiliars.h"
#include <sstream>
#include <iomanip>
#include <chrono>

// Necesario para el cuerpo de mostrarResultat si se usa Solucio
// y duration<double>. Se asume que Solucio está definida en "classes/classesBacktracking/Solucio.h"
#include "eines.h"
#include "../classes/classesBacktracking/Solucio.h"

using namespace std::chrono;

/**
 * IMPLEMENTACIÓN DE ESTRUCTURAS Y CONSTRUCTORES
 */

excepcio::entradaIncorrecta::entradaIncorrecta(const string &m) { missatge = m; }
excepcio::errorLectura::errorLectura(const string &m) { missatge = m; }

Parametres::Parametres() {
    tipoCalculo = 'b';
    Ho = "06:00";
    Ht = "22:00";
    Pr = INT_MAX;
    Pi = INT_MAX;
    path = "";
    ajuda = false;
}

/**
 * FUNCIONES AUXILIARES DE VALIDACIÓN
 */

// Se asume que la función 'tokens' se define en "eines/eines.h" o está implementada aquí
// La función 'tokens' no está incluida aquí, asumiendo que es externa (eines.h) o se moverá

bool validarHoraUnica(const string &h) {
    if (h.size() != 5) return false;
    if (h[2] != ':') return false;

    if (!isdigit(h[0]) || !isdigit(h[1]) || !isdigit(h[3]) || !isdigit(h[4]))
        return false;

    int hh = stoi(h.substr(0, 2));
    int mm = stoi(h.substr(3, 2));

    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;

    return true;
}

bool validarHores(const string &h1, const string &h2) {
    if (!validarHoraUnica(h1) || !validarHoraUnica(h2))
        return false;

    int t1 = stoi(h1.substr(0, 2)) * 60 + stoi(h1.substr(3, 2));
    int t2 = stoi(h2.substr(0, 2)) * 60 + stoi(h2.substr(3, 2));

    return t1 < t2;
}

bool validarDades(int id, char tipus, char mida, const string &tv1, const string &tv2) {
    return (id > 0) && (tipus == 'r' || tipus == 'i') && (mida == 'p' || mida == 'm' || mida == 'g') &&
           validarHores(tv1, tv2);
}

/**
 * IMPLEMENTACIÓN DE FUNCIONES AUXILIARES
 */

void mostrarAjuda(const char *nomPrograma) {
    cerr << "Ús: " << nomPrograma
            << " [ -h] | [ -v|-m ] [ -r <int> ] [ -i <int> ] "
            << "[-ho <hh:mm>] [-ht <hh:mm>] fitxer" << endl << endl;

    cerr << "Opció pot ser:" << endl;

    const int indent = 4;
    const int width = 12;

    cerr << string(indent, ' ') << left << setw(width) << "-h, --help"
            << "mostra aquest missatge d'ajuda i surt" << endl;

    cerr << string(indent, ' ') << left << setw(width) << "-v"
            << "cerca ràpida amb un algoritme voraç" << endl;

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

    if (string(argv[nParams]) == "-h" || string(argv[nParams]) == "--help") {
        p.ajuda = true;
        return p;
    }

    while (nParams < argn) {
        string param = argv[nParams];

        if (param == "-r" || param == "-i") {
            if (nParams + 1 >= argn) {
                throw excepcio::entradaIncorrecta("Error: el valor associat a l'opcio '-r' és incorrecte.");
            }

            int numPortes = stoi(argv[++nParams]);

            if (param == "-r") p.Pr = numPortes;
            else p.Pi = numPortes;
        } else if (param == "-ho" || param == "-ht") {
            if (nParams + 1 >= argn) {
                throw excepcio::entradaIncorrecta("Error: el formato de la hora es incorrecto.");
            }

            string hora = argv[++nParams];
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
        } else if (nParams == argn - 1) {
            // TODO: excepción de si no está incluido el path
            // Asume que el último parámetro es el path
            p.path = argv[nParams];
        }
        nParams++;
    }

    // Comprobación final si el path fue procesado
    if (p.path.empty() && !p.ajuda) {
        throw excepcio::entradaIncorrecta("Error: falta el nom del fitxer.");
    }
    return p;
}

int llegirDades(const string &path, vector<Vol> &vols) {
    ifstream f(path.c_str());
    if (!f.is_open()) {
        throw excepcio::entradaIncorrecta(
            "Error : El fitxer [ " + path + " ] no es pot obrir . Repassa el nom i permisos ."
        );
    }

    string linea;
    vector<string> campos;
    int nDades = 0;

    getline(f, linea); // saltar cabezera

    while (getline(f, linea)) {
        try {
            campos.clear();
            campos = tokens(linea, '\t', false);

            int id = stoi(campos[0]);
            char tipus = campos[1][0];
            char mida = campos[2][0];
            string tv1 = campos[3];
            string tv2 = campos[4];

            if (!validarDades(id, tipus, mida, tv1, tv2)) {
                throw excepcio::errorLectura("Error llegint les dades dels vols.");
            }
            Vol v(id, tipus, mida, tv1, tv2);
            vols.emplace_back(v);
            nDades++;
        } catch (...) {
            throw excepcio::errorLectura("Error leyendo los datos de vuelos.");
        }
    }
    f.close();
    return nDades;
}

void mostrarResultat(const Solucio &sol, const duration<double> &duracio) {
    // Implementación de la lógica de visualización de resultados
    sol.mostrarSolucio();
    cout << "Temps: " << duracio.count() << " segons" << endl;
}
