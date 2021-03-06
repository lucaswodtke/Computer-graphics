#include "Transform.hpp"

// Implementação dos algoritmos de transformada com matrizes

Transform::Transform()
{
    for(int i=0; i<M_SIZE; i++)
        for(int j=0; j<M_SIZE; j++)
            _m[i][j] = 0;
    _m[0][0] = _m[1][1] = _m[2][2] = 1;
}

Transform Transform::newTranslation(double dx, double dy)
{
    Matrix m = {{  {1,  0,  0},
                   {0,  1,  0},
                   {dx, dy, 1}  }};
    return Transform(m);
}

Transform Transform::newScaling(double sx, double sy)
{
    Matrix m = {{  {sx,  0,  0},
                   {0,  sy,  0},
                   {0,   0,  1}  }};
    return Transform(m);
}

Transform Transform::newScalingAroundObjCenter(double sx, double sy, const Coordinate& center)
{
    return newTranslation(-center.x, -center.y) * newScaling(sx, sy) * 
                                                  newTranslation(center.x, center.y);
}

Transform Transform::newRotation(double graus)
{
    double rad = toRadians(graus);
    Matrix m = {{  { cos(rad), sin(rad), 0},
                   {-sin(rad), cos(rad), 0},
                   {        0,        0, 1}  }};
    return Transform(m);
}

Transform Transform::newRotationAroundPoint(double graus, const Coordinate& p)
{
    return newTranslation(-p.x, -p.y) * newRotation(graus) * newTranslation(p.x, p.y);
}

Transform& Transform::operator*=(const Transform& t2)
{
    auto m1 = this->_m;
    const auto &m2 = t2.getM();
    for(int i=0; i<M_SIZE; i++)
        for(int j=0; j<M_SIZE; j++)
        {
            _m[i][j]=0;
            for(int k=0; k<M_SIZE; k++)
                _m[i][j] += m1[i][k] * m2[k][j];
        }

    return *this;
}

Transform operator*(Transform t1, const Transform& t2)
{
    t1 *= t2;
    return t1;
}

std::ostream& operator<<(std::ostream& os, const Transform& t)
{
    const auto &m = t.getM();
    for(int i=0; i<M_SIZE; i++)
    {
        for(int j=0; j<M_SIZE; j++)
        {
            os.width(10);
            os << m[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}
