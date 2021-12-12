#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Objetos.hpp"

#define MIN_SIZE 1
#define MAX_SIZE 10000

// Classe que representa a Janela e seus métodos de acesso
class Window
{
    public:
        Window(double vWidth, double vHeight): _center( (vWidth/2)*0.95,(vHeight/2)*0.95 ), 
                                    _angulo(0), _width( (vWidth/2) ), _height( (vHeight/2) ) {}
        virtual ~Window() {}
        Transform& getT(){ return _t; }
        Coordinate wMin() const { return Coordinate(-1,-1); }
        Coordinate wMax() const { return Coordinate(1,1); }
        Coordinate center(){ return _center; }
        double getWidth(){return _width;}
        double getHeight(){return _height;}
        double getAngulo(){ return _angulo; }
        void setAngulo(double graus){ _angulo += graus; }
        void zoom(double step);
        void move(double x, double y);
        void updateTransform();
    protected:
    private:
        Coordinate _center;
        double _angulo;
        double _width, _height;
        Transform _t;
};

void Window::updateTransform()
{
    _t = Transform();
    _t *= Transform::newTranslation(-_center.x, -_center.y);
    _t *= Transform::newRotation(-_angulo);
    _t *= Transform::newScaling(1/_width, 1/_height);
}

void Window::zoom(double step)
{
    if( (getWidth()+step <= MIN_SIZE || getHeight()+step <= MIN_SIZE) && step < 0 )
        throw "Zoom máximo atingido.";
    else if( (getWidth()+step >= MAX_SIZE || getHeight()+step >= MAX_SIZE) && step > 0)
        throw "Zoom mínimo atingido.";
    _width += step;
    _height += step;
}

void Window::move(double x, double y)
{
    _center.x += x;
    _center.y += y;
}

#endif // WINDOW_HPP
