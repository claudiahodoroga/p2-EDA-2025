//
// Created by claud on 03/12/2025.
//

#include "Candidats.h"

int Candidats::actual() const {
    if (!esFiSlots()) {
        return _idxSlot;
    }
    return -1;
}

void Candidats::seguent() {
    if (!esFiSlots()) {
        _idxSlot++;
    }
    else cerr << "no hay más slots disponibles" << endl;
}

bool Candidats::esFiSlots() const {
    return _idxSlot > _maxSlots;
}
