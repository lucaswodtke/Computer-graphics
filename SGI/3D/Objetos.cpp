#include "Objetos.hpp"

Transform SuperficieBSpline::_M({{
            {-1.0/6.0,     0.5,  -0.5, 1.0/6.0},
            {     0.5,      -1,   0.5,       0},
            {    -0.5,       0,   0.5,       0},
            { 1.0/6.0, 4.0/6.0, 1/6.0,       0},}});

Transform SuperficieBSpline::_Mt = SuperficieBSpline::_M.transpor();

std::ostream& operator<<(std::ostream& os, const Coordinate& c)
{
    os << "x: " << c.x << " - y: " << c.y << " - z: " << c.z << std::endl;
    return os;
}

Coordinate& Coordinate::operator+=(double step)
{
    this->x += step;
    this->y += step;
    this->z += step;
    return *this;
}

Coordinate& Coordinate::operator-=(double step)
{
    this->x -= step;
    this->y -= step;
    this->z -= step;
    return *this;
}

Coordinate& Coordinate::operator+=(const Coordinate& c)
{
    this->x += c.x;
    this->y += c.y;
    this->z += c.z;
    return *this;
}

Coordinate& Coordinate::operator-=(const Coordinate& c)
{
    this->x -= c.x;
    this->y -= c.y;
    this->z -= c.z;
    return *this;
}

Coordinate Coordinate::operator-() const
{
    return Coordinate(-this->x, -this->y, -this->z);
}

Coordinate operator-(const Coordinate& c1, const Coordinate& c2)
{
    return Coordinate(c1.x - c2.x, c1.y - c2.y, c1.z - c2.z);
}

Coordinate& Coordinate::operator*=(const Transform& t)
{
    const auto &m = t.getM();
    double x = this->x, y = this->y, z = this->z, w = this->w;
    this->x = x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0];
    this->y = x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1];
    this->z = x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2];
    this->w = x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3];
    return *this;
}

Coordinate Objeto::center() const
{
    Coordinate c;
    int n = _coords.size();

    for(auto &p : _coords)
    {
        c.x += p.x;
        c.y += p.y;
	    c.z += p.z;
    }
    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Objeto::centerN() const
{
    Coordinate c;
    int n = _coordsN.size();
    for(auto &p : _coordsN)
    {
        c.x += p.x;
        c.y += p.y;
	    c.z += p.z;
    }
    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Objeto3D::center() const
{
    Coordinate c;
    int n = 0;

    for(auto face : _listaFaces)
    {
        for(auto p : face.getCoords())
        {
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += face.getCoords().size();
    }

    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Objeto3D::centerN() const
{
    Coordinate c;
    int n = 0;

    for(auto face : _listaFaces)
    {
        for(auto p : face.getCoordsN())
        {
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += face.getCoordsN().size();
    }
    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Superficie::center() const
{
    Coordinate c;
    int n = 0;
    for(auto curva : _listaCurvas)
    {
        for(auto p : curva.getCoords())
        {
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += curva.getCoords().size();
    }
    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

Coordinate Superficie::centerN() const
{
    Coordinate c;
    int n = 0;
    for(auto curva : _listaCurvas)
    {
        for(auto p : curva.getCoordsN())
        {
            c.x += p.x;
            c.y += p.y;
            c.z += p.z;
        }
        n += curva.getCoordsN().size();
    }
    c.x /= n;
    c.y /= n;
    c.z /= n;
    return c;
}

void Objeto::transform(const Transform& t)
{
    for(auto &p : _coords)
        p *= (t);
}

void Objeto::transformN(const Transform& t)
{
     //if(_coordsN.size() > 0)	
     _coordsN.clear();
     for(auto p : _coords)
	_coordsN.push_back( (p *= t) );
}

void Objeto3D::transform(const Transform& t)
{
    for(auto &face : _listaFaces)
	face.transform(t);
}

void Objeto3D::transformN(const Transform& t)
{
    for(auto &face : _listaFaces)
        face.transformN(t);
}

void Objeto::setNCoord(const Coordinates& c)
{
    _coordsN.clear();
    _coordsN.insert(_coordsN.end(), c.begin(), c.end());
}

void CurvaBezier::gerarCurva(const Coordinates& cpCoords)
{
    if(_points.size() != 0)
        return;
    setPontos(cpCoords);
    const auto& coords = _points;
    int numCurvas = ((coords.size()-4)/3) + 1;
    for(int i = 0; i<numCurvas; i++)
    {
        // Blending Function
        for(float t = 0; t<1; t += _step)
        {
            double t2 = t * t;
            double t3 = t2 * t;
            double x,y,z;
            x = (-t3 +3*t2 -3*t +1)*coords[i*3+0].x + (3*t3 -6*t2 +3*t)*coords[i*3+1].x +
                (-3*t3 +3*t2)*coords[i*3+2].x + (t3)*coords[i*3+3].x;
            y = (-t3 +3*t2 -3*t +1)*coords[i*3+0].y + (3*t3 -6*t2 +3*t)*coords[i*3+1].y +
                (-3*t3 +3*t2)*coords[i*3+2].y + (t3)*coords[i*3+3].y;
            z = (-t3 +3*t2 -3*t +1)*coords[i*3+0].z + (3*t3 -6*t2 +3*t)*coords[i*3+1].z +
                (-3*t3 +3*t2)*coords[i*3+2].z + (t3)*coords[i*3+3].z;

            _coords.emplace_back(x,y,z);
        }
    }
}

void CurvaBSpline::gerarCurva(const Coordinates& cpCoords)
{
    if(_points.size() != 0)
        return;
    setPontos(cpCoords);
    const auto& coords = _points;
    int numCurvas = coords.size()-3;
    double t = _step;
    double t2 = t * _step;
    double t3 = t2 * _step;
    double n16 = 1.0/6.0;
    double n23 = 2.0/3.0;
    for(int i = 0; i < numCurvas; i++)
    {
        auto c1 = coords[i];
        auto c2 = coords[i+1];
        auto c3 = coords[i+2];
        auto c4 = coords[i+3];
        double ax = -n16 * c1.x  +0.5 * c2.x -0.5 * c3.x +n16 * c4.x;
        double bx =  0.5 * c1.x  -      c2.x +0.5 * c3.x;
        double cx = -0.5 * c1.x              +0.5 * c3.x;
        double dx =  n16 * c1.x  +n23 * c2.x +n16 * c3.x;
        double deltaX = ax*t3 +bx*t2 +cx*t;
        double deltaX3 = ax*(6*t3);
        double deltaX2 = deltaX3 +bx*(2*t2);
        double ay = -n16 * c1.y  +0.5 * c2.y -0.5 * c3.y +n16 * c4.y;
        double by =  0.5 * c1.y  -      c2.y +0.5 * c3.y;
        double cy = -0.5 * c1.y              +0.5 * c3.y;
        double dy =  n16 * c1.y  +n23 * c2.y +n16 * c3.y;
        double deltaY = ay*t3 +by*t2 +cy*t;
        double deltaY3 = ay*(6*t3);
        double deltaY2 = deltaY3 +by*(2*t2);
	    double az = -n16 * c1.z  +0.5 * c2.z -0.5 * c3.z +n16 * c4.z;
        double bz =  0.5 * c1.z  -      c2.z +0.5 * c3.z;
        double cz = -0.5 * c1.z              +0.5 * c3.z;
        double dz =  n16 * c1.z  +n23 * c2.z +n16 * c3.z;
        double deltaZ = az*t3 +bz*t2 +cz*t;
        double deltaZ3 = az*(6*t3);
        double deltaZ2 = deltaZ3 +bz*(2*t2);
        double vx = dx, vy = dy, vz = dz;
        addCoordinate(vx, vy, dz);
        for(float t = 0.0; t < 1.0; t += _step)
        {
            double x = vx, y = vy, z = vz;
            x += deltaX;
            deltaX += deltaX2;
            deltaX2 += deltaX3;
            y += deltaY;
            deltaY += deltaY2;
            deltaY2 += deltaY3;
	        z += deltaZ;
            deltaZ += deltaZ2;
            deltaZ2 += deltaZ3;
            addCoordinate(x,y,z);
            vx = x;
            vy = y;
	        vz = z;
        }
    }
}

void Superficie::transform(const Transform& t)
{
    for(auto &curva : _listaCurvas)
	curva.transform(t);
}

void Superficie::transformN(const Transform& t)
{
    for(auto &curva : _listaCurvas)
	curva.transformN(t);
}

void SuperficieBezier::gerarSuperficie(const Coordinates& cpCoords)
{
    if(_pontosControle.size() != 0)
        return;
    setPontosControle(cpCoords);
    const auto& coords = _pontosControle;
    int tmp3xMaxLines = 3*m_maxLines, tmp3xMaxCols = 3*m_maxCols;

    for(int nLine = 0; (m_maxLines != 4 && nLine <= tmp3xMaxLines) || (nLine < tmp3xMaxLines); nLine += tmp3xMaxCols)
    {
        for(int nCol = 0; nCol < m_maxCols-1; nCol += 3)
        {
            for(float s = 0.0; s <= 1.0; s += _step)
            {
                double s2 = s * s;
                double s3 = s2 * s;
                Curva2 curva("curva"+std::to_string(s));
                for(float t = 0.0; t <= 1.0; t += _step)
                {
                    double t2 = t * t;
                    double t3 = t2 * t;
                    curva.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
                }
                _listaCurvas.insert(_listaCurvas.end(), curva);
            }
            for(float t = 0.0; t <= 1.0; t += _step)
            {
                double t2 = t * t;
                double t3 = t2 * t;
                Curva2 curva("curva"+std::to_string(t));
                for(float s = 0.0; s <= 1.0; s += _step)
                {
                    double s2 = s * s;
		            double s3 = s2 * s;
                    curva.addCoordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
                }
                _listaCurvas.insert(_listaCurvas.end(), curva);
            }
        }
    }
}

Coordinate SuperficieBezier::blendingFunction(float s, double s2, double s3, float t, double t2, double t3, int nLine, int nCol, const Coordinates& coords)
{
    double x = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].x +(s3)*coords[m_maxCols*3+2+nLine+nCol].x) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].x +(s3)*coords[m_maxCols*3+3+nLine+nCol].x))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].x +(s3)*coords[m_maxCols*3+2+nLine+nCol].x))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].x +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].x +(s3)*coords[m_maxCols*3+1+nLine+nCol].x))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].x +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].x   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].x   +(s3)*coords[m_maxCols*3+nLine+nCol].x));
     double y = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].y +(s3)*coords[m_maxCols*3+2+nLine+nCol].y) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].y +(s3)*coords[m_maxCols*3+3+nLine+nCol].y))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].y +(s3)*coords[m_maxCols*3+2+nLine+nCol].y))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].y +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].y +(s3)*coords[m_maxCols*3+1+nLine+nCol].y))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].y +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].y   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].y   +(s3)*coords[m_maxCols*3+nLine+nCol].y));
    double z = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].z +(s3)*coords[m_maxCols*3+2+nLine+nCol].z) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol].z +(s3)*coords[m_maxCols*3+3+nLine+nCol].z))*t3
     +(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol].z +(s3)*coords[m_maxCols*3+2+nLine+nCol].z))*t2
     +(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol].z +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol].z +(s3)*coords[m_maxCols*3+1+nLine+nCol].z))*t
     +(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol].z +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol].z   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol].z   +(s3)*coords[m_maxCols*3+nLine+nCol].z));
    return Coordinate(x,y,z);
}

void SuperficieBSpline::gerarSuperficie(const Coordinates& cpCoords)
{
    if(_pontosControle.size() != 0)
        return;
    setPontosControle(cpCoords);
    double delta_s = 1.0 / (_ns-1);
    double delta_t = 1.0 / (_nt-1);
    criarDeltaMatrizes(delta_s, delta_t);
    for(int nLine = 0; (m_maxLines != 4 && nLine <= m_maxLines) || (nLine < m_maxLines); nLine += m_maxCols)
    {
        for(int nCol = 0; nCol <= m_maxCols-4; nCol += 1)
        {
            atualizarCoordsMatrizes(nLine, nCol);
            calcCoeficientes();
            criarForwardDiffMatrizes();
            for(int i = 0; i < _ns; i++)
            {
                gerarCurva(_ns,_DDx[0][0], _DDx[0][1], _DDx[0][2], _DDx[0][3], 
                               _DDy[0][0], _DDy[0][1], _DDy[0][2], _DDy[0][3], 
                               _DDz[0][0], _DDz[0][1], _DDz[0][2], _DDz[0][3]);
                atualizarForwardDiffMatrizes();
            }
            criarForwardDiffMatrizes();
            _DDx = Transform(_DDx).transpor().getM();
            _DDy = Transform(_DDy).transpor().getM();
            _DDz = Transform(_DDz).transpor().getM();
            for(int i = 0; i < _nt; i++)
            {
                gerarCurva(_nt, _DDx[0][0], _DDx[0][1], _DDx[0][2], _DDx[0][3],
                                _DDy[0][0], _DDy[0][1], _DDy[0][2], _DDy[0][3],
                                _DDz[0][0], _DDz[0][1], _DDz[0][2], _DDz[0][3]);
                atualizarForwardDiffMatrizes();
            }
        }
     }
}

void SuperficieBSpline::atualizarCoordsMatrizes(int nLine, int nCol)
{
    for(int i = 0; i < 4; i++)
    {
        int tmp = m_maxCols*i+nLine+nCol;
        for(int j = 0; j < 4; j++)
        {	
            auto &coord = _pontosControle[tmp+j];
            _coordsX[i][j] = coord.x;
            _coordsY[i][j] = coord.y;
            _coordsZ[i][j] = coord.z;
        }
    }
}

void SuperficieBSpline::atualizarForwardDiffMatrizes()
{
    //row1 <- row1 + row2
    _DDx[0][0] += _DDx[1][0]; _DDx[0][1] += _DDx[1][1]; _DDx[0][2] += _DDx[1][2]; _DDx[0][3] += _DDx[1][3];
    _DDy[0][0] += _DDy[1][0]; _DDy[0][1] += _DDy[1][1]; _DDy[0][2] += _DDy[1][2]; _DDy[0][3] += _DDy[1][3];
    _DDz[0][0] += _DDz[1][0]; _DDz[0][1] += _DDz[1][1]; _DDz[0][2] += _DDz[1][2]; _DDz[0][3] += _DDz[1][3];
    //row2 <- row2 + row3
    _DDx[1][0] += _DDx[2][0]; _DDx[1][1] += _DDx[2][1]; _DDx[1][2] += _DDx[2][2]; _DDx[1][3] += _DDx[2][3];
    _DDy[1][0] += _DDy[2][0]; _DDy[1][1] += _DDy[2][1]; _DDy[1][2] += _DDy[2][2]; _DDy[1][3] += _DDy[2][3];
    _DDz[1][0] += _DDz[2][0]; _DDz[1][1] += _DDz[2][1]; _DDz[1][2] += _DDz[2][2]; _DDz[1][3] += _DDz[2][3];
    //row3 <- row3 + row4
    _DDx[2][0] += _DDx[3][0]; _DDx[2][1] += _DDx[3][1]; _DDx[2][2] += _DDx[3][2]; _DDx[2][3] += _DDx[3][3];
    _DDy[2][0] += _DDy[3][0]; _DDy[2][1] += _DDy[3][1]; _DDy[2][2] += _DDy[3][2]; _DDy[2][3] += _DDy[3][3];
    _DDz[2][0] += _DDz[3][0]; _DDz[2][1] += _DDz[3][1]; _DDz[2][2] += _DDz[3][2]; _DDz[2][3] += _DDz[3][3];
}

void SuperficieBSpline::calcCoeficientes()
{
    Transform coordsX(_coordsX);
    _cx = ((_M * coordsX) * _Mt).getM();
    Transform coordsY(_coordsY);
    _cy = ((_M * coordsY) * _Mt).getM();
    Transform coordsZ(_coordsZ);
    _cz = ((_M * coordsZ) * _Mt).getM();
}

void SuperficieBSpline::criarDeltaMatrizes(double delta_s, double delta_t)
{
    double s = delta_s;
    double s2 = s*s;
    double s3 = s2*s;
    _es[0][0] = 0.0;
    _es[0][1] = 0.0;
    _es[0][2] = 0.0;
    _es[0][3] = 1.0;
    _es[1][0] = s3;
    _es[1][1] = s2;
    _es[1][2] = s;
    _es[1][3] = 0.0;
    _es[2][0] = 6.0*s3;
    _es[2][1] = 2.0*s2;
    _es[2][2] = 0.0;
    _es[2][3] = 0.0;
    _es[3][0] = 6.0*s3;
    _es[3][1] = 0.0;
    _es[3][2] = 0.0;
    _es[3][3] = 0.0;
    double t = delta_t;
    double t2 = t*t;
    double t3 = t2*t;
    _et[0][0] = 0.0;
    _et[1][0] = 0.0;
    _et[2][0] = 0.0;
    _et[3][0] = 1.0;
    _et[0][1] = t3;
    _et[1][1] = t2;
    _et[2][1] = t;
    _et[3][1] = 0.0;
    _et[0][2] = 6.0*t3;
    _et[1][2] = 2.0*t2;
    _et[2][2] = 0.0;
    _et[3][2] = 0.0;
    _et[0][3] = 6.0*t3;
    _et[1][3] = 0.0;
    _et[2][3] = 0.0;
    _et[3][3] = 0.0;
}

void SuperficieBSpline::criarForwardDiffMatrizes()
{
    Transform es(_es), et(_et);
    _DDx = ((es * _cx) * et).getM();
    _DDy = ((es * _cy) * et).getM();
    _DDz = ((es * _cz) * et).getM();
}

void SuperficieBSpline::gerarCurva(double n, double x, double Dx, double D2x, double D3x, double y, double Dy, double D2y, double D3y, double z, double Dz, double D2z, double D3z)
{
    CurvaBSpline curva("curva"+std::to_string(_listaCurvas.size()));
    curva.addCoordinate(x,y,z);
    for(int i = 0; i < (n-1); i++)
    {
        x += Dx;  Dx += D2x;  D2x += D3x;
	    y += Dy;  Dy += D2y;  D2y += D3y;
	    z += Dz;  Dz += D2z;  D2z += D3z;
	    curva.addCoordinate(x,y,z);
    }
    _listaCurvas.push_back(curva);
}





















