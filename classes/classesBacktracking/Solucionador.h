//
// Created by claud on 03/12/2025.
//

#ifndef P2_EDA_2025_SOLUCIONADOR_H
#define P2_EDA_2025_SOLUCIONADOR_H
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include "Solucio.h"
#include "Candidats.h"

using namespace std;

// contiene métodos para solución voraz, una solución, y solución óptima

// clase base
class Solucionador {
    public:
    virtual ~Solucionador() = default; // creo
    virtual void solucionar(const Solucio& inicial);
};

class Voraz: public Solucionador {
    public:
    void solucionar(const Solucio& sol) override; // hace override
};

class BacktrackingUnaSol : public Solucionador {
    public:
    void solucionar(const Solucio& inicial) override;
    private:
    void backtrackingUna(const Solucio& inicial);
};

class BacktrackingMillorSol : public Solucionador {
    public:
    void solucionar(const Solucio& inicial) override;
    private:
    void backtrackingMillor(const Solucio& inicial);
};

#endif //P2_EDA_2025_SOLUCIONADOR_H