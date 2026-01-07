//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_PORTA_H
#define P2_EDA_2025_PORTA_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <vector>

#include "Vol.h"

using namespace std;

class Porta {
    public:
        Porta() : _tipus('r'), _nVols(0), _nSlotsOcupats(0) {}
        Porta(char tipus, int nSlots)
            : _tipus(tipus), _slots(nSlots, 0), _nSlotsOcupats(0), _nVols(0) {}
        void afegirVol(const Vol& v, int slotInici); ///< Añade un vuelo @param v a su slot de inicio @param slotInici
        void quitarVol(const Vol& v, int slotInici); ///< Quita un vuelo de _vols y actualiza _slots, _nVols, _nSlotsOcupats
        int obtNSlotsOcupats() const; ///< Devuelve el número de slots ocupados que tiene la puerta
        int obtSlot(int idx) const; ///< devuelve número guardado en un slot idx; 0 => sin ocupar, >0 => ocupado por un vuelo de _vols
        char obtTipus() const; ///< Devuelve el tipo de la puerta ('r' o 'i')
        int calcularMinGap() const; ///< Calcula el espacio mínimo entre vuelos consecutivos
        void mostrar(int ho, const vector<Vol>& vols) const; ///< Muestra la información de la puerta

    private:
        char _tipus; ///< tipo de puerta, regional o internacional; r, i
        int _nVols; ///< número total de vuelos
        int _nSlotsOcupats; ///< número de slots ocupados, para saber si hay disponibles
        vector<int> _slots; ///< contiene los índices a los vuelos en los slots ocupados por dichos vuelos
        vector<Vol> _vols; ///< índice de vuelos con su información para referenciarla
        map<int, int> _volSlotsInicials; ///< Mapeo de un id de vuelo a un slot de inicio
    // falta el min gap
};


#endif //P2_EDA_2025_PORTA_H