/* InterfaceController.cpp */

#include <iostream>
#include "InterfaceController.hpp"
#include "MyIGS.hpp"
#include "Canvas.hpp"
#include "ShapeBuilder.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Wireframe.hpp"
#include "Coord.hpp"

InterfaceController::InterfaceController(MyIGS *interface, Canvas *canvas) :
        _interface(interface),
        _canvas(canvas),
        _xViewportMin(0),
        _xViewportMax(500),
        _yViewportMin(0),
        _yViewportMax(500) {
}

InterfaceController::~InterfaceController() {
    auto shape = _shapes.begin();
    while (shape != _shapes.end()) {
        delete *shape;
        shape++;
    }
    _shapes.clear();
}

// Create a new shape based on its type.
void InterfaceController::createShape(ShapeType type) {
    Shape *shape = ShapeBuilder::instance()->createShape(type);
    this->finalizeShapeCreation(shape);
}

void InterfaceController::createPoint(std::string name, const double xPos, const double yPos) {
    Point *point = new Point(name, xPos, yPos);
    InterfaceController::finalizeShapeCreation(point);
}

void InterfaceController::createLine(std::string name, const double x1Pos, const double y1Pos,
        const double x2Pos, const double y2Pos) {

    std::cout << "Creating a new line." << std::endl;

    const std::string p1_name = name + "_p1";
    const std::string p2_name = name + "_p2";
    Point *p1 = new Point(p1_name , x1Pos, y1Pos);
    Point *p2 = new Point(p2_name , x2Pos, y2Pos);
    Line *line = new Line(name, p1, p2);
    InterfaceController::finalizeShapeCreation(line);

    Wireframe *w2 = new Wireframe("w2");
    w2->addPoint(new Point("ppp",10.0,10.0));
    w2->addPoint(new Point("ppp",80.0,70.0));
    w2->addPoint(new Point("ppp",90.0,30.0));
    InterfaceController::finalizeShapeCreation(w2);
}

void InterfaceController::createWireframe(std::string name, std::list<Coord<double>*> coords) {
    // int counter = 0;
    // Point *p;
    // Wireframe *wireframe = new Wireframe(name);

    // auto coord = coords.begin();
    // while (coord != coords.end()) {
    //     counter++;
    //     const std::string p_name = name + "_p" + std::to_string(counter);
    //     p = new Point(p_name, (*coord)->getX(), (*coord)->getY());
    //     wireframe->addPoint(p);
    //     delete *coord;
    //     coord = coords.erase(coord);
    // }
    // std::cout << "About to create wireframe... " << std::endl;
    // InterfaceController::finalizeShapeCreation(wireframe);

    Wireframe *w2 = new Wireframe("w2");
    w2->addPoint(new Point("ppp",10.0,10.0));
    w2->addPoint(new Point("ppp",80.0,70.0));
    w2->addPoint(new Point("ppp",90.0,30.0));
    InterfaceController::finalizeShapeCreation(w2);
}

void InterfaceController::finalizeShapeCreation(Shape *shape) {
    _canvas->addToDisplayFile(shape);
    _shapes.push_back(shape);
    _interface->appendObjectToViewList(shape);
    this->update(shape);
}

void InterfaceController::update(Shape *shape) {
    shape->clipToWindow(&_window);
    this->toViewport(shape);
    _canvas->invalidateCanvas();
}

void InterfaceController::toViewport(Shape *shape) {
    std::cout << "Converting to viewport coordinates." << std::endl;

    const std::list<Coord<double>*>* wCoords = shape->getWindowCoordinates();
    shape->clearViewportCoordinates();

    int vpX, vpY;
    auto c = wCoords->begin();
    while (c != wCoords->end()) {
        vpX = this->xWindowToViewport((*c)->getX());
        vpY = this->yWindowToViewport((*c)->getY());
        shape->addViewportCoordinate(new Coord<int>(vpX, vpY));
        c++;
        std::cout << "vpX = " << vpX << std::endl;
        std::cout << "vpY = " << vpY << std::endl;
    }
}

int InterfaceController::xWindowToViewport(const double xWindow) {
    double xViewport;
    xViewport = xWindow - _window.getXMin();
    xViewport *= (_xViewportMax - _xViewportMin)/(_window.getXMax() - _window.getXMin());
    xViewport += _xViewportMin;
    return (int) xViewport;
}

int InterfaceController::yWindowToViewport(const double yWindow) {
    double yViewport;
    yViewport = (_yViewportMax - _yViewportMin) / (_window.getYMax() - _window.getYMin());
    yViewport *= (yWindow - _window.getYMin());
    yViewport = _yViewportMax - yViewport;
    return (int) yViewport;
}

void InterfaceController::moveWindow(int moveX, int moveY) {
    _window.moveWindow(moveX, moveY);
    this->updateShapes();
}

void InterfaceController::zoomWindow(int inOrOut) {
    _window.zoomWindow(inOrOut);
    this->updateShapes();
}

void InterfaceController::updateShapes() {
    auto shape = _shapes.begin();
    while (shape != _shapes.end()) {
        this->update((*shape));
        shape++;
    }
}
