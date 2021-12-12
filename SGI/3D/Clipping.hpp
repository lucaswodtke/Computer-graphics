#ifndef CLIPPING_HPP
#define CLIPPING_HPP

#include "Objetos.hpp"

// Usado para verificar se o clipping está ok
class ClipWindow : public Poligono 
{
    public:
        ClipWindow(double minX_, double maxX_, double minY_, double maxY_);
        void addCoordinate(double x, double y) {_coords.emplace_back(x,y);
                                                _coordsN.emplace_back(x,y);}
        double minX, maxX, minY, maxY;
};

// seleção entre Cohen-Suterland e Liang-Barsky  
enum class AlgoritmoClipping { CS, LB };

class Clipping
{
    public:
        Clipping(const ClipWindow* window):
            _w(window) {}
        virtual ~Clipping() {}
        void setClipAlg(const AlgoritmoClipping alg){ _current = alg; }
        bool clip(Objeto* obj);

    private:
        bool clipPonto(const Coordinate& c);
        bool clipReta(Coordinate& c1, Coordinate& c2);
        bool clipPoligono(Objeto* obj){return SutherlandHodgman(obj);}
        bool clipCurva2(Objeto *obj);
        int getCoordRC(const Coordinate& c);
        bool CohenSutherland(Coordinate& c1, Coordinate& c2);
        bool LiangBarsky(Coordinate& c1, Coordinate& c2);
        bool SutherlandHodgman(Objeto* obj);
        void clipEast(Coordinates& input, Coordinates& output);
        void clipWest(Coordinates& input, Coordinates& output);
        void clipNorth(Coordinates& input, Coordinates& output);
        void clipSouth(Coordinates& input, Coordinates& output);

    private:
        const ClipWindow* _w;
        AlgoritmoClipping _current = AlgoritmoClipping::CS;

        enum RC {MID=0, WEST=1, EAST=2, SOUTH=4, NORTH=8};
};

ClipWindow::ClipWindow(double minX_, double maxX_, double minY_, double maxY_): Poligono("_border_") 
{
    minX = minX_; maxX = maxX_;
    minY = minY_; maxY = maxY_;
    addCoordinate(minX,minY);
    addCoordinate(maxX,minY);
    addCoordinate(maxX,maxY);
    addCoordinate(minX,maxY);
}

bool Clipping::clip(Objeto* obj)
{
    switch(obj->getType())
    {
        case ObjType::PONTO:
            return clipPonto(obj->getCoordN(0));
        case ObjType::RETA:
            return clipReta(obj->getCoordN(0), obj->getCoordN(1));
        case ObjType::POLIGONO:
            return clipPoligono(obj);
        case ObjType::CURVABEZIER:
        case ObjType::CURVABSPLINE:
            return clipCurva2(obj);
        case ObjType::OBJETO3D:
        {
            Objeto3D *obj3d = (Objeto3D*) obj;
            bool draw = false;
            for(auto &face : obj3d->getListaFaces())
            {
                bool tmp = clipPoligono(&face);
                if(!tmp){ face.getCoordsN().clear(); }
                draw |= tmp;
            }
            return draw;
        }
        case ObjType::SUPERFICIE_BEZIER:
        case ObjType::SUPERFICIE_BSPLINE:
        {
            Superficie *superficie = (Superficie*) obj;
            bool draw = false;
            for(auto &curva : superficie->getListaCurvas())
            {
                bool tmp = clipCurva2(&curva);
                if(!tmp){ curva.getCoordsN().clear(); }
                draw |= tmp;
            }
            return draw;
        }
    } // End switch
    return false;
}

bool Clipping::clipPonto(const Coordinate& c)
{
    return c.x >= _w->minX && c.x <= _w->maxX && c.y >= _w->minY && c.y <= _w->maxY;
}

bool Clipping::clipReta(Coordinate& c1, Coordinate& c2)
{
    if(_current == AlgoritmoClipping::CS)
        return CohenSutherland(c1,c2);
    else
        return LiangBarsky(c1,c2);
}


int Clipping::getCoordRC(const Coordinate& c)
{
    int rc = Clipping::RC::MID;
    if(c.x < _w->minX)
        rc |= Clipping::RC::WEST;
    else if(c.x > _w->maxX)
        rc |= Clipping::RC::EAST;
    if(c.y < _w->minY)
        rc |= Clipping::RC::SOUTH;
    else if(c.y > _w->maxY)
        rc |= Clipping::RC::NORTH;
    return rc;
}

// CS
bool Clipping::CohenSutherland(Coordinate& c1, Coordinate& c2)
{
    if(c1 == c2) return clipPonto(c1);
    int rc1 = getCoordRC(c1);
    int rc2 = getCoordRC(c2);
    while(true)
    {
        if( (rc1 | rc2) == 0 )
            return true; // inside
        else if( (rc1 & rc2) != 0 )
            return false; // outside

        double x,y;
        int rc = rc1 ? rc1 : rc2;
        double m = (c2.y-c1.y)/(c2.x-c1.x);

        if(rc & Clipping::RC::NORTH)
        {
            x = c1.x + 1/m * (_w->maxY-c1.y);
            y = _w->maxY;
        }
        else if(rc & Clipping::RC::SOUTH)
        {
            x = c1.x + 1/m * (_w->minY-c1.y);
            y = _w->minY;
        }
        else if(rc & Clipping::RC::EAST)
        {
            y = m * (_w->maxX-c1.x) + c1.y;
            x = _w->maxX;
        }
        else if(rc & Clipping::RC::WEST)
        {
            y = m * (_w->minX-c1.x) + c1.y;
            x = _w->minX;
        }
        if(rc == rc1)
        {
            c1.x = x;
            c1.y = y;
            rc1 = getCoordRC(c1);
        }
        else
        {
            c2.x = x;
            c2.y = y;
            rc2 = getCoordRC(c2);
        }
    }
}

// LB
bool Clipping::LiangBarsky(Coordinate& c1, Coordinate& c2)
{
    if(c1 == c2) return clipPonto(c1);
    auto delta = c2 - c1;
    double p,q,r;
    double u1 = 0.0, u2 = 1.0;
    for(int pos = 0; pos<4; pos++)
    {
        if     (pos == 0){ p = -delta.x; q = c1.x - _w->minX; }
        else if(pos == 1){ p =  delta.x; q = _w->maxX - c1.x; }
        else if(pos == 2){ p = -delta.y; q = c1.y - _w->minY; }
        else if(pos == 3){ p =  delta.y; q = _w->maxY - c1.y; }
        if(p == 0 && q < 0)
            return false;
        r = q/p;
        if(p < 0)
        {
            if(r > u1)
                u1 = r;
        }
        else if(p > 0)
        {
            if(r < u2)
                u2 = r;
        }
    }
    if(u1 > u2) return false;
    if(u2 < 1)
    {
        c2.x = c1.x + u2*delta.x;
        c2.y = c1.y + u2*delta.y;
    }
    if(u1 > 0)
    {
        c1.x = c1.x + u1*delta.x;
        c1.y = c1.y + u1*delta.y;
    }
    return true;
}

bool Clipping::SutherlandHodgman(Objeto* obj)
{
    auto &input = obj->getCoordsN();
    Coordinates tmp;
    clipWest(input, tmp);
    clipEast(tmp, input);
    clipNorth(input, tmp);
    clipSouth(tmp, input);
    return(input.size() != 0);
}

void Clipping::clipWest(Coordinates& input, Coordinates& output)
{
    if(output.size() > 0)
        output.clear();
    if(input.size() == 0)
        return;
    double clipX = _w->minX;
    input.push_back(input[0]);
    for(unsigned int i = 0; i < input.size()-1; i++)
    {
        Coordinate c0 = input[i];
        Coordinate c1 = input[i+1];

        // fora pra fora
        if(c0.x < clipX && c1.x < clipX)
        { 
            continue; 
        }
        // dentro pra dentro
        if(c0.x >= clipX && c1.x >= clipX)
        {
            output.push_back(c1);
	        continue;
	    }
        double x = clipX;
        double m = (c1.y-c0.y)/(c1.x-c0.x);
        double y = m * (x-c0.x) + c0.y;
        // dentro pra fora
        if(c0.x >= clipX && c1.x < clipX)
        {
            output.emplace_back(x,y);
	        continue;
	    }
        // fora pra dentro
        if(c0.x < clipX && c1.x >= clipX)
        {
            output.emplace_back(x,y);
            output.push_back(c1);
        }
    }
}

void Clipping::clipEast(Coordinates& input, Coordinates& output)
{
    if(output.size() > 0)
        output.clear();
    if(input.size() == 0)
        return;

    double clipX = _w->maxX;
    input.push_back(input[0]);
    for(unsigned int i = 0; i < input.size()-1; i++)
    {
        Coordinate c0 = input[i];
        Coordinate c1 = input[i+1];

        // fora para fora
        if(c0.x > clipX && c1.x > clipX)
        { 
            continue; 
        }
        // dentro pra dentro
        if(c0.x <= clipX && c1.x <= clipX)
        {
            output.push_back(c1);
	        continue;
	    }
        double x = clipX;
        double m = (c1.y-c0.y)/(c1.x-c0.x);
        double y = m * (x-c0.x) + c0.y;
        // dentro pra fora
        if(c0.x <= clipX && c1.x > clipX)
        {
            output.emplace_back(x,y);
	        continue;
	    }
        // fora pra dentro
        if(c0.x > clipX && c1.x <= clipX)
        {
            output.emplace_back(x,y);
            output.push_back(c1);
        }
    }
}

void Clipping::clipNorth(Coordinates& input, Coordinates& output)
{
    if(output.size() > 0)
        output.clear();
    if(input.size() == 0)
        return;
    double clipY = _w->maxY;
    input.push_back(input[0]);
    for(unsigned int i = 0; i < input.size()-1; i++)
    {
        Coordinate c0 = input[i];
        Coordinate c1 = input[i+1];
        // fora pra fora
        if(c0.y > clipY && c1.y > clipY)
        { 
            continue; 
        }
        // dentro pra dentro
        if(c0.y <= clipY && c1.y <= clipY)
        {
            output.push_back(c1);
	        continue;
	    }
        double y = clipY;
        double m = (c1.x-c0.x)/(c1.y-c0.y);
        double x = m * (y-c0.y) + c0.x;
        // dentro pra fora
        if(c0.y <= clipY && c1.y > clipY)
        {
            output.emplace_back(x,y);
	        continue;
	    }
        // fora pra dentro
        if(c0.y > clipY && c1.y <= clipY)
        {
            output.emplace_back(x,y);
            output.push_back(c1);
        }
    }
}

void Clipping::clipSouth(Coordinates& input, Coordinates& output)
{
    if(output.size() > 0)
        output.clear();
    if(input.size() == 0)
        return;
    double clipY = _w->minY;
    input.push_back(input[0]);
    for(unsigned int i = 0; i < input.size()-1; i++)
    {
        Coordinate c0 = input[i];
        Coordinate c1 = input[i+1];
        // fora pra fora
        if(c0.y < clipY && c1.y < clipY){ continue; }
        // dentro pra dentro
        if(c0.y >= clipY && c1.y >= clipY)
        {
            output.push_back(c1);
	        continue;
	    }
        double y = clipY;
        double m = (c1.x-c0.x)/(c1.y-c0.y);
        double x = m * (y-c0.y) + c0.x;
        // dentro pra fora
        if(c0.y >= clipY && c1.y < clipY)
        {
            output.emplace_back(x,y);
	        continue;
	    }
        //Caso 4: fora pra dentro
        if(c0.y < clipY && c1.y >= clipY)
        {
            output.emplace_back(x,y);
            output.push_back(c1);
        }
    }
}

bool Clipping::clipCurva2(Objeto *obj)
{
    auto& coords = obj->getCoordsN();
    Coordinates newPath;
    bool prevInside = true;
    Coordinate prev;
    for(unsigned int i = 0; i < coords.size(); i++)
    {
        if(clipPonto(coords[i]))
        {
            if(!prevInside)
            {
                clipReta(prev, coords[i]);
                newPath.push_back(prev);
            }
            newPath.push_back(coords[i]);
            prevInside = true;
        }
        else
        {
            if(prevInside && newPath.size() != 0)
            {
                clipReta(prev, coords[i]);
                newPath.push_back(coords[i]);
            }
            prevInside = false;
        }
        prev = coords[i];
    }
    if(newPath.size() == 0)
        return false;
    obj->setNCoord(newPath);
    return true;
}

#endif // CLIPPING_HPP
