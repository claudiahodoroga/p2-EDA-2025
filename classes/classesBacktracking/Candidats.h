//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_CANDIDATS_H
#define P2_EDA_2025_CANDIDATS_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

using namespace std;

class Candidats {
    public:
    // primer slot de la puerta porta
        Candidats(int porta, int maxSlots, int maxPortes)
            : _idxPorta(porta), _idxSlot(0), _maxSlots(maxSlots), _maxPortes(maxPortes) {}
        pair<int, int> actual() const;
        void seguent();
        bool esFiSlots() const;
        bool esFiPortes() const;
    private:
    // se accede a la solución, que contiene el vector de puertas con sus slots
        int _idxPorta; // guarda la puerta que se está considerando
        int _idxSlot; // guarda el índice de slot que se está considerando
        int _maxSlots; // numero máximo de slots
        int _maxPortes; // máximas puertas internacionales
};


#endif //P2_EDA_2025_CANDIDATS_H