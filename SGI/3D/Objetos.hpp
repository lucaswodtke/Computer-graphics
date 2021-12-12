#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>
#include "Transform.hpp"

class Transform;
class Coordinate
{
    public:
        Coordinate(){}
        Coordinate(double cx, double cy, double cz = 0.0) : x(cx), y(cy), z(cz), w(1.0){}
        virtual ~Coordinate(){};
        Coordinate& operator+=(double step);
        Coordinate& operator-=(double step);
        Coordinate& operator+=(const Coordinate& c);
        Coordinate& operator-=(const Coordinate& c);
        Coordinate& operator*=(const Transform& t);
	    Coordinate operator-() const;
        bool operator==(const Coordinate& c)
        {
            return (this->x==c.x && this->y==c.y && this->z==c.z);
        }
        double x = 0.0, y = 0.0 , z = 0.0, w = 1.0 ;
};

std::ostream& operator<<(std::ostream& os, const Coordinate& c);
Coordinate operator-(const Coordinate& c1, const Coordinate& c2);

typedef std::vector<Coordinate> Coordinates;
enum class ObjType { OBJETO, PONTO, RETA, POLIGONO, CURVABEZIER, CURVABSPLINE, OBJETO3D, SUPERFICIE_BEZIER, SUPERFICIE_BSPLINE };

class Objeto
{
    public:
        Objeto(const std::string name) : _name(name){}
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
        virtual Coordinate center() const;
        virtual Coordinate centerN() const;
        virtual void transform(const Transform& t);
        virtual void transformN(const Transform& t); //bool clear=true);
	    //virtual void alterarProjecao(double d)
        bool operator==(const Objeto& other) { return this->getName() == other.getName(); }
        Objeto& operator*(){ return *this; }
	    virtual void addCoordinate(double x, double y, double z) {_coords.emplace_back(x,y,z);}
	    void addCoordinate(const Coordinate& c) {_coords.push_back(c);}
    protected:
        std::string _name;
        Coordinates _coords;
        Coordinates _coordsN;
        void addCoordinate(const Coordinates& coords) { _coords.insert(_coords.end(), coords.begin(), coords.end()); }
};

class Ponto : public Objeto
{
    public:
        Ponto(std::string name) : Objeto(name) {}
		Ponto(std::string name, double x, double y, double z) : Objeto(name) { addCoordinate(x,y,z); }
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
        Curva2(std::string name) : Objeto(name) {}
	    Curva2(std::string& name, const Coordinates& coords) : Objeto(name) {}
        virtual void gerarCurva(const Coordinates& cpCoords){};
        Coordinates& getPontosControle(){ return _points; }
    protected:
        void setPontos(const Coordinates& coords) { _points.insert(_points.end(), coords.begin(), coords.end()); }
    protected:
            Coordinates _points;
            float _step = 0.02;
};

class CurvaBezier : public Curva2
{
    public:
        CurvaBezier(std::string& name) : Curva2(name) {}
	    CurvaBezier(std::string& name, const Coordinates& coords) : Curva2(name) { gerarCurva(coords); }
	virtual ObjType getType() const { return ObjType::CURVABEZIER; }
	virtual std::string getTypeName() const { return "Curva Bezier"; }
	void gerarCurva(const Coordinates& cpCoords);
};

class CurvaBSpline : public Curva2
{
    public:
        CurvaBSpline(std::string name) :
        Curva2(name) {}
	    CurvaBSpline(std::string& name, const Coordinates& coords) :
        Curva2(name) { gerarCurva(coords); }
        virtual ObjType getType() const { return ObjType::CURVABSPLINE; }
	    virtual std::string getTypeName() const { return "Curva B-Spline"; }
	    void gerarCurva(const Coordinates& cpCoords);	
};

typedef std::vector<Poligono> ListaFaces;

class Objeto3D : public Objeto
{
    public:
        Objeto3D(std::string name) : Objeto(name) {}
        Objeto3D(std::string& name, const ListaFaces& faces) : Objeto(name) { _listaFaces = std::move(faces); }
        virtual ObjType getType() const { return ObjType::OBJETO3D; }
		virtual std::string getTypeName() const { return "Objeto 3D"; }
        void transform(const Transform& t);
        void transformN(const Transform& t);
	    void alterarProjecao(double d);
        Coordinate center() const;
        Coordinate centerN() const;
        ListaFaces& getListaFaces() { return _listaFaces; }
        void insertFaces(const ListaFaces& faces) { _listaFaces.insert(_listaFaces.end(), faces.begin(), faces.end()); }
    protected:
        ListaFaces _listaFaces;
};

typedef std::vector<Curva2> ListaCurvas;

class Superficie : public Objeto
{
    public:
        Superficie(std::string& name) : Objeto(name) {}
        Superficie(std::string& name, int maxLin, int maxCols) : Objeto(name) { m_maxLines = maxLin; m_maxCols = maxCols; }
	    Superficie(std::string& name, const Coordinates& coords) : Objeto(name) { }
        virtual void gerarSuperficie(const Coordinates& cpCoords){};
        Coordinates& getPontosControle(){ return _pontosControle; }
        void transform(const Transform& t);
        void transformN(const Transform& t);
        Coordinate center() const;
	    Coordinate centerN() const;
        int getMaxLin(){ return m_maxLines; }
        int getMaxCols(){ return m_maxCols; }
        ListaCurvas& getListaCurvas() { return _listaCurvas; }
    protected:
        void setPontosControle(const Coordinates& coords) { _pontosControle.insert(_pontosControle.end(), coords.begin(), coords.end()); }
    protected:
            Coordinates _pontosControle;
            float _step = 0.02; 
            int m_maxLines = 4, m_maxCols = 4;
            ListaCurvas _listaCurvas;
};

class SuperficieBezier : public Superficie
{
    public:
        SuperficieBezier(std::string& name) : Superficie(name) {}
	    SuperficieBezier(std::string& name, int maxLin, int maxCols, const Coordinates& coords) : Superficie(name,maxLin,maxCols) { gerarSuperficie(coords); }
	    SuperficieBezier(std::string& name, const Coordinates& coords) : Superficie(name) { gerarSuperficie(coords); }
        virtual ObjType getType() const { return ObjType::SUPERFICIE_BEZIER; }
	    virtual std::string getTypeName() const { return "Superficie Bezier"; }
	    void gerarSuperficie(const Coordinates& cpCoords);
        Coordinate blendingFunction(float s, double s2, double s3, float t, double t2, double t3, int nLine, int nCol, const Coordinates& coords);
};

class SuperficieBSpline : public Superficie
{
    public:
        SuperficieBSpline(std::string& name) : Superficie(name) {}
        SuperficieBSpline(std::string& name, int maxLin, int maxCols, const Coordinates& coords) :
        Superficie(name,maxLin,maxCols) { gerarSuperficie(coords); }
	    SuperficieBSpline(std::string& name, const Coordinates& coords) : Superficie(name) { gerarSuperficie(coords); }
        virtual ObjType getType() const { return ObjType::SUPERFICIE_BSPLINE; }
	    virtual std::string getTypeName() const { return "Superficie B-Spline"; }
	    void gerarSuperficie(const Coordinates& cpCoords);
    private:
	    typedef std::array<std::array<double,4>,4>Matrix4x4;

	double _ns=30, _nt=30;
	//Matrizes usadas para a criação da superficie.
	Matrix4x4 _coordsX, _coordsY, _coordsZ;
	Matrix4x4 _cx, _cy, _cz;
	Matrix4x4 _es, _et;
	Matrix4x4 _DDx, _DDy, _DDz;
	static Transform _M;
    static Transform _Mt;

    private:
        void atualizarCoordsMatrizes(int nLine, int nCol);
        void atualizarForwardDiffMatrizes();
        void calcCoeficientes();
        void criarDeltaMatrizes(double delta_s, double delta_t);
        void criarForwardDiffMatrizes();
        void gerarCurva( double n, double x, double Dx, double D2x, double D3x, double y, double Dy, double D2y, double D3y, double z, double Dz, double D2z, double D3z);
};

#endif // OBJETOS_HPP
