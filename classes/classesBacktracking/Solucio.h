//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_SOLUCIO_H
#define P2_EDA_2025_SOLUCIO_H

#include <iostream>
#include <iomanip>
#include <climits>
#include <vector>

#include "Candidats.h"
#include "../classesAuxiliars/Porta.h"
#include "../classesAuxiliars/Vol.h"

using namespace std;

// la solución es una serie de puertas con vuelos asignados a sus slots


// CLASE BASE
class Solucio {
public:
    Solucio()
    :_maxSlots(-1), _maxPortesInter(-1), _maxPortesReg(-1),
    _nPortesInter(0), _nPortesReg(0), _ho(0), _ht(0)  {}
    Solucio(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht,
        const vector<Vol>&vols);
    virtual ~Solucio() = default;

    int obtNumPortes() const;
    int obtNumPortesInter() const;
    int obtNumPortesReg() const;
    int obtMaxSlots() const;
    const vector<Porta>& obtPortes() const;
    const vector<Vol>& obtVols() const;
    int obtMaxPortesInter() const;
    int obtMaxPortesReg() const;
    int obtHo() const;
    int obtHt() const;

    // métricas
    int calcuarSlotsInactius() const;
    int calcularMinGap() const;

    int horaToMin(const string& h) const; ///< Conversión de HH:MM a minutos
    string minToHora(int minuts) const; ///< Conversión de minutos a HH:MM

    void mostrarSolucio() const;


protected:
    // Estructuras de datos principales
    vector<Porta> _portes; ///< Puertas de embarque con asignación de slots
    vector<Vol> _vols; ///< Vuelos que asignar

    // Restricciones
    int _maxPortesReg;  ///< Número máximo de puertas regionales posibles
    int _maxPortesInter;  ///< Número máximo de puertas internacionales posibles
    int _maxSlots; ///< Número máximo de slots por puerta disponibles (según horas de apertura)
    int _ho; ///< Hora de apertura del aeropuerto
    int _ht; ///< Hora de cierre del aeropuerto

    // Estado actual
    int _nPortesReg; ///< Número actual de puertas regionales en uso
    int _nPortesInter; ///< Número actual de puertas internacionales en uso

    // Métodos auxiliares
    bool potCrearPorta(char tipus) const; ///< Comprueba que se pueda crear una nueva puerta de tipo @param tipus
    void crearNovaPorta(char tipus); ///< Crea nueva puerta de tipo @param tipus

};

// SOLUCIÓN VORAZ
class SolucioVoraz : public Solucio {
    public:
    SolucioVoraz(int maxPortesReg, int maxPortesInt, const string& Ho,
                 const string& Ht, const vector<Vol>& vols);

    bool assignarVolsVoraz(); ///< Asigna vuelos con método voraz. Devuelve true si todos están asignados

    private:
    bool assignarVol(int idxVol); ///< Intenta asignar un vuelo al primer slot disponible. Devuelve true si se puede
    pair<int, int> trobarPrimeraOpcio(const Vol& vol) const; ///< Método auxiliar para encontrar el primer slot compatible con el vuelo @param vol
    void ordenarVols(); ///< Ordena los vuelos por la longitud de su servicio
};

// BASE PARA BACKTRACKING
class SolucioBacktracking : public Solucio {
    public:
    SolucioBacktracking(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht, const vector<Vol>& vols)
        : Solucio(maxPortesReg, maxPortesInt, Ho, Ht, vols), _niv(0) {}

    Candidats inicialitzarCandidats() const; ///< Inicializar candidatos para el vuelo actual
    bool acceptable(int idxPorta, int slotInicio) const; ///< Determinar si el candidato es aceptable
    void anotar(int idxPorta, int slotInicio); ///< Asigna el vuelo actual al slot
    void desanotar(int idxPorta, int slotInicio); ///< Desasigna el vuelo del slot
    bool completa() const; ///< Devuelve si todos los vuelos están asignados

    int obtNivell() const;
    protected:
    int _niv; ///< Nivel actual de la recursión. Corresponde al índice del vuelo que se está asignando.
    bool slotsDisponibles(int idxPorta, int slotInicio, int numSlots) const; ///< Método auxiliar para función aceptable
    bool compatibleAmbFinestra(int slotInicio, const Vol& vol) const; ///< Método auxiliar para comprobar si el slot es compatible con la ventana de servicio del vuelo
};

// SOLUCIÓN PARA BACKTRACKING UNA SOLUCION. No necesita nada adicional a la clase base.
class SolucioUna : public SolucioBacktracking {
    public:
    SolucioUna(int maxPortesReg, int maxPortesInt, const string& Ho, const string& Ht, const vector<Vol>& vols)
    : SolucioBacktracking(maxPortesReg, maxPortesInt, Ho, Ht, vols) {}
};

// SOLUCIÓN PARA BACKTRACKING MEJOR SOLUCIÓN
class SolucioMillor : public SolucioBacktracking {
    public:
    SolucioMillor(int maxPortesReg, int maxPortesInt, const string& Ho,
                  const string& Ht, const vector<Vol>& vols)
        : SolucioBacktracking(maxPortesReg, maxPortesInt, Ho, Ht, vols) {}
    // ctor de copia
    SolucioMillor(const SolucioMillor& sol)
        : SolucioBacktracking(sol._maxPortesReg, sol._maxPortesInter,
                              sol.minToHora(sol._ho), sol.minToHora(sol._ht), sol._vols) {
        _portes = sol._portes;
        _nPortesInter = sol._nPortesInter;
        _nPortesReg = sol._nPortesReg;
        _niv = sol._niv;
    }
    // operador de asignación
    SolucioMillor& operator=(const SolucioMillor& sol) {
        if (this != &sol) {
            // copiar miembros de la clase base
            _vols = sol._vols;
            _maxPortesReg = sol._maxPortesReg;
            _maxPortesInter = sol._maxPortesInter;
            _maxSlots = sol._maxSlots;
            _ho = sol._ho;
            _ht = sol._ht;

            // copiar miembros de la clase derivada
            _portes = sol._portes;
            _nPortesInter = sol._nPortesInter;
            _nPortesReg = sol._nPortesReg;
            _niv = sol._niv;
        }
        return *this;
    }
    bool esMillor(int optimaSlotsInactius, int optimaMinGap) const; ///< Comprobar si la solución actual es mejor que la optima
    bool potSerMillor(int idxPortaCand, int idxSlotCand, int optimaSlotsInactius) const; ///< Comprobar si la solución actual puede llegar a ser mejor que la optima
};

#endif //P2_EDA_2025_SOLUCIO_H