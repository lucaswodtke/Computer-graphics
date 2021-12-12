#ifndef DISPLAYFILE_HPP
#define DISPLAYFILE_HPP

#include "ListaEnc.hpp"
#include "Objetos.hpp"

// Classe para adição de objetos

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
    protected:
    private:
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
        throw "Já existe um objeto com este nome.";
}

Objeto* DisplayFile::getObj(Objeto *obj)
{
    return *posicaoMem(obj);
}

#endif // DISPLAYFILE_HPP
