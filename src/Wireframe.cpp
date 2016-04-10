/* Wireframe.cpp */

#include <iostream>
#include "Wireframe.hpp"
#include "Point.hpp"
#include "Coord.hpp"
#include "Window.hpp"
#include "AbstractDrawer.hpp"

Wireframe::Wireframe(const std::string name) :
        Shape(name, ShapeType::WIREFRAME) {
}


Wireframe::~Wireframe() {
    auto coord = _wCoords.begin();
    while (coord != _wCoords.end()) {
        delete *coord;
        coord++;
    }
    _wCoords.clear();
    auto point = _vertices.begin();
    while (point != _vertices.end()) {
        delete *point;
        point++;
    }
    _vertices.clear();
}


const Coord<double> Wireframe::get_centroid() {
    double x = 0.0;
    double y = 0.0;
    auto p = _vertices.begin();
    while (p != _vertices.end()) {
        x += (*p)->getX();
        y += (*p)->getY();
        p++;
    }
    x = x / (double) _vertices.size();
    y = y / (double) _vertices.size();
    return Coord<double>(x, y);
}


void Wireframe::add_point(Point *point) {
    _vertices.push_back(point);
}


// Visitor
void Wireframe::accept(AbstractDrawer *drawer) {
    drawer->draw(this);
}


void Wireframe::clip_to_window(Window *w) {
    std::cout << "Clipping to window." << std::endl;

    /* Temporary implementation */
    Coord<double> *coord;
    _wCoords.clear();
    auto p = _vertices.begin();
    while (p != _vertices.end()) {
        coord = new Coord<double>((*p)->getX(), (*p)->getY());
        _wCoords.push_back(coord);
        p++;
    }
}


void Wireframe::transform(TMatrix *matrix) {
    auto p = _vertices.begin();
    while (p != _vertices.end()) {
        (*p)->transform(matrix);
        p++;
    }
}