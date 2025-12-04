//
// Created by claud on 03/12/2025.
//

#include <iostream>
#include <chrono>
#include <set>

#include "eines/eines.h"
#include "classes/classesBacktracking/Solucio.h"
#include "classes/classesBacktracking/Solucionador.h"
#include "classes/classesAuxiliars/Vol.h"
#include "eines/auxiliars.h" // incluye los métodos auxiliares para el main

using namespace std::chrono;
using namespace std;

int main(int argn, char **argv) {
    try {
        Parametres p = processaParametres(argn, argv);

        if (p.ajuda) {
            mostrarAjuda(argv[0]);
        } else {
            // Validación de path movida a `processaParametres` para una mejor separación de lógica,
            // pero se mantiene aquí el check inicial si se prefiere.
            if (p.path.empty()) {
                throw excepcio::entradaIncorrecta("Error: falta el nom del fitxer.");
            }

            cerr << "Fitxer de entrada de vols: " << p.path << endl;

            set<Vol> vols;
            int dades = llegirDades(p.path, vols);
            cout << " ==> " << dades << " vols llegits." << endl;

            Solucio sol(p.Pr, p.Pi, p.Ho, p.Ht);

            high_resolution_clock::time_point t1 = high_resolution_clock::now();

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

            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            // calcular tiempo de cómputo del algoritmo
            duration<double> duracio = duration_cast< duration<double> >(t2 - t1);

            mostrarResultat(sol, duracio);
        }
    } catch (excepcio::entradaIncorrecta ex) {
        cerr << ex.missatge << endl;
        mostrarAjuda(argv[0]);
    } catch (excepcio::errorLectura ex) {
        cerr << ex.missatge << endl;
    }
    return 0;
}