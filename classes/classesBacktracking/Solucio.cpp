//
// Created by claud on 03/12/2025.
//

#include "Solucio.h"

// función auxiliar para convertir las horas a minutos
int horaToMin(const string& h) {
    int hh = stoi(h.substr(0, 2));
    int mm = stoi(h.substr(3, 2));
    return hh * 60 + mm;
}


Solucio::Solucio(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht, char destiInicial) {
    _niv = 0;
    _slotsInactius = 0;

    _maxPortesReg = maxPortesReg;
    _maxPortesInter = maxPortesInt;
    _ho = horaToMin(Ho);
    _ht = horaToMin(Ht);

    // calcular numero de slots en función de las horas de apertura del aeropuerto
    int duracio = _ht - _ho; // tiempo de apertura del aeropuerto en minutos
    int nSlots = duracio / 15;

    _maxSlots = nSlots;

    if (destiInicial=='r') {
        _nPortesReg = 1;
        _nPortesInter = 0;
    }
    else {
        _nPortesReg = 0;
        _nPortesInter = 1;
    }

    _portes.resize(1, Porta(destiInicial, _maxSlots)); // vector se irá agrandando de forma dinámica
}

Candidats Solucio::inicialitzarCandidats() const {
    return Candidats(_maxSlots);
}

/**
 * será aceptable si:
 * - hay una puerta compatible con slots disponibles
 * - se puede crear una puerta nueva compatible
 * - dentro de la puerta, hay un número de slots disponibles en el rango de horas del vuelo
 */

bool Solucio::acceptable(char tipus, int horaIniciMinuts, int horaFiMinuts, int slotsUs) const {
    bool acceptable = false;
    // buscar si existe puerta compatible
    int porta = 1;
    bool trobat = false;
    while (porta < _portes.size() && not trobat) {
        // si la puerta es del mismo tipo que el vuelo y tiene suficientes slots libres, ver si se puede usar
        if (_portes[porta].obtTipus() == tipus && _portes[porta].obtNSlotsOcupats() >= slotsUs) {
            // comprobar si tiene slots disponibles en el rango de horas del vuelo
            int slot = 0;
            bool esRangInici = false;
            int minutsSlot = 0;
            while (slot < _maxSlots && not esRangInici) {
                // convertir índice de slot a minutos?
                if ((_ho+minutsSlot) >= horaIniciMinuts) esRangInici = true; // cada slot = 15 minutos. si es <= a horaIniciMinuts, significa que está dentro del rango de inicio
                else {
                    slot++;
                    minutsSlot += 15;
                }
            }

            // si es rango de inicio, comprobar que se puedan ocupar los suficientes slots y que esté en el rango de final del servicio
            if (esRangInici) {
                bool teSlotsSuficientes = true;
                while (slot < _maxSlots && slot < slotsUs && teSlotsSuficientes) {
                    if (_portes[porta].obtSlot(slot)==0) algo; // nose
                }
            }
        }
    }
}



// anotar un candidato significa que se le asignan los slots a un vuelo
// y se pasa al siguiente vuelo
void Solucio::anotar(const pair<int, int> &cand) {

    // acceder a vector<Porta> _portes; puerta correspondiente
    // guardar el vuelo en el vector de vols para poder acceder a su información mediante el índice
    // marcar los slots que tocan para el candidato como ocupadas por el índice del vuelo en cuestión

}
