//
// Created by claud on 03/12/2025.
//

#include "Candidats.h"


Candidats::Candidats(int numPortes, int maxSlots, bool potCrearPorta)
    : _idxPorta(0), _idxSlot(0), _numPortes(numPortes), _maxSlots(maxSlots), _potCrearPorta(potCrearPorta){
    // si no existen puertas y no se pueden crear nuevas, acabar
    if (_numPortes <= 0 && !_potCrearPorta) _esFi = true;
}

pair<int, int> Candidats::actual() const {
    if (_esFi) {
        return{-1,-1}; // devolver candidato inválido
    }
    return {_idxPorta, _idxSlot};
}

void Candidats::seguent() {
    if (!_esFi) {
        // probar slot siguiente en puerta actual
        _idxSlot++;
        if (_idxSlot >= _maxSlots) {
            // probar siguiente puerta si se han probado todos los slots de la acual
            _idxSlot = 0;
            _idxPorta++;

            // si se han probado todas las puertas existentes, intentar crear una nueva
            if (_potCrearPorta && _idxPorta == _numPortes) { // segunda condición significa que se crea una puerta nueva
                _idxSlot = 0;
                _idxPorta = _numPortes;
                _potCrearPorta = false;
            }
            else {
                // ya no quedan candidatos
                _esFi = true;
            }
        }
    }
}

bool Candidats::esFi() const {
    return _esFi;
}
