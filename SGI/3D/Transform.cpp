#include "Transform.hpp"

// Algoritmos de transformada, translação, rotação

Transform::Transform()
{
    for(int i=0; i<TAMANHO; i++)
        for(int j=0; j<TAMANHO; j++)
            _matrix[i][j] = 0;
    _matrix[0][0] = _matrix[1][1] =
    _matrix[2][2] = _matrix[3][3] = 1;
}

Transform Transform::newTranslation(double dx, double dy, double dz)
{
    Matriz4x4 m = {{{1, 0, 0, 0},
                     {0, 1, 0, 0},
                     {0, 0, 1, 0},
                     {dx, dy, dz, 1}}};
    return Transform(m);
}

Transform Transform::newScaling(double sx, double sy, double sz)
{
    Matriz4x4 m = {{{sx, 0, 0, 0},
                    {0, sy, 0, 0},
                    {0, 0, sz, 0},
                    {0, 0,  0, 1}}};
    return Transform(m);
}

Transform Transform::newScalingAroundObjCenter(double sx, double sy, double sz, const Coordinate& center)
{
    return newTranslation(-center.x, -center.y, -center.z) * newScaling(sx, sy, sz) * newTranslation(center.x, center.y, center.z);
}

Transform Transform::newRx(double anguloX, bool isRad)
{
    double rad = isRad ? anguloX : toRadians(anguloX);
    Matriz4x4 m = {{{1, 0, 0, 0},
                     {0, cos(rad), sin(rad), 0},
                     {0, -sin(rad), cos(rad), 0},
                     {0, 0, 0, 1}}};
    return Transform(m);
}

Transform Transform::newRy(double anguloY, bool isRad)
{
    double rad = isRad ? anguloY : toRadians(anguloY);
    Matriz4x4 m = {{{cos(rad), 0, -sin(rad), 0},
                     {0, 1, 0, 0},
                     {sin(rad), 0, cos(rad),  0},
                     {0, 0, 0, 1}}};
    return Transform(m);
}

Transform Transform::newRz(double anguloZ, bool isRad)
{
    double rad = isRad ? anguloZ : toRadians(anguloZ);
    Matriz4x4 m = {{{cos(rad), sin(rad), 0, 0},
                     {-sin(rad), cos(rad), 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};
    return Transform(m);
}

Transform Transform::newRa(double anguloA, const Coordinate& p)
{
    if(anguloA == 0 || (p.x == 0 && p.y == 0 && p.z == 0))
        return Transform();

    double d = sqrt(p.y*p.y + p.z*p.z);
    double beta = atan(p.x/p.z);
    double alfa = atan(p.y/d);

    return Transform::newTranslation(-p.x, -p.y, -p.z) * Transform::newRy(-beta,true) *
           Transform::newRx(alfa,true) * Transform::newRz(anguloA) *
           Transform::newRx(-alfa,true) * Transform::newRy(beta,true) *
           Transform::newTranslation(p.x, p.y, p.z);
}

Transform Transform::newRotation(double anguloX, double anguloY, double anguloZ)
{
    return newRx(anguloX) * newRy(anguloY) * newRz(anguloZ);
}

Transform Transform::newFullRotation(double anguloX, double anguloY, double anguloZ, double anguloA, const Coordinate& p)
{
    return newRx(anguloX) * newRy(anguloY) * newRz(anguloZ) * newRa(anguloA, p);
}

/* 
Transform Transform::novaProjecao(double d)
{
    // TODO
}
*/

Transform& Transform::operator*=(const Transform& t2)
{
    auto m1 = this->_matrix;
    const auto &m2 = t2.getM();
    for(int i=0; i<TAMANHO; i++)
    {
        for(int j=0; j<TAMANHO; j++)
        {
            _matrix[i][j]=0.0;
            for(int k=0; k<TAMANHO; k++)
                _matrix[i][j] += m1[i][k] * m2[k][j];
        }
    }
    return *this;
}

Transform Transform::transpor()
{
    Matriz4x4 t;
    for(int i = 0; i < TAMANHO; i++)
        for(int j = 0; j < TAMANHO; j++)
            t[i][j] = _matrix[j][i];
    return Transform(t);
}

Transform operator*(Transform t1, const Transform& t2)
{
    t1 *= t2;
    return t1;
}

std::ostream& operator<<(std::ostream& os, const Transform& t)
{
    const auto &m = t.getM();
    for(int i=0; i<TAMANHO; i++)
    {
        for(int j=0; j<TAMANHO; j++)
        {
            os.width(10);
            os << m[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}
