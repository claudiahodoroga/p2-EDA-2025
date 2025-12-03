//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_VOL_H
#define P2_EDA_2025_VOL_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

#include "Vol.h"

using namespace std;

class Vol {
public:
    Vol();

    Vol(char desti, char capacitat, int slotsUs, const string &horaInMin, const string &horaFiMax)
        : _desti(desti), _capacitat(capacitat), _soltsUs(slotsUs), _horaInMin(horaInMin), _horaFiMax(horaFiMax) {
    }

    int obtSlotsUs();

    string obtCapacitat();

    string obtHoraInMin();

    string obtHoraFiMax();

    char obtDesti();

private:
    char _desti; // r o i
    char _capacitat; // p, m, g
    int _soltsUs; // p => 4, m => 6, g => 8
    string _horaInMin; // hora mínima de inicio del servicio
    string _horaFiMax; // hora mínima de fin del servicio
};


#endif //P2_EDA_2025_VOL_H
