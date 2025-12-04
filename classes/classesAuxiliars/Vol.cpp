//
// Created by claud on 03/12/2025.
//

#include "Vol.h"

int Vol::obtSlotsUs() const{
    return _slotsUs;
}

char Vol::obtCapacitat() const {
    return _capacitat;
}

string Vol::obtHoraInMin() const {
    return _horaInMin;
}

string Vol::obtHoraFiMax() const{
   return _horaFiMax;
}

char Vol::obtDesti() const {
    return _desti;
}

bool Vol::operator<(const Vol &v) const {
    return _id < v._id;
}
