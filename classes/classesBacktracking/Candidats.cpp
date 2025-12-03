//
// Created by claud on 03/12/2025.
//

#include "Candidats.h"

pair<int, int> Candidats::actual() const {
    if (!esFiSlots()) {
        return {_idxPorta, _idxSlot};
    }
    else return{-1, -1};
}

void Candidats::seguent() {
    if (!esFiSlots()) {
        _idxSlot++;
    }
    // si es el final de los slots de la puerta, crear una nueva si no es límite
    else if (!esFiPortes()) {
        _idxPorta++;
        _idxSlot = 0;
    }
    else cerr << "no se pueden crear más puertas" << endl;
}

bool Candidats::esFiSlots() const {
    return _idxSlot > _maxSlots;
}

// se comprueba si se pueden crear más del tipo específico de puerta con la solución
bool Candidats::esFiPortes() const {
    return _idxPorta > _maxPortes;
}
