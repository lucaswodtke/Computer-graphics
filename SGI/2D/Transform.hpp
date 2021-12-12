#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <cmath>
#include <array>
#include "Objetos.hpp"

#define PI 3.1415926535897932384626433832795

// Classe Transform
// Usada para reescalar, rotacionar, translação

#define M_SIZE 3
typedef std::array<std::array<double, M_SIZE>, M_SIZE> Matrix;

class Coordinate;

class Transform
{
    public:
        Transform(const Matrix m):
        _m(m) {}
        Transform();
        virtual ~Transform() {}
        const Matrix& getM() const {return _m;}
        Matrix& getM() {return _m;}
		Transform& operator*=(const Transform& t2);
        static Transform newTranslation(double dx, double dy);
        static Transform newRotation(double graus);
		static Transform newRotationAroundPoint(double graus, const Coordinate& p);
        static Transform newScaling(double sx, double sy);
		static Transform newScalingAroundObjCenter(double sx, double sy, const Coordinate& center);
		static double toRadians(double degrees){ return (PI/180) * degrees; }

    private:
        Matrix _m;
};

Transform operator*(Transform t1, const Transform& t2);
std::ostream& operator<<(std::ostream& os, const Transform& t);

#endif // TRANSFORM_HPP
