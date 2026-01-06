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
// TODO: implementar afegirVol y quitarVol

using namespace std;

class Porta {
    public:
        Porta();
        Porta(char tipus, int nSlots)
            : _tipus(tipus), _slots(nSlots), _nSlotsOcupats(0), _nVols(0) {}
        void afegirVol(const Vol& v); ///< añade un vuelo a _vols y actualiza _slots, _nVols, _nSlotsOcupats
        void quitarVol(const Vol& v); ///< quita un vuelo de _vols y actualiza _slots, _nVols, _nSlotsOcupats
        int obtNSlotsOcupats() const; ///< devuelve el número de slots ocupados que tiene la puerta
        //vector<Vol> obtVols();
        int obtSlot(int idx) const; ///< devuelve número guardado en un slot idx; 0 => sin ocupar, >0 => ocupado por un vuelo de _vols
        char obtTipus() const;

    private:
        char _tipus; ///< tipo de puerta, regional o internacional; r, i
        int _nVols; ///< número total de vuelos
        int _nSlotsOcupats; ///< número de slots ocupados, para saber si hay disponibles
        vector<int> _slots; ///< contiene los índices a los vuelos en los slots ocupados por dichos vuelos
        vector<Vol> _vols; ///< índice de vuelos con su información para referenciarla
    // falta el min gap
};


#endif //P2_EDA_2025_PORTA_H