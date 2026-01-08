//
// Created by claud on 03/12/2025.
//

#include <algorithm>
#include "Porta.h"


void Porta::afegirVol(const Vol &v, int slotInici) {
    // añadir vuelo a la lista
    _vols.push_back(v);
    int idxVol = _vols.size(); // indexar comenzando por el 1 (0 vacío)
    // guardar inicio del vuelo
    _volSlotsInicials[v.obtId()] = slotInici;
    // marcar slots como ocupados
    int nSlots = v.obtSlotsUs();
    for (int i=0; i<nSlots; i++) {
        _slots[slotInici+i] = idxVol;
    }

    _nSlotsOcupats += nSlots;
    _nVols++;
}

void Porta::quitarVol(const Vol &v, int slotInici) {
    int volId = v.obtId();
    // comprobar si el vuelo existe en la puerta
    map<int,int>::const_iterator itSlot = _volSlotsInicials.find(volId);
    if (itSlot != _volSlotsInicials.end()) {
        int slotIniciAfegit = itSlot->second;
        int nSlots = v.obtSlotsUs();

        // limpiar slots
        for (int i=0; i<nSlots; i++) {
            _slots[slotIniciAfegit+i] = 0;
        }

        _nSlotsOcupats -= nSlots;
        _volSlotsInicials.erase(itSlot);

        // eliminar del vector de vuelos
        vector<Vol>::iterator itVol = find_if(
            _vols.begin(),
            _vols.end(),
            [volId](const Vol &v) {
                return v.obtId() == volId;
            }
        );

        if (itVol != _vols.end()) {
            _vols.erase(itVol);
            _nVols--;
        }
    }
}

int Porta::obtNSlotsOcupats() const {
    return _nSlotsOcupats;
}

int Porta::obtSlot(int idx) const {
    if (idx < 0 || idx >= _slots.size()) return -1;
    return _slots[idx];
}

char Porta::obtTipus() const {
    return _tipus;
}

int Porta::calcularMinGap() const {
    if (_nVols <= 1) return -1; // no hay gap si hay 0 o 1 vuelo
    int minGap = INT_MAX;
    int anteriorFi = -1;

    // encontrar rangos de slots ocupados
    for (int i=0; i< _slots.size(); i++) {
        if (_slots[i] != 0) {
            int inici = i;
            while (i< _slots.size() && _slots[i]!=0) {
                i++;
            }
            int fi = i;
            if (anteriorFi != -1) {
                int gap = inici - anteriorFi; // gap entre el final de vuelo anterior y principio del actual
                if (gap<minGap) {
                    minGap = gap;
                }
            }
            anteriorFi = fi;
            i--;
        }
    }
    if (minGap == INT_MAX) {
        return -1;
    }
    else return minGap;
}

void Porta::mostrar(int ho, const vector<Vol> &vols) const {
    cout << _nVols << " vols *" << endl;
    cout << "* Slots ocupats =" << _nSlotsOcupats
         << " Min. gap =";

    int gap = calcularMinGap();
    if (gap == -1) {
        cout << "--";
    } else {
        cout << gap;
    }
    cout << " *" << endl;
    cout << "* -------------------------------------*" << endl;

    // mostrar info de  cada vuelo
    for (int i=0; i<_nVols; i++) {
        int volId = _vols[i].obtId();

        // obtener slot de inicio para este vuelo
        map<int, int>::const_iterator it = _volSlotsInicials.find(volId);
        if (it != _volSlotsInicials.end()) {
            int slotInici = it->second;
            int slotsUs = _vols[i].obtSlotsUs();

            // calcular hora de salida en minutos
            int horaSortida = ho + ((slotInici + slotsUs)*15);

            _vols[i].mostrar(horaSortida);
        }
    }
}
