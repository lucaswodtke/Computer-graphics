#ifndef TESTES_HPP
#define TESTES_HPP

#include <string>
#include <iostream>
#include "ListaEnc.hpp"
#include "Objeto.hpp"
#include "Ponto.hpp"

using namespace std;

class Testes
{
    public:
        Testes() {}
        ~Testes() {}
        void executar()
        {

            // ***** Testes ***** \\


            // Teste ListaEnc
            ListaEnc<int> lista;
            lista.adicionaNoInicio(1);
            lista.adicionaNoInicio(2);
            lista.adicionaNoInicio(3);
            cout << "Teste Lista: " << endl;
            cout << "Lista posição 1: " << lista.retira() << endl;
            cout << "Lista posição 2: " << lista.retira() << endl;
            cout << "Lista posição 3: " << lista.retira() << endl;

            // Teste Ponto
            Ponto p1;
            p1.x = 10;
            p1.y = 25;
            Ponto p2;
            p2.x = -10;
            p2.y = 5;
            cout << "Teste Ponto: " << endl;
            cout << "Coordenadas do ponto 1: \nX = " << p1.x << "\nY = " << p1.y << endl;
            cout << "Coordenadas do ponto 2: \nX = " << p2.x << "\nY = " << p2.y << endl;

            // Teste Objeto
            Objeto obj1;
            obj1.nome = "Objeto 1";
            obj1.tipo = "Triangulo";
            cout << "Teste Objeto" << endl;
            cout << "Nome do objeto: " << obj1.nome << ", Tipo do objeto: " << obj1.tipo << endl;

        }

};

#endif
