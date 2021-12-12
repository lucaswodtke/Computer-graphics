#ifndef WORLD_HPP
#define WORLD_HPP

#include "DisplayFile.hpp"
#include "Dialogs.hpp"

// Adição de objetos no mundo e operações neles

class World
{
    public:
        World() {}
        virtual ~World() {}
        Objeto* addPonto(std::string name, const Coordinate& p);
        Objeto* addReta(std::string name, const Coordinates& c);
        Objeto* addPoligono(std::string name, bool filled, const Coordinates& c);
        Objeto* addCurvaBezier(std::string name, const Coordinates& c);
	    Objeto* addCurvaBSpline(std::string name, const Coordinates& c);
	    Objeto* addObj3D(std::string name, const ListaFaces& faces);
	    Objeto* addSuperficie(std::string name, ObjType tipo, int maxLin, int maxCols, const Coordinates& c);
        Objeto* getObj(int pos){ return _objs.getObj(pos); }
        Objeto* getObj(std::string name);
        Objeto* translacaoObj(std::string objName, double dx, double dy, double dz);
        Objeto* escalonarObj(std::string objName, double sx, double sy, double sz);
        Objeto* rotacaoObj(std::string objName, double anguloX, double anguloY, double anguloZ, double angulo, const Coordinate& p, rotacao type);
        Elemento<Objeto*>* getFirstObject(){ return _objs.getFirstElement(); }
        int numObjs(){ return _objs.size(); }
        void addObj(Objeto *obj){ _objs.addObj(obj); }
        void removeObj(std::string name);
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

Objeto* World::addObj3D(std::string name, const ListaFaces& faces)
{
    
    Objeto3D *obj = new Objeto3D(name, faces);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addCurvaBezier(std::string name, const Coordinates& c)
{
    if(c.size() < 4 || (c.size()-4)%3 != 0)
        throw "A Bézier deve ter a forma de { 4, 7, 10, ... } pontos!";
    CurvaBezier *obj = new CurvaBezier(name, c);
    _objs.addObj(obj);
    return obj;
}

Objeto* World::addCurvaBSpline(std::string name, const Coordinates& c)
{
    if(c.size() < 4)
        throw "A B-Spline deve ter ao menos 4 pontos!";
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

Objeto* World::translacaoObj(std::string objName, double dx, double dy, double dz)
{
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);
    obj->transform(Transform::newTranslation(dx,dy,dz));
    return obj;
}

Objeto* World::escalonarObj(std::string objName, double sx, double sy, double sz)
{
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);
    obj->transform(Transform::newScalingAroundObjCenter(sx,sy,sz,obj->center()));
    return obj;
}

Objeto* World::rotacaoObj(std::string objName, double anguloX, double anguloY, double anguloZ, double angulo, const Coordinate& p, rotacao type)
{
    Objeto tmp(objName);
    Objeto *obj = _objs.getObj(&tmp);
    if(angulo == 0)
    {
        obj->transform(Transform::newRotation(anguloX,anguloY,anguloZ));
        return obj;
    }
    switch(type)
    {
        case rotacao::OBJETO:
            obj->transform(Transform::newFullRotation(anguloX,anguloY,anguloZ,angulo,obj->center()));
            break;
        case rotacao::PONTO:
            obj->transform(Transform::newFullRotation(anguloX,anguloY,anguloZ,angulo,p));
            break;
    }
    return obj;
}

Objeto* World::addSuperficie(std::string name, ObjType tipo, int maxLin, int maxCols, const Coordinates& c)
{
    if(c.size() < 16)
        throw "Uma superfície tem que ter ao menos 16 pontos!";
    Superficie *obj = nullptr;
    if(tipo == ObjType::SUPERFICIE_BEZIER)
        obj = new SuperficieBezier(name, maxLin, maxCols, c);
    else if(tipo == ObjType::SUPERFICIE_BSPLINE)
        obj = new SuperficieBSpline(name, maxLin, maxCols, c);
    _objs.addObj(obj);
    return obj;
}

#endif // WORLD_HPP
