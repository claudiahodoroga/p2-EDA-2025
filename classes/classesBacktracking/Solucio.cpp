//
// Created by claud on 03/12/2025.
//

#include "Solucio.h"

// función auxiliar para convertir las horas a minutos
int Solucio::horaToMin(const string& h) const {
    int hh = stoi(h.substr(0, 2));
    int mm = stoi(h.substr(3, 2));
    return hh * 60 + mm;
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

bool Solucio::potCrearPorta(char tipus) const {
    if (tipus == 'i') {
        return _nPortesInter < _maxPortesInter;
    }
    else {
        return _nPortesInter < _maxPortesInter;
    }
}

void Solucio::crearNovaPorta(char tipus) {
    _portes.push_back(Porta(tipus, _maxSlots));

    if (tipus == 'i') {
        _nPortesInter++;
    }
    else _nPortesReg++;
}

// TODO: arreglar formato del bucle
int Solucio::calcuarSlotsInactius() const {
    int slotsDispTotals = _portes.size() * _maxSlots;
    int slotsOcupats = 0;
    for (const auto& porta: _portes) {
        slotsOcupats += porta.obtNSlotsOcupats();
    }
    return slotsDispTotals - slotsOcupats;
}

// TODO: arreglar formato del método e implementar función en porta
int Solucio::calcularMinGap() const {
    int minGap = INT_MAX;
    for (const auto& porta: _portes) {
        int gapPorta = porta.calcularMinGap();
        if (gapPorta>=0 && gapPorta<minGap) {
            minGap = gapPorta;
        }
    }
    return (minGap== INT_MAX ? -1 : minGap);
}

// TODO: comprobar formatos de la función
void Solucio::mostrarSolucio() const {
    for (size_t i = 0; i < _portes.size(); i++) {
        cout << "****************************************" << endl;
        cout << "* Porta " << (i+1) << " ("
             << (_portes[i].obtTipus() == 'i' ? "INT" : "REG") << ") ";
        // Continue with formatting as per your specification
        // _portes[i].mostrar();
    }

    cout << "Num. portes: " << _portes.size()
         << " (INT.: " << _nPortesInter << ")" << endl;
    cout << "Slots inactivitat: " << calcuarSlotsInactius() << endl;
    cout << "Mínim gap: " << calcularMinGap() << endl;
}

// SOLUCIÓN DE MÉTODO VORAZ
bool SolucioVoraz::assignarVolsVoraz() {
    //  TODO: ordenar vuelos según algún tipo de criterio

    for (int i=0; i<_vols.size(); i++) {
        if (!assignarVol(i)) {
            return false; // si no se puede asignar un vuelo, parar
        }
    }
    return true; // todos los vuelos están correctamente asignados
}

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
        idxSlot = 0;
        // buscar primer slot válido en la nueva puerta
        int horaInMin = horaToMin(vol.obtHoraInMin());
        int primeraHoraPosible = horaInMin - (vol.obtSlotsUs() * 15);
        if (primeraHoraPosible > _ho) {
            idxSlot = (primeraHoraPosible - _ho) / 15;
        }
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
            for (int idxSlot=0; idxSlot < _maxSlots-slotsNecessaris+1; idxSlot++) {
                bool disponible = true;
                for (int i=0; i<slotsNecessaris; i++) {
                    if (p.obtSlot(idxSlot+i) != 0) {
                        disponible = false;
                        break; // NOOOO
                    }
                }
                if (disponible) {
                    int horaServei = _ho + (idxSlot * 15);
                    int horaSortida = horaServei + (slotsNecessaris * 15);

                    if (horaSortida >= horaInMin && horaSortida <= horaFiMax) return {idxPorta, idxSlot};
                }
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
    if (_niv > 0 && _niv <= _vols.size()) {
        const Vol& vol = _vols[_niv];
        _portes[idxPorta].quitarVol(vol);
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

bool SolucioBacktracking::slotsDisponibles(int idxPorta, int slotInicio, int numSlots) const {
    if (slotInicio < 0 || slotInicio + numSlots > _maxSlots) {
        return false;
    }

    const Porta& porta = _portes[idxPorta];
    for (int i=0; i<numSlots; i++) {
        if (porta.obtSlot(slotInicio + i) !=0) {
            return false;
        }
    }
    return true;
}

bool SolucioBacktracking::compatibleAmbFinestra(int slotInicio, const Vol &vol) const {
    int horaServei = _ho + (slotInicio * 15);
    int horaSortida = horaServei + (vol.obtSlotsUs() *15);
    int horaInMin = horaToMin(vol.obtHoraInMin());
    int horaFiMax = horaToMin(vol.obtHoraFiMax());
    // hora de salida debe estar dentro del rango horario del vuelo
    return (horaSortida >= horaInMin && horaSortida <= horaFiMax);
}

// MEJOR SOLUCIÓN
bool SolucioMillor::esMillor() const {
    int slotsInactius = calcuarSlotsInactius();
    int minGap = calcularMinGap();

    // minimizar slots inactivos
    if (slotsInactius < _millorSlotsInactius) {
        return true;
    }

    if (slotsInactius == _millorSlotsInactius) {
        if (minGap < _millorMinGap) {
            return true;
        }
    }
    return false;
}

// TODO: puede ser mejor se calcula con el candidato actual, para saber si merece la pena usarlo
bool SolucioMillor::potSerMillor() const {
    if (_millorSlotsInactius == INT_MAX) {
        return true; // todavía no se ha encontrado una solución
    }

    int slotsInactiusActuals = calcuarSlotsInactius(); // solución actual tendrá como mínimo esta cantidad de slots inactivos
    return slotsInactiusActuals <= _millorSlotsInactius; // TODO: mejorar este criterio...
}

void SolucioMillor::guardarMillor() {
    _millorSlotsInactius = calcuarSlotsInactius();
    _millorMinGap = calcularMinGap();
    _millorPortes = _portes;
}
