//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_CANDIDATS_H
#define P2_EDA_2025_CANDIDATS_H

#include <iostream>
#include <utility>

using namespace std;

/**
 * Candidats: Generates all possible (gate, slot) assignments for current flight
 *
 * For a flight at level `niv`, we try:
 * 1. All existing gates, all compatible slots
 * 2. Creating a new gate (if allowed), starting at earliest valid slot
 *
 * A candidate is represented as pair<int, int>:
 * - first: gate index (or numGates to indicate "create new gate")
 * - second: starting slot index
 */

class Candidats {
    public:
    /**
     * Constructor
     * @param numGates Current number of gates in solution
     * @param maxSlots Maximum slots per gate
     * @param canCreateGate Whether we can create a new gate
     */
        Candidats(int numGates, int maxSlots, bool potCrearPorta)
            : _idxSlot(0), _maxSlots(maxSlots) {}
        pair<int, int> actual() const; ///< Obtener candidato actual
        void seguent();
        bool esFi() const;
    private:
        int _idxPorta; ///< Indice de la puerta que se está considerando
        int _idxSlot; ///< Slot desde el que se está considerando
        int _numPortes; ///< Número de puertas existentes
        int _maxSlots; ///< Máximo número de slots por puerta
};


#endif //P2_EDA_2025_CANDIDATS_H