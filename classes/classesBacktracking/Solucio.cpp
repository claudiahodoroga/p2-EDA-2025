//
// Created by claud on 03/12/2025.
//

#include "Solucio.h"
#include <algorithm>

// función auxiliar para convertir las horas a minutos
int Solucio::horaToMin(const string& h) const {
    int hh = stoi(h.substr(0, 2));
    int mm = stoi(h.substr(3, 2));
    return hh * 60 + mm;
}

// función auxiliar para convertir de minutos a HH::MM
string Solucio::minToHora(int minuts) const {
    int h = minuts / 60;
    int m = minuts % 60;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", h, m);
    return string(buffer);
}

Solucio::Solucio(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht, const vector<Vol>& vols)
    : _vols(vols), _maxPortesReg(maxPortesReg), _maxPortesInter(maxPortesInt), _nPortesInter(0), _nPortesReg(0){

    _ho = horaToMin(Ho);
    _ht = horaToMin(Ht);

    // calcular número de slots según horario de apertura del aeropuerto
    int duracio = _ht - _ho;
    _maxSlots = duracio / 15; // slots de 15 minutos

    _portes.clear();
}

int Solucio::obtNumPortes() const {
    return _portes.size();
}

int Solucio::obtNumPortesInter() const {
    return _nPortesInter;
}

int Solucio::obtNumPortesReg() const {
    return _nPortesReg;
}

int Solucio::obtMaxSlots() const {
    return _maxSlots;
}

const vector<Porta> & Solucio::obtPortes() const {
    return _portes;
}

const vector<Vol> & Solucio::obtVols() const {
    return _vols;
}

bool Solucio::potCrearPorta(char tipus) const {
    if (tipus == 'i' || tipus =='I') {
        return _nPortesInter < _maxPortesInter;
    }
    else {
        return _nPortesReg < _maxPortesReg;
    }
}

void Solucio::crearNovaPorta(char tipus) {
    _portes.push_back(Porta(tipus, _maxSlots));

    if (tipus == 'i') {
        _nPortesInter++;
    }
    else _nPortesReg++;
}

int Solucio::calcuarSlotsInactius() const {
    int slotsDispTotals = _portes.size() * _maxSlots;
    int slotsOcupats = 0;
    for (int i=0; i<_portes.size(); i++) {
        slotsOcupats += _portes[i].obtNSlotsOcupats();
    }
    return slotsDispTotals - slotsOcupats;
}

int Solucio::calcularMinGap() const {
    int minGap = INT_MAX;
    for (int i=0; i<_portes.size(); i++) {
        int gapPorta = _portes[i].calcularMinGap();
        if (gapPorta>=0 && gapPorta<minGap) {
            minGap = gapPorta;
        }
    }
    if (minGap == INT_MAX) {
        return -1;
    }
    else return minGap;
}

void Solucio::mostrarSolucio() const {
    for (size_t i = 0; i < _portes.size(); i++) {
        cout << "****************************************" << endl;
        cout << "* Porta " << (i+1) << " ("
             << (_portes[i].obtTipus() == 'i' ? "INT" : "REG") << ") ";
        _portes[i].mostrar(_ho, _vols);
        cout << "****************************************" << endl;
    }

    cout << "Num. portes: " << _portes.size()
         << " (INT.: " << _nPortesInter << ")" << endl;
    cout << "Slots inactivitat: " << calcuarSlotsInactius() << endl;
    cout << "Mínim gap: " << calcularMinGap() << endl;
}

// SOLUCIÓN DE MÉTODO VORAZ
SolucioVoraz::SolucioVoraz(int maxPortesReg, int maxPortesInt, const string& Ho,
                           const string& Ht, const vector<Vol>& vols)
    : Solucio(maxPortesReg, maxPortesInt, Ho, Ht, vols) {
    ordenarVols(); // ordenar vuelos según duración del servicio, con prioridad para los internacionales
}

void SolucioVoraz::ordenarVols() {
    sort(_vols.begin(), _vols.end(), [](const Vol& a, const Vol& b) {
        // primero los vuelos internacionales
        if (a.obtDesti()=='i' && b.obtDesti()=='r') {
            return true;
        }
        if (a.obtDesti()=='r' && b.obtDesti()=='i') {
            return false;
        }

        // si son del mismo tipo, ordenar por duración del servicio
        return a.obtSlotsUs() > b.obtSlotsUs();
    });
}

bool SolucioVoraz::assignarVolsVoraz() {
    for (int i=0; i<_vols.size(); i++) {
        if (!assignarVol(i)) {
            return false; // si no se puede asignar un vuelo, parar
        }
    }
    return true; // todos los vuelos están correctamente asignados
}

// TODO: mejorar condición del bucle
bool SolucioVoraz::assignarVol(int idxVol) {
    const Vol& vol = _vols[idxVol];

    // buscar primera pareja de puerta y vuelo disponibles
    pair<int, int> portaSlot = trobarPrimeraOpcio(vol);
    int idxPorta = portaSlot.first;
    int idxSlot = portaSlot.second;
    if (idxPorta == -1) { // ninguna puerta disponible funciona
        // intentar crear una puerta nueva
        if (!potCrearPorta(vol.obtDesti())) {
            return false; // no se pueden crear más puertas
        }

        crearNovaPorta(vol.obtDesti());
        idxPorta = _portes.size() - 1;

        // buscar primer slot válido en la nueva puerta
        int horaInMin = horaToMin(vol.obtHoraInMin());
        int horaFiMax = horaToMin(vol.obtHoraFiMax());
        int slotsNecessaris = vol.obtSlotsUs();

        int primeraHoraPosible = horaInMin - (slotsNecessaris * 15);
        if (primeraHoraPosible > _ho) {
            idxSlot = (primeraHoraPosible - _ho) / 15;
        }

        // verificar compatibilidad con el rango horario del vuelo
        int horaSortida = _ho + (idxSlot + slotsNecessaris) * 15;
        bool slotValid = (horaSortida >= horaInMin && horaSortida <= horaFiMax);

        int slot = 0;
        while (slot <= _maxSlots - slotsNecessaris && !slotValid) {
            int testSortida = _ho + (slot + slotsNecessaris) * 15;
            if (testSortida >= horaInMin && testSortida <= horaFiMax) {
                idxSlot = slot;
                slotValid = true;
            }
            else slot++;
        }
        if (!slotValid) return false;
    }
    // asignar vuelo
    _portes[idxPorta].afegirVol(vol, idxSlot);
    return true;
}

pair<int, int> SolucioVoraz::trobarPrimeraOpcio(const Vol& vol) const {
    int slotsNecessaris = vol.obtSlotsUs();
    int horaInMin = horaToMin(vol.obtHoraInMin());
    int horaFiMax = horaToMin(vol.obtHoraFiMax());

    // probar todas las puertas existentes
    for (int idxPorta = 0; idxPorta < _portes.size(); idxPorta++) {
        const Porta& p = _portes[idxPorta];
        // comprobar compatibilidad
        if (!(vol.obtDesti() == 'i' && p.obtTipus() == 'r')) { // los internacionales no pueden usar puertas regionales
            // probar cada slot en esta puerta
            int slot = 0;
            while (slot <= _maxSlots - slotsNecessaris) {
                // comprobar si hay slots disponibles
                bool disponible = true;
                int k = 0;
                while (k< slotsNecessaris && disponible) {
                    if (p.obtSlot(slot+k) !=0) {
                        disponible = false;
                    }
                    k++;
                }

                if (disponible) {
                    // comprobar compatibilidad temporal
                    int horaSortida = _ho + ((slot + slotsNecessaris) * 15);
                    if (horaSortida >= horaInMin && horaSortida <= horaFiMax) {
                        return make_pair(idxPorta, horaSortida);
                    }
                }
                slot++;
            }
        }
    }

    return{-1,-1}; // no se ha podido hacer una asignación válida
}


// SOLUCIÓN DE BACKTRACKING
Candidats SolucioBacktracking::inicialitzarCandidats() const {
    bool potCrear = false;

    if (_niv < _vols.size()) {
        char tipusVol = _vols[_niv].obtDesti();
        potCrear = potCrearPorta(tipusVol);
    }

    return Candidats(_portes.size(), _maxSlots, potCrear);
}

bool SolucioBacktracking::acceptable(int idxPorta, int slotInicio) const {
    if (_niv >= _vols.size()) return false;

    const Vol& vol = _vols[_niv];

    // Intentar crear nueva puerta en caso especial
    if (idxPorta == _portes.size()) {
        return potCrearPorta(vol.obtDesti());
    }

    // comprobar validez del índice de puerta
    if (idxPorta < 0 || idxPorta > _portes.size()) return false;

    const Porta& porta = _portes[idxPorta];

    // comprobar compatibilidad
    if (vol.obtDesti() == 'i' && porta.obtTipus() == 'r') {
        return false; // vuelo internacional necesita puerta internacional
    }

    // comprobar si hay slots disponibles
    if (!slotsDisponibles(idxPorta, slotInicio, vol.obtSlotsUs())) {
        return false;
    }

    // comprobar compatibilidad de ventana temporal
    if (!compatibleAmbFinestra(slotInicio, vol)) {
        return false;
    }

    return true;
}

void SolucioBacktracking::anotar(int idxPorta, int slotInicio) {
    if (_niv < _vols.size()) {
        const Vol& vol = _vols[_niv];

        // crear puerta nueva si es necesario
        if (idxPorta == _portes.size()) {
            crearNovaPorta(vol.obtDesti());
        }

        // asignar vuelo a una puerta
        _portes[idxPorta].afegirVol(vol, slotInicio);
        _niv++;
    }
}

void SolucioBacktracking::desanotar(int idxPorta, int slotInicio) {
    _niv--;
    if (_niv >= 0 && _niv < _vols.size()) {
        const Vol& vol = _vols[_niv];
        _portes[idxPorta].quitarVol(vol, slotInicio);
        if (idxPorta == _portes.size()-1 && _portes[idxPorta].obtNSlotsOcupats() == 0) {
            char tipus = _portes[idxPorta].obtTipus();
            _portes.pop_back();

            if (tipus == 'i') {
                _nPortesInter--;
            }
            else {
                _nPortesReg--;
            }
        }
    }
}

bool SolucioBacktracking::completa() const {
    return _niv >= _vols.size(); // se han asignado todos los vuelos
}

int SolucioBacktracking::obtNivell() const {
    return _niv;
}

bool SolucioBacktracking::slotsDisponibles(int idxPorta, int slotInicio, int numSlots) const {
    if (slotInicio < 0 || slotInicio + numSlots > _maxSlots) {
        return false;
    }

    const Porta& porta = _portes[idxPorta];
    for (int i=0; i<numSlots; i++) {
        if (porta.obtSlot(slotInicio + i) !=0) {
            return false; // slot ocupado
        }
    }
    return true;
}

bool SolucioBacktracking::compatibleAmbFinestra(int slotInicio, const Vol &vol) const {
    int slotsNecessaris = vol.obtSlotsUs();
    int horaSortida = _ho + ((slotInicio + slotsNecessaris) * 15);
    int horaInMin = horaToMin(vol.obtHoraInMin());
    int horaFiMax = horaToMin(vol.obtHoraFiMax());
    return (horaSortida >= horaInMin && horaSortida <= horaFiMax);
}

// MEJOR SOLUCIÓN
bool SolucioMillor::esMillor(int optimaSlotsInactius, int optimaMinGap) const {
    int slotsInactius = calcuarSlotsInactius();
    int minGap = calcularMinGap();

    if (slotsInactius < optimaSlotsInactius) {
        return true;
    }

    if (slotsInactius == optimaSlotsInactius) {
        if (minGap > optimaMinGap) {
            return true;
        }
    }
    return false;
}

bool SolucioMillor::potSerMillor(int idxPortaCand, int idxSlotCand, int optimaSlotsInactius) const {
    if (optimaSlotsInactius == INT_MAX) {
        return true; // todavía no se ha encontrado una solución
    }

    const Vol& vol = _vols[_niv];

    // simular añadir el siguiente vuelo
    bool creariaNovaPorta = (idxPortaCand == _portes.size());
    int nPortesSimulat = 0;
    if (!creariaNovaPorta) {
        nPortesSimulat = _portes.size();
    }
    else nPortesSimulat = _portes.size() + 1;

    // calcular slots ocupados
    int slotsOcupatsActuals = 0;
    for (int i=0; i<_portes.size(); i++) {
        slotsOcupatsActuals += _portes[i].obtNSlotsOcupats();
    }

    // añadir los slots del siguiente vuelo que se asignaría
    int slotsOcupatsSimulat = slotsOcupatsActuals + vol.obtSlotsUs();
    int totalSlotsSimulat = nPortesSimulat * _maxSlots;
    int slotsInactiusSimulat = totalSlotsSimulat - slotsOcupatsSimulat;

    // si ya es peor que la óptima, descartar
    if (slotsInactiusSimulat > optimaSlotsInactius) {
        return false;
    }

    // aproximación optimista de los vuelos restantes
    int volsRestants = _vols.size() - _niv - 1;
    if (volsRestants > 0) {
        // calcular número mínimo de slots necesarios para los vuelos restantes
        int minSlotssRestants = 0;
        for (int i= _niv+1; i < _vols.size(); i++) {
            minSlotssRestants += _vols[i].obtSlotsUs();
        }

        // comprobar si hay suficiente capacidad
        int slotsDisponiblesRestants = totalSlotsSimulat - slotsOcupatsSimulat;
        if (slotsDisponiblesRestants < minSlotssRestants) {
            // se necesitarían más puertas
            int portesExtres = (minSlotssRestants - slotsDisponiblesRestants + _maxSlots + 1) / _maxSlots;
            int slotsExtres = portesExtres * _maxSlots;
            slotsInactiusSimulat += slotsExtres - (minSlotssRestants - slotsDisponiblesRestants);

            if (slotsInactiusSimulat > optimaSlotsInactius) {
                return false;
            }
        }
    }
    return true;
}
