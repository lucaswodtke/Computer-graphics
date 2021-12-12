#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>
#include "Transform.hpp"

class Transform;

// Classe que define um objeto

class Coordinate
{
    public:
        Coordinate() :
            x(0), y(0), z(1){}
        Coordinate(double cx, double cy) :
            x(cx), y(cy), z(1){}
        virtual ~Coordinate(){};

        Coordinate& operator+=(double step);
        Coordinate& operator-=(double step);
        Coordinate& operator+=(const Coordinate& c);
        Coordinate& operator-=(const Coordinate& c);
        Coordinate& operator*=(const Transform& t);
        bool operator==(const Coordinate& c){
            return (this->x==c.x && this->y==c.y);
        }

        double x,y,z;
    protected:
    private:
};

Coordinate operator-(const Coordinate& c1, const Coordinate& c2);

typedef std::vector<Coordinate> Coordinates;
enum class ObjType { OBJETO, PONTO, RETA, POLIGONO, CURVA2, CURVABSPLINE };

class Objeto
{
    public:
        Objeto(const std::string name) :
            _name(name){}
        virtual ~Objeto() {}

        std::string getName(){ return _name; }
        const std::string getName() const { return _name; }

        virtual ObjType getType() const { return ObjType::OBJETO; }
		virtual std::string getTypeName() const { return "Objeto"; }

        Coordinates& getCoords() {return _coords;}
        Coordinate& getCoord(int index) { return _coords[index]; }
        int getCoordsSize(){ return _coords.size(); }

        Coordinates& getCoordsN() {return _coordsN;}
		Coordinate& getCoordN(int index) { return _coordsN[index]; }
		int getCoordsNSize(){ return _coordsN.size(); }
		void setNCoord(const Coordinates& c);

        Coordinate center() const;
        Coordinate centerN() const;
        void transform(const Transform& t);
        void transformN(const Transform& t);

        bool operator==(const Objeto& other)
        {
            return this->getName() == other.getName();
        }
        Objeto& operator*(){ return *this; }

	void addCoordinate(double x, double y) {_coords.emplace_back(x,y);}
	void addCoordinate(const Coordinate& c) {_coords.push_back(c);}
    protected:
        std::string _name;
        Coordinates _coords;
        Coordinates _coordsN; // N = normalizado
        void addCoordinate(const Coordinates& coords)
        {
            _coords.insert(_coords.end(), coords.begin(), coords.end());
        }
};

class Ponto : public Objeto
{
    public:
        Ponto(std::string name) : Objeto(name) {}
		Ponto(std::string name, double x, double y) : Objeto(name) { addCoordinate(x,y); }
        Ponto(std::string name, const Coordinate& c) : Objeto(name) { addCoordinate(c); }
        virtual ObjType getType() const { return ObjType::PONTO; }
		virtual std::string getTypeName() const { return "Ponto"; }
};

class Reta : public Objeto
{
    public:
        Reta(std::string name) : Objeto(name) {}
		Reta(std::string name, const Coordinates& coords) : Objeto(name) { addCoordinate(coords); }
        virtual ObjType getType() const { return ObjType::RETA; }
		virtual std::string getTypeName() const { return "Reta"; }
};

class Poligono : public Objeto
{
    public:
        Poligono(std::string name) : Objeto(name) { _filled = false; }
		Poligono(std::string name, const Coordinates& coords) : Objeto(name) { _filled = false; addCoordinate(coords); }
        Poligono(std::string name, bool filled, const Coordinates& coords) : Objeto(name) { _filled = filled; addCoordinate(coords); }
        virtual ObjType getType() const { return ObjType::POLIGONO; }
		virtual std::string getTypeName() const { return "Poligono"; }
        bool filled() const { return _filled; }
        void setFilled(bool v){ _filled = v; }
    private:
        bool _filled;
};

class Curva2 : public Objeto
{
    public:
        Curva2(std::string& name) :
            Objeto(name) {}
		Curva2(std::string& name, const Coordinates& coords) :
            Objeto(name) { addCoordinate(coords); criarCurva(); }

        virtual ObjType getType() const { return ObjType::CURVA2; }
		virtual std::string getTypeName() const { return "Curva2"; }

		void criarCurva();
		Coordinates& getPontos(){ return _points; }

    private:
        void setPontos(const Coordinates& coords)
            { _points.insert(_points.end(), coords.begin(), coords.end()); }

    protected:
        Coordinates _points;
        // bf
        float _step = 0.01; 
};

class CurvaBSpline : public Objeto
{
    public:
        CurvaBSpline(std::string& name) :
            Objeto(name) {}
		CurvaBSpline(std::string& name, const Coordinates& coords) :
            Objeto(name) { addCoordinate(coords); criarCurvaBSpline(); }

        virtual ObjType getType() const { return ObjType::CURVABSPLINE; }
		virtual std::string getTypeName() const { return "Curva B-Spline"; }

	void criarCurvaBSpline();
		Coordinates& getPontos(){ return _points; }

    private:
        void setPontos(const Coordinates& coords)
            { _points.insert(_points.end(), coords.begin(), coords.end()); }

    protected:
        Coordinates _points;
        float _step = 0.01; 	
};



#endif
