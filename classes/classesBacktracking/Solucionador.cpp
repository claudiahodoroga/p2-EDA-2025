//
// Created by claud on 03/12/2025.
//

#include "Solucionador.h"

// SOLUCIONADOR VORAZ
void SolucionadorVoraz::solucionar(SolucioVoraz& sol) {
    if (!sol.assignarVolsVoraz()) {
        cerr << "No s'ha pogut trobar una solució voraç." << endl;
    }
}

// SOLUCIONADOR UNA SOLUCIÓN
void SolucionadorUna::solucionar(Solucio& solBase) {
    SolucioUna& sol = dynamic_cast<SolucioUna&>(solBase);
    _trobat = false;
    backtracking(sol);
}

void SolucionadorUna::backtracking(SolucioUna& actual) {

    Candidats cands = actual.inicialitzarCandidats();

    while (!cands.esFi() && !_trobat) {
        pair<int, int> candidat = cands.actual();
        int idxPorta = candidat.first;
        int idxSlot = candidat.second;
        if (actual.acceptable(idxPorta, idxSlot)) {
            actual.anotar(idxPorta, idxSlot);
            if (!actual.completa()) {
                backtracking(actual);
                if (!_trobat) {
                    actual.desanotar(idxPorta, idxSlot);
                }
            }
            else {
                _trobat = true;
            }
        }
        cands.seguent();
    }
}

// BACKTRACKING MILLOR SOLUCIO
void SolucionadorMillor::solucionar(Solucio& solBase) {
    SolucioMillor& sol = dynamic_cast<SolucioMillor&>(solBase);
    // variable local para optima por errores de memoria
    SolucioMillor optima(sol.obtMaxPortesReg(), sol.obtMaxPortesInter(), sol.minToHora(sol.obtHo()), sol.minToHora(sol.obtHt()), sol.obtVols());

    _millorSlotsInactius = INT_MAX;
    _millorMinGap = -1;
    _teMillor = false;

    backtracking(sol, optima);
    // copiar óptima para mostrarla
    if (_teMillor) {
        sol = optima;
    }
}

bool SolucionadorMillor::teMillorSolucio() const {
    return _teMillor;
}

void SolucionadorMillor::backtracking(SolucioMillor& actual, SolucioMillor& optima) {
    Candidats cand = actual.inicialitzarCandidats();

    while (!cand.esFi()) {
        pair<int, int> portaSlot = cand.actual();
        int idxPorta = portaSlot.first;
        int idxSlot = portaSlot.second;
        if (actual.acceptable(idxPorta, idxSlot) &&
            actual.potSerMillor(idxPorta, idxSlot, _millorSlotsInactius)) {

            actual.anotar(idxPorta, idxSlot);

            if (!actual.completa()) {
                backtracking(actual, optima);
            }

            else {
                if (actual.esMillor(_millorSlotsInactius, _millorMinGap)) {
                    optima = actual;
                    _millorSlotsInactius = actual.calcuarSlotsInactius();
                    _millorMinGap = actual.calcularMinGap();
                    _teMillor = true;
                }
            }
            actual.desanotar(idxPorta, idxSlot);
        }
        cand.seguent();
    }
}




