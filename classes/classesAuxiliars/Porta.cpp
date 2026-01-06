//
// Created by claud on 03/12/2025.
//

#include "Porta.h"

void Porta::afegirVol(const Vol &v) {

}

void Porta::quitarVol(const Vol &v) {
}

int Porta::obtNSlotsOcupats() const {
    return _nSlotsOcupats;
}

int Porta::obtSlot(int idx) const {
    return _slots[idx];
}

char Porta::obtTipus() const {
    return _tipus;
}