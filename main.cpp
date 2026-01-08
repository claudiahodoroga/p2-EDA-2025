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
            if (p.path.empty()) {
                throw excepcio::entradaIncorrecta("Error: falta el nom del fitxer.");
            }

            cerr << "Fitxer de entrada de vols: " << p.path << endl;

            vector<Vol> vols;
            int dades = llegirDades(p.path, vols);
            cout << " ==> " << dades << " vols llegits." << endl;

            high_resolution_clock::time_point t1 = high_resolution_clock::now();

            if (p.tipoCalculo == 'v') {
                cerr << "usando solucionador voraz" << endl;

                SolucioVoraz sol(p.Pr, p.Pi, p.Ho, p.Ht, vols);
                SolucionadorVoraz solucionador;
                solucionador.solucionar(sol);

                high_resolution_clock::time_point t2 = high_resolution_clock::now();
                duration<double> duracio = duration_cast<duration<double>>(t2 - t1);

                mostrarResultat(sol, duracio);
            }
            else if (p.tipoCalculo == 'm') {
                cerr << "usando backtracking mejor solución" << endl;

                SolucioMillor sol(p.Pr, p.Pi, p.Ho, p.Ht, vols);
                SolucionadorMillor solucionador;
                solucionador.solucionar(sol);

                high_resolution_clock::time_point t2 = high_resolution_clock::now();
                duration<double> duracio = duration_cast<duration<double>>(t2 - t1);

                if (solucionador.teMillorSolucio()) {
                    mostrarResultat(sol, duracio);
                }
                else {
                    cout << "No hi ha solucio per la configuracio indicada" << endl;
                    cout << "Temps: " << duracio.count() << " segons" << endl;
                }
            }
            else {
                cerr << "usando backtracking una solución" << endl;

                SolucioUna sol(p.Pr, p.Pi, p.Ho, p.Ht, vols);
                SolucionadorUna solucionador;
                solucionador.solucionar(sol);

                high_resolution_clock::time_point t2 = high_resolution_clock::now();
                duration<double> duracio = duration_cast<duration<double>>(t2 - t1);

                if (sol.obtNumPortes()>0) {
                    mostrarResultat(sol, duracio);
                }
                else {
                    cout << "No hi ha solucio per la configuracio indicada" << endl;
                    cout << "Temps: " << duracio.count() << " segons" << endl;
                }
            }
        }
    } catch (excepcio::entradaIncorrecta ex) {
        cerr << ex.missatge << endl;
        mostrarAjuda(argv[0]);
    } catch (excepcio::errorLectura ex) {
        cerr << ex.missatge << endl;
    }
    return 0;
}