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

// TODO: añadir condición de completa si cal
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
            _trobat = true;
        }
        cands.seguent();
    }
}

// BACKTRACKING MILLOR SOLUCIO
void SolucionadorMillor::solucionar(Solucio& solBase) {
    SolucioMillor& sol = dynamic_cast<SolucioMillor&>(solBase);
    // inicializar optima como copia vacía
    _optima = new SolucioMillor(sol);
    _millorSlotsInactius = INT_MAX;
    _millorMinGap = -1;
    _teMillor = false;

    backtracking(sol);
    // copiar óptima para mostrarla
    if (_teMillor) {
        sol = *_optima;
    }

    delete _optima;
    _optima = nullptr;
}

bool SolucionadorMillor::teMillorSolucio() const {
    return _teMillor;
}

// TODO: añadir condición de es millor
void SolucionadorMillor::backtracking(SolucioMillor& actual) {
    Candidats cand = actual.inicialitzarCandidats();

    while (!cand.esFi()) {
        pair<int, int> portaSlot = cand.actual();
        int idxPorta = portaSlot.first;
        int idxSlot = portaSlot.second;
        if (actual.acceptable(idxPorta, idxSlot) &&
            actual.potSerMillor(idxPorta, idxSlot, _millorSlotsInactius)) {

            actual.anotar(idxPorta, idxSlot);

            if (!actual.completa()) {
                backtracking(actual);
            }

            else {
                if (actual.esMillor(_millorSlotsInactius, _millorMinGap)) {
                    *_optima = actual;
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




