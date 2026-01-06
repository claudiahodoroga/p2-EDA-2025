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
    virtual bool solucionar()=0; ///< Resolver y devolver true si hay solución
    virtual void mostrarResultat() const=0; ///< Mostrar el resultado
};

// ALGORITMO VORAZ
class SolucionadorVoraz : public Solucionador {
    public:
    SolucionadorVoraz(SolucioVoraz& sol): _sol(sol){}
    bool solucionar() override;
    void mostrarResultat() const override;

private:
    SolucioVoraz& _sol;
};

// BACKTRACKING UNA SOLUCIÓN
class SolucionadorUna : public Solucionador {
    public:
    SolucionadorUna(SolucioUna& sol): _sol(sol), _trobat(false){}
    bool solucionar() override;
    void mostrarResultat() const override;
private:
    void backtracking(); ///< Recursividad
    SolucioUna& _sol;
    bool _trobat; ///< Para saber si se ha encontrado una solución, y parar
};

// BACKTRACKING MEJOR SOLUCIÓN
class SolucionadorMillor : public Solucionador {
public:
    SolucionadorMillor(SolucioMillor& sol) : _optima(sol), _trobatAlguna(false){}
private:
    void backtracking(); ///< Recursividad
    SolucioMillor& _optima;
    bool _trobatAlguna; ///< Para saber si al menos hay una solución
};

#endif //P2_EDA_2025_SOLUCIONADOR_H