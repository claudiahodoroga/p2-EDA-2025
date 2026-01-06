//
// Created by claud on 03/12/2025.
//

#include "Solucionador.h"

// TODO: pasar solución inicial a los métodos...

// SOLUCIONADOR VORAZ
bool SolucionadorVoraz::solucionar() {
    return _sol.assignarVolsVoraz();
}

void SolucionadorVoraz::mostrarResultat() const {
    _sol.mostrarSolucio();
}

// SOLUCIONADOR UNA SOLUCIÓN
bool SolucionadorUna::solucionar() {
    _trobat = false;
    backtracking();
    return _trobat;
}

void SolucionadorUna::backtracking() {
    Candidats cand = _sol.inicialitzarCandidats();

    while (!cand.esFi() && !_trobat) {
        pair<int, int> portaSlot = cand.actual();
        if (_sol.acceptable(portaSlot.first, portaSlot.second)) {
            _sol.anotar(portaSlot.first, portaSlot.second);
            backtracking();
            if (!_trobat) {
                _sol.desanotar(portaSlot.first, portaSlot.second);
            }
        }
        cand.seguent();
    }
}

void SolucionadorUna::mostrarResultat() const {
    if (_trobat) {
        _sol.mostrarSolucio();
    }
    else throw("No hi ha solució per la configuració indicada");
}

// BACKTRACKING MILLOR SOLUCIO
bool SolucionadorMillor::solucionar() {
    _trobatAlguna = false;
    backtracking();
    return _trobatAlguna;
}

void SolucionadorMillor::backtracking(SolucioMillor& sol) {
    Candidats cand = sol.inicialitzarCandidats();

    while (!cand.esFi()) {
        pair<int, int> portaSlot = cand.actual();
        if (sol.acceptable(portaSlot.first, portaSlot.second) &&
            sol.potSerMillor()) {
            sol.anotar(portaSlot.first, portaSlot.second);
            backtracking(sol);
            sol.desanotar(portaSlot.first, portaSlot.second);
        }
        cand.seguent();
    }
}

void SolucionadorMillor::mostrarResultat(const SolucioMillor& sol) const {
    if (_trobatAlguna) {
        sol.mostrarSolucio();
    }
    else throw("No hi ha solució per la configuració indicada.");
}























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





