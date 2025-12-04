//
// Created by claud on 03/12/2025.
//

#include "Solucionador.h"

void Voraz::solucionar(const Solucio &sol) {

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

