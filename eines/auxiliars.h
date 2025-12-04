//
// Created by claud on 04/12/2025.
//

#ifndef P2_EDA_2025_AUXILIARS_H
#define P2_EDA_2025_AUXILIARS_H


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <set>

#include "../classes/classesAuxiliars/Vol.h"

using namespace std;

// Estructura de excepciones
struct excepcio {
    struct entradaIncorrecta {
        string missatge;

        entradaIncorrecta(const string &m);
    };

    struct errorLectura {
        string missatge;

        errorLectura(const string &m);
    };
};

// Estructura para almacenar los parámetros de entrada
struct Parametres {
    string path;
    char tipoCalculo;
    string Ho;
    string Ht;
    int Pr;
    int Pi;
    bool ajuda;

    Parametres();
};

// Declaraciones de funciones auxiliares
void mostrarAjuda(const char *nomPrograma);

Parametres processaParametres(int argn, char **argv);

int llegirDades(const string &path, set<Vol> &vols);

void mostrarResultat(const Solucio &sol, const duration<double> &duracio);

#endif //P2_EDA_2025_AUXILIARS_H
