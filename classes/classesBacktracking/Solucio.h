//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_SOLUCIO_H
#define P2_EDA_2025_SOLUCIO_H

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

#include "Candidats.h"
#include "../classesAuxiliars/Porta.h"


// la solución es una serie de puertas con vuelos asignados a sus slots

class Solucio {
public:
    Solucio(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht, char destiInicial);
    // inicializa candidatos con el número máximo de puertas regionales, internacionales, y número de slots máximos según las horas activas del aeropuerto
    Candidats inicialitzarCandidats() const;
    bool acceptable(const pair<int,int>& cand) const;
    void anotar(const pair<int,int>& cand);
    void desanotar(const pair<int, int>& cand);
    bool completa() const;
    bool completable(const pair<int,int> & cand) const;
    //bool millorable(const Candidats& candidats) const;

private:
    vector<Porta> _portes; // ASIGNACIÓN FINAL, cada puerta contiene n slots disponibles
    int _slotsInactius; ///< número de slots sin utilizar
    //int _minGap; por ahora no lo uso
    int _niv; ///< nivel = vuelo al que se le está asignando slots

    int _maxPortesReg;  ///< guarda el número máximo de puertas regionales posibles
    int _maxPortesInter;  ///< guarda el número máximo de puertas internacionales posibles
    int _maxSlots; ///< guarda el número máximo de slots por puerta disponibles
    string _ho; ///< guarda la hora de apertura del aeropuerto
    string _ht; ///< guarda la hora de cierre del aeropuerto

    int _nPortesReg; ///< guarda el número actual de puertas regionales en uso
    int _nPortesInter; ///< guarda el número actual de puertas internacionales en uso
};


#endif //P2_EDA_2025_SOLUCIO_H