#ifndef LISTAENC_HPP
#define LISTAENC_HPP

#include "Elemento.hpp"

template<typename T>
class ListaEnc 
{
    public:
        ListaEnc();
        ~ListaEnc();
        void adicionaNoInicio(const T& dado);
        T retiraDoInicio();
        void eliminaDoInicio();
        void adicionaNaPosicao(const T& dado, int pos);
        int posicao(const T& dado) const;
        T* posicaoMem(const T& dado) const;
        bool contem(const T& dado);
        T retiraDaPosicao(int pos);
        void adiciona(const T& dado);
        T retira();
        T retiraEspecifico(const T& dado);
        void adicionaEmOrdem(const T& data);
        bool listaVazia() const;
        bool igual(T dado1, T dado2);
        bool igual(T dado1, T dado2) const;
        bool maior(T dado1, T dado2);
        bool menor(T dado1, T dado2);
        void destroiLista();
    protected:
        Elemento<T>* getHead(){ return head; }
        T getFromPos(int pos);
        int getSize(){ return this->size; }
    private:
        Elemento<T>* head;
        int size;
};

template <typename T>
T ListaEnc<T>::getFromPos(int pos)
{
    if(pos == 0)
        return this->head->getInfo();
    else if (pos < 0 || pos > this->size)
        throw "Posição inválida.";

    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < pos; i++)
    {
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição.";
        }
    }
    return temporario->getInfo();
}

template <typename T>
ListaEnc<T>::ListaEnc() 
{
    this->head = NULL;
    this->size = 0;
}

template <typename T>
ListaEnc<T>::~ListaEnc() 
{
    this->destroiLista();
}

template <typename T>
bool ListaEnc<T>::listaVazia() const 
{
    return this->size == 0;
}

template <typename T>
void ListaEnc<T>::adicionaNoInicio(const T& valor) 
{
    Elemento<T> *ult = this->head;
    this->head = new Elemento<T>(valor, ult);
    this->size++;
}


template <typename T>
void ListaEnc<T>::adiciona(const T& valor)
{
    if (this->listaVazia()) 
    {
        return this->adicionaNoInicio(valor);
    }
    int i, max;
    Elemento<T> *temporario = this->head;
    max = this->size - 1;
    for (i = 0; i < max; i++) 
    {
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição ";
        }
    }
    Elemento<T> *novo = new Elemento<T>(valor, NULL);
    temporario->setProximo(novo);
    this->size++;
}

template <typename T>
void ListaEnc<T>::adicionaNaPosicao(const T& valor, int posicao) 
{
    if (posicao == 0) 
    {
        return this->adicionaNoInicio(valor);
    } 
    else if (posicao < 0 || posicao > this->size) 
    {
        throw "Posicao inválida.";
    }
    Elemento<T> *temporario = this->head;
    Elemento<T> *novo;
    posicao = posicao - 1;
    int h;
    for (h = 0; h < posicao; h++) 
    {
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Posicao inválida ou lista cheia.";
        }
    }
    novo = temporario->getProximo();
    temporario->setProximo(new Elemento<T>(valor, novo));
    this->size++;
}

template <typename T>
void ListaEnc<T>::adicionaEmOrdem(const T& data) 
{
    if (this->listaVazia()) 
    {
        return this->adicionaNoInicio(data);
    }
    Elemento<T> *temporario = this->head;
    int max, posicao;
    max = this->size - 1;
    posicao = 0;
    while (posicao < max && this->maior(data, temporario->getInfo())) 
    {
        temporario = temporario->getProximo();
        posicao++;
    }
    if (this->maior(data, temporario->getInfo())) 
    {
        this->adicionaNaPosicao(data, posicao+1);
    } 
    else 
    {
        this->adicionaNaPosicao(data, posicao);
    }
}

template <typename T>
void ListaEnc<T>::destroiLista() 
{
    Elemento<T> *atual = this->head;
    int i, max;
    max = this->size;
    for (i = 0; i < max; i++) 
    {
        Elemento<T> *anterior = atual;
        atual = atual->getProximo();
        delete anterior;
    }
    this->head = NULL;
    this->size = 0;
}

template <typename T>
T ListaEnc<T>::retiraDaPosicao(int pos) 
{
    if (pos == 0) 
    {
        return this->retiraDoInicio();
    } 
    else if (pos < 0 || pos > this->size) 
    {
        throw "Posicao inválida.";
    }
    int i;
    Elemento<T> *temporario = this->head;
    pos -= 1;
    for (i = 0; i < pos; i++) 
    {
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição.";
        }
    }
    Elemento<T> *proximo = temporario->getProximo();
    T valor = proximo->getInfo();
    temporario->setProximo(proximo->getProximo());
    delete proximo;
    this->size--;
    return valor;
}

template <typename T>
T ListaEnc<T>::retiraEspecifico(const T& dado) 
{
    int posicao = this->posicao(dado);
    return this->retiraDaPosicao(posicao);
}

template <typename T>
int ListaEnc<T>::posicao(const T& dado) const 
{
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) 
    {
        if (igual(temporario->getInfo(), dado)) 
        {
            return i;
        }
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição.";
        }
    }
    throw "Elemento não encontrado na lista.";
}

template <typename T>
T* ListaEnc<T>::posicaoMem(const T& dado) const 
{
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) {
        if (igual(temporario->getInfo(), dado)) 
        {
            return temporario->getInfoPointer();
        }
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição.";
        }
    }
    throw "Elemento não encontrado na lista.";
}

template <typename T>
T ListaEnc<T>::retiraDoInicio() 
{
    if (this->listaVazia())
    {
        throw "A lista está vazia.";
    }
    Elemento<T> *inicio = this->head;
    T valor = inicio->getInfo();
    this->head = inicio->getProximo();
    this->size--;
    delete inicio;
    return valor;
}

template <typename T>
T ListaEnc<T>::retira() 
{
    if (this->listaVazia()) 
    {
        throw "A lista está vazia.";
    }
    int i, max;
    Elemento<T> *temporario = this->head;
    max = this->size - 2;
    if (max < 0) 
    {
        return this->retiraDoInicio();
    }
    for (i = 0; i < max; i++) 
    {
        temporario = temporario->getProximo();
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posicao.";
        }
    }
    Elemento<T> *proximo = temporario->getProximo();
    T valor = proximo->getInfo();
    temporario->setProximo(proximo->getProximo());
    delete proximo;
    this->size--;
    return valor;
}

template <typename T>
void ListaEnc<T>::eliminaDoInicio() 
{
    if (this->listaVazia()) 
    {
        throw "A lista está vazia.";
    }
    Elemento<T> *inicio = this->head;
    Elemento<T> *atual = inicio->getProximo();
    this->head = atual;
    this->size--;
    delete inicio;
}

template <typename T>
bool ListaEnc<T>::contem(const T& dado) 
{
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) 
    {
        if (temporario == NULL) 
        {
            throw "Temporário foi detectado como nulo na posição. ";
        }
        if (igual(temporario->getInfo(), dado)) 
        {
            return true;
        }
        temporario = temporario->getProximo();
    }
    return false;
}

template <typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) 
{
    if(dado1 != NULL && dado2 == NULL) return false;
    if(dado2 != NULL && dado1 == NULL) return false;
    return *dado1 == *dado2;
}

template <typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) const 
{
    if(dado1 != NULL && dado2 == NULL) return false;
    if(dado2 != NULL && dado1 == NULL) return false;
    return *dado1 == *dado2;
}

template <typename T>
bool ListaEnc<T>::maior(T dado1, T dado2) 
{
    if (dado1 > dado2) 
    {
        return true;
    }
    return false;
}

template <typename T>
bool ListaEnc<T>::menor(T dado1, T dado2) 
{
    if (dado1 < dado2)
    {
        return true;
    }
    return false;
}

#endif
