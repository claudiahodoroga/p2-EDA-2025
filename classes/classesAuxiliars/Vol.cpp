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

int Vol::obtId() const {
    return _id;
}

bool Vol::operator<(const Vol &v) const {
    return _id < v._id;
}

void Vol::mostrar(int horaSortida) const {
    int hh = horaSortida / 60;
    int mm = horaSortida % 60;
    char sortida[6];
    sprintf(sortida, "%02d:%02d", hh, mm);

    cout << "* Vol: " << _id
         << " [" << _horaInMin << "-" << _horaFiMax << "] "
         << (_desti == 'i' ? "INT" : "REG") << " / ";

    if (_capacitat == 'p') cout << "Petit";
    else if (_capacitat == 'm') cout << "Mitja";
    else cout << "Gran";

    cout << " (" << _slotsUs << " sl.) *" << endl;
    cout << "* + Sortida: " << sortida << " *" << endl;
}
