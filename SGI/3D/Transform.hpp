#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <cmath>
#include <array>
#include "Objetos.hpp"

#define PI 3.1415926535897932384626433832795
#define TAMANHO 4

// Matriz de transformada
template<class type, int n, int m>
using Matrix = std::array<std::array<type, m>, n>;
typedef Matrix<double, TAMANHO, TAMANHO> Matriz4x4;

class Coordinate;

class Transform
{
    public:
        Transform(const Matriz4x4 m):
        _matrix(std::move(m)) {}
        Transform();
        virtual ~Transform() {}
        const Matriz4x4& getM() const {return _matrix;}
        Matriz4x4 getM() {return _matrix;}
	Transform& operator*=(const Transform& t2);
	Transform transpor();
        static Transform newTranslation(double dx, double dy, double dz);
	static Transform newRx(double anguloX, bool isRad=false);
        static Transform newRy(double anguloY, bool isRad=false);
        static Transform newRz(double anguloZ, bool isRad=false);
        static Transform newRa(double anguloA, const Coordinate& p);
        static Transform newRotation(double anguloX, double anguloY, double anguloZ);
	static Transform newFullRotation(double anguloX, double anguloY, double anguloZ, double anguloA, const Coordinate& p);
        static Transform newScaling(double sx, double sy, double sz);
	static Transform newScalingAroundObjCenter(double sx, double sy, double sz, const Coordinate& center);
	static double toRadians(double degrees){ return (PI/180) * degrees; }
    protected:
    private:
       Matriz4x4 _matrix;
};

Transform operator*(Transform t1, const Transform& t2);
std::ostream& operator<<(std::ostream& os, const Transform& t);

#endif // TRANSFORM_HPP
