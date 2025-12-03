//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_PORTA_H
#define P2_EDA_2025_PORTA_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

#include "Vol.h"

using namespace std;

class Porta {
    public:
        Porta();
        Porta(int id, char tipus, int nSlots)
            : _id(id), _tipus(tipus), _slots(nSlots) {}
        void afegirVol(const Vol& v);
        void quitarVol(const Vol& v);
        int obtNSlotsOcupats();
        vector<Vol> obtVols();
        int obtSlot(int idx); // devuelve número guardado en un slot idx; 0 => sin ocupar, >0 => ocupado
    private:
        int _id;
        char _tipus; // r, i
        int _nVols;
        int _nSlotsOcupats;
        vector<int> _slots; // contiene los índices a los vuelos que ocupan los slots
        vector<Vol> _vols; // contiene la información de los vuelos indexados
    // falta el min gap
};


#endif //P2_EDA_2025_PORTA_H