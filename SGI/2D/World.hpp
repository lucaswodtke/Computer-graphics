#ifndef WORLD_HPP
#define WORLD_HPP

#include "DisplayFile.hpp"
#include "Dialogs.hpp"

// Classe que define o mundo dos objetos
class World
{
    public:
        World() {}
        virtual ~World() {}
        Objeto* addPonto(std::string name, const Coordinate& p);
        Objeto* addReta(std::string name, const Coordinates& c);
        Objeto* addPoligono(std::string name, bool filled, const Coordinates& c);
        Objeto* addCurva2(std::string name, const Coordinates& c);
	    Objeto* addCurvaBSpline(std::string name, const Coordinates& c);
        Objeto* translacaoObj(std::string objName, double dx, double dy);
        Objeto* escalonarObj(std::string objName, double sx, double sy);
        Objeto* rotacaoObj(std::string objName, double angle, const Coordinate& p, rotacao type);
        Objeto* getObj(int pos){ return _objs.getObj(pos); }
        Objeto* getObj(std::string name);
        void addObj(Objeto *obj){ _objs.addObj(obj); }
        void removeObj(std::string name);
        int numObjs(){ return _objs.size(); }
	    Elemento<Objeto*>* getFirstObject(){ return _objs.getFirstElement(); }
    protected:
    private:
        DisplayFile _objs;
};

Objeto* World::addPonto(std::string name, const Coordinate& p)
{
    Ponto *obj = new Ponto(name, p);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addReta(std::string name, const Coordinates& c)
{
    Reta *obj = new Reta(name, c);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addPoligono(std::string name, bool filled, const Coordinates& c)
{
    Poligono *obj = new Poligono(name, filled, c);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addCurva2(std::string name, const Coordinates& c)
{
    if(c.size() < 4 || (c.size()-4)%3 != 0)
        throw "Uma curva de Bezier deve ter 4, 7, 10, 13... coordenadas.";
    Curva2 *obj = new Curva2(name, c);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addCurvaBSpline(std::string name, const Coordinates& c)
{
    if(c.size() < 4)
        throw "Uma curva B-Spline deve ter no minimo 4 coordenadas.";
    CurvaBSpline *obj = new CurvaBSpline(name, c);
    _objs.addObj(obj);
    return obj;
}

void World::removeObj(std::string name)
{
    Objeto tmp(name);
    _objs.removeObj(&tmp);
}

Objeto* World::getObj(std::string name)
{
    Objeto tmp(name);
    return _objs.getObj(&tmp);
}

Objeto* World::translacaoObj(std::string objName, double dx, double dy)
{
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);
    obj->transform(Transform::newTranslation(dx,dy));
    return obj;
}

Objeto* World::escalonarObj(std::string objName, double sx, double sy)
{
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);
    obj->transform(Transform::newScalingAroundObjCenter(sx,sy,obj->center()));
    return obj;
}

Objeto* World::rotacaoObj(std::string objName, double angle, const Coordinate& p, rotacao type){
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);

    switch(type)
    {
        case rotacao::OBJETO:
            obj->transform(Transform::newRotationAroundPoint(angle, obj->center()));
            break;
        case rotacao::MUNDO:
            obj->transform(Transform::newRotationAroundPoint(angle, Coordinate(0,0)));
            break;
        case rotacao::PONTO:
            obj->transform(Transform::newRotationAroundPoint(angle, p));
            break;
    }
    return obj;
}

#endif // WORLD_HPP
