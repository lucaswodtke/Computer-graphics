#ifndef OBJETO_HPP
#define OBJETO_HPP

#include <string>
#include <iostream>
#include "ListaEnc.hpp"
#include "Ponto.hpp"

using namespace std;

// Classe que representa um objeto: um conjunto de pontos interligados no espaço vetorial

class Objeto
{
    public:
        string nome;
        string tipo;
        ListaEnc<Ponto> coordenadas;
};

#endif
