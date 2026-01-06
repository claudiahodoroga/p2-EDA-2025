//
// Created by claud on 03/12/2025.
//

#include "Solucionador.h"

void Voraz::solucionar(Solucio &sol) {
    Candidats cand = sol.inicialitzarCandidats();
    while (!sol.completa() && !cand.esFiSlots()) {
        if (sol.completable(cand.actual())) {
            sol.anotar(cand.actual());
        }
    }

    if (sol.completa()) {
         cerr << "solucion voraz completa" << endl;
    }
    else {
        cerr << "no hay solucion voraz" << endl;
    }
}

// modificar solución inicial para que al terminar la recursividad, contenga la solución final
void BacktrackingUnaSol::solucionar(const Solucio &inicial) {
    encertat = false;
    actual = inicial;
    backtrackingUna(actual);
    // mostrar sol?
}

// TODO: revisar que la estructura tenga sentido con el problema pq no sé qué hago
void BacktrackingUnaSol::backtrackingUna() {
    // inicializar cand
    Candidats cand = actual.inicialitzarCandidats();
    // mientras queden cand y no encertat:
    while (!cand.esFiSlots() && !actual.completa()) {
        if (actual.acceptable(cand.actual())) {
            actual.anotar(cand.actual());
            if (!actual.completa()) {
                backtrackingUna(actual);
                if (!encertat) {
                    sol.desanotar(cand.actual());
                }
            }
            else encertat = true;
        }
        cand.seguent();
    }
}


void BacktrackingMillorSol::solucionar(const Solucio &inicial) {
    Solucio actual = optima = inicial;
    backtrackingMillor(actual);
    // mostrar sol?
}

void BacktrackingMillorSol::backtrackingMillor(const Solucio &actual) {
    // inicializar candidatos
    Candidats cand = actual.inicialitzarCandidats();

    while (!cand.esFiSlots()) {
        if (actual.acceptable(cand.actual())) { // TODO: añadir condición de potSerMillor
            actual.anotar(cand.actual());
            if (!actual.completa()) {
                backtrackingMillor(actual);
            }
            else if (actual.esMillor(optima)) {
                optima = actual;
            }
            actual.desanotar(cand.actual());
        }
        actual.seguent();
    }
}





