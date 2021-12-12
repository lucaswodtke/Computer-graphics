#ifndef DISPLAYFILE_HPP
#define DISPLAYFILE_HPP

#include "ListaEnc.hpp"
#include "Objetos.hpp"

class DisplayFile : public ListaEnc<Objeto*>
{
    public:
        DisplayFile(){}
        virtual ~DisplayFile() {}
        Objeto* getObj(int pos){ return getFromPos(pos); }
        Objeto* getObj(Objeto *obj);
	    Elemento<Objeto*>* getFirstElement(){ return getHead(); }
        void addObj(Objeto *obj);
        void removeObj(Objeto *obj){ delete retiraEspecifico(obj); }
        int size(){ return getSize(); }
};

void DisplayFile::addObj(Objeto* obj)
{
    if(obj->getName() == "")
        throw "Digite o nome do objeto.";
    if(!contem(obj))
    {
        adiciona(obj);
    }
    else
        throw "Já existe um objeto com esse nome!";
}

Objeto* DisplayFile::getObj(Objeto *obj)
{
    return *posicaoMem(obj);
}

#endif // DISPLAYFILE_HPP