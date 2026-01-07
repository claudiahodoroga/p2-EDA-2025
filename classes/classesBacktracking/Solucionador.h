//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_SOLUCIONADOR_H
#define P2_EDA_2025_SOLUCIONADOR_H
#include <iostream>
#include "Solucio.h"

using namespace std;

// contiene métodos para solución voraz, una solución, y solución óptima

// CLASE BASE
class Solucionador {
    public:
    virtual ~Solucionador() = default;
    virtual void solucionar(Solucio& sol)=0; ///< Resolver con la solución inicial
};

// ALGORITMO VORAZ
class SolucionadorVoraz {
    public:
    SolucionadorVoraz() {}
    void solucionar(SolucioVoraz& sol);
};

// BACKTRACKING UNA SOLUCIÓN
class SolucionadorUna : public Solucionador {
    public:
    SolucionadorUna(): _trobat(false){}
    void solucionar(Solucio& solBase) override; ///< Toma cualquier Solucio, pero trabaja con SolucioUna
private:
    void backtracking(SolucioUna& sol); ///< Recursividad
    bool _trobat; ///< Para saber si se ha encontrado una solución, y parar
};

// BACKTRACKING MEJOR SOLUCIÓN
class SolucionadorMillor : public Solucionador {
public:
    SolucionadorMillor() : _millorSlotsInactius(INT_MAX), _millorMinGap(-1), _teMillor(false) {}
    void solucionar(Solucio& solBase) override;
    bool teMillorSolucio() const;
private:
    void backtracking(SolucioMillor& actual); ///< Recursividad
    SolucioMillor* _optima; ///< Mejor solución encontrada actualmente
    int _millorSlotsInactius; ///< Métricas de la mejor solución
    int _millorMinGap;
    bool _teMillor; ///< Para saber si se ha encontrado al menos una solución
};

#endif //P2_EDA_2025_SOLUCIONADOR_H
