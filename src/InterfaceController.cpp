/* InterfaceController.cpp */

#include <iostream>
#include "InterfaceController.h"
#include "MyIGS.h"
#include "Canvas.h"
#include "Point.h"
#include "Line.h"
#include "Coord.h"

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

void InterfaceController::createPoint(std::string name, const size_t xPos, const size_t yPos) {
    Point *point = new Point(name, xPos, yPos);
    InterfaceController::finalizeShapeCreation(point);
}

void InterfaceController::createLine(std::string name, const size_t x1Pos, const size_t y1Pos,
        const size_t x2Pos, const size_t y2Pos) {

    std::cout << "Creating a new line." << std::endl;

    const std::string p1_name = name + "_p1";
    const std::string p2_name = name + "_p2";
    Point *p1 = new Point(p1_name , x1Pos, y1Pos);
    Point *p2 = new Point(p2_name , x2Pos, y2Pos);
    Line *line = new Line(name, p1, p2);
    InterfaceController::finalizeShapeCreation(line);
}

void InterfaceController::finalizeShapeCreation(Shape *shape) {

    std::cout << "Finalizing shape creation." << std::endl;
    _shapes.push_back(shape);
    _canvas->addToDisplayFile(shape);

    /* Append object name to the list of objects in the view */
    _interface->appendObjectToViewList(shape);

    /* Clip to current window dimensions */
    shape->clipToWindow(&_window);

    /* Compute viewport coordinates */
    this->toViewport(shape);

    /* Force redraw */
    _canvas->invalidateCanvas();
}

void InterfaceController::toViewport(Shape *shape) {
    std::cout << "Converting to viewport coordinates." << std::endl;

    const std::list<Coord<double>*>* wCoords = shape->getWindowCoordinates();
    shape->clearViewportCoordinates();

    size_t vpX, vpY;
    int count = 0;
    auto c = wCoords->begin();
    while (c != wCoords->end()) {
        count++;
        vpX = this->xWindowToViewport((*c)->getX());
        std::cout << "vpX = " << vpX << std::endl;
        std::cout << "" << count << " of " << wCoords->size() << std::endl;
        vpY = this->yWindowToViewport((*c)->getY());
        std::cout << "vpY = " << vpY << std::endl;
        shape->addViewportCoordinate(new Coord<size_t>(vpX, vpY));
        c++;
    }
    std::cout << "Converted." << std::endl;
}

size_t InterfaceController::xWindowToViewport(const double xWindow) {
    double xViewport;
    xViewport = xWindow - _window._xMin;
    std::cout << "xViewport = " << xViewport << std::endl;
    xViewport *= (_xViewportMax - _xViewportMin)/(_window._xMax - _window._xMin);
    std::cout << "xViewport = " << xViewport << std::endl;
    xViewport += _xViewportMin;
    std::cout << "xViewport = " << xViewport << std::endl;
    return (size_t) xViewport;
}

size_t InterfaceController::yWindowToViewport(const double yWindow) {
    double yViewport;
    yViewport = (_yViewportMax - _yViewportMin) / (_window._yMax - _window._yMin);
    std::cout << "yViewport = " << yViewport << std::endl;
    yViewport *= (yWindow - _window._yMin);
    std::cout << "yViewport = " << yViewport << std::endl;
    yViewport = _yViewportMax - yViewport;
    std::cout << "yViewport = " << yViewport << std::endl;
    return (size_t) yViewport;
}

void InterfaceController::translateUp() {
    // TODO
}

void InterfaceController::translateRight() {
    // TODO
}

void InterfaceController::translateDown() {
    // TODO
}

void InterfaceController::translateLeft() {
    // TODO
}

void InterfaceController::zoomWindowIn() {
    // TODO
}

void InterfaceController::zoomWindowOut() {
    // TODO
}