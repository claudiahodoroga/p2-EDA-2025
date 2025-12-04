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
    _ho = Ho;
    _ht = Ht;

    // calcular numero de slots en función de las horas de apertura del aeropuerto
    int hoMin = horaToMin(_ho);
    int htMin = horaToMin(_ht);

    int duracio = htMin - hoMin; // tiempo de apertura del aeropuerto en minutos
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

    _portes.resize(1, Porta(0, destiInicial, _maxSlots)); // vector se irá agrandando de forma dinámica
}

Candidats Solucio::inicialitzarCandidats() const {
    return {0, _maxSlots, (_maxPortesInter+_maxPortesReg)};
}

// anotar un candidato significa que se le asignan los slots a un vuelo
// y se pasa al siguiente vuelo
void Solucio::anotar(const pair<int, int> &cand) {

    // acceder a vector<Porta> _portes; puerta correspondiente
    // guardar el vuelo en el vector de vols para poder acceder a su información mediante el índice
    // marcar los slots que tocan para el candidato como ocupadas por el índice del vuelo en cuestión

}
