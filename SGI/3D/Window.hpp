#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Objetos.hpp"

// Tamanho da janela
#define MINIMO 1
#define MAXIMO 10000

class Window
{
    public:
        Window(double vWidth, double vHeight): _width( (vWidth/2) ), _height( (vHeight/2) ) {}
        virtual ~Window() {}
        Transform& getT(){ return _t; }
        Coordinate wMin() const { return Coordinate(-1,-1); }
        Coordinate wMax() const { return Coordinate(1,1); }
        Coordinate center(){ return _center; }
        double getWidth(){return _width;}
        double getHeight(){return _height;}
	    double getAnguloX(){ return _anguloX; }
        double getAnguloY(){ return _anguloY; }
        double getAnguloZ(){ return _anguloZ; }
	    void setAnguloX(double graus);
        void setAnguloY(double graus);
        void setAnguloZ(double graus);
        void zoom(double step);
        void move(double x, double y, double z=0.0);
        void updateTransform();
        void setProjecao();
    private:
        Coordinate _center;
        double _anguloX = 0.0, _anguloY = 0.0, _anguloZ = 0.0;
        double _width, _height;
        Transform _t;
};

void Window::setProjecao()
{
    // TODO fix
}

void Window::updateTransform()
{
    _t = Transform();
    _t *= Transform::newTranslation(-_center.x, -_center.y, -_center.z);
    _t *= Transform::newRotation(-_anguloX, -_anguloY, -_anguloZ);
    _t *= Transform::newScaling(1.0/_width, 1.0/_height, 2.0/(_width + _height));
}

void Window::zoom(double step)
{
    step = getWidth()*(step/100);
    if( (getWidth()+step <= MINIMO || getHeight()+step <= MINIMO) && step < 0 )
        throw "Máximo nível de zoom atingido.";
    else if( (getWidth()+step >= MAXIMO || getHeight()+step >= MAXIMO) && step > 0)
        throw "Mínimo nível de zoom atingido.";
    _width += step;
    _height += step;
}

void Window::move(double x, double y, double z)
{
    Coordinate c(x,y,z);
    _center.x += c.x;
    _center.y += c.y;
    _center.z += c.z;
}

void Window::setAnguloX(double graus)
{
    _anguloX += graus;
}

void Window::setAnguloY(double graus)
{
    _anguloY += graus;
}

void Window::setAnguloZ(double graus)
{
    _anguloZ += graus;
}

#endif // WINDOW_HPP
