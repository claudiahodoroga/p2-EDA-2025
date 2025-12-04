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
// asume que los datos han sido validados
    Vol(int id, char desti, char capacitat, const string &horaInMin, const string &horaFiMax)
        : _id(id), _desti(desti), _capacitat(capacitat), _horaInMin(horaInMin), _horaFiMax(horaFiMax) {
        // cálculo de slots de uso
        if (capacitat=='p') _slotsUs = 4;
        else if (capacitat=='m') _slotsUs = 6;
        else _slotsUs = 8;
    }

    int obtSlotsUs() const;

    char obtCapacitat() const;

    string obtHoraInMin() const;

    string obtHoraFiMax() const;

    char obtDesti() const;

    bool operator<(const Vol &v) const;

private:
    int _id; ///< identificador del vuelo
    char _desti; ///< destino regional o internacional (r o i)
    char _capacitat; ///< capacidad pequeña, meidana o grande (p, m, g). afecta al número de slots necesarios para el servicio
    int _slotsUs; ///< número de slots necesarios para el servicio en función de la capacidad. p => 4, m => 6, g => 8
    string _horaInMin; ///< hora mínima de inicio del servicio
    string _horaFiMax; ///< hora mínima de fin del servicio
};


#endif //P2_EDA_2025_VOL_H
