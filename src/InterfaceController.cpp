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
#include "TMatrix.hpp"
#include "TMatrixBuilder.hpp"


InterfaceController::InterfaceController(MyIGS *interface, Canvas *canvas) :
    m_interface(interface),
    m_canvas(canvas),
    m_xViewportMin(0),
    m_xViewportMax(500),
    m_yViewportMin(0),
    m_yViewportMax(500)
{
}


InterfaceController::~InterfaceController() {
    auto shape = m_shapes.begin();
    while (shape != m_shapes.end()) {
        delete *shape;
        shape++;
    }
    m_shapes.clear();
}


// Create a new shape based on its type.
void InterfaceController::createShape(ShapeType type) {
    Shape *shape = ShapeBuilder::instance()->createShape(type);
    this->finalizeShapeCreation(shape);
}


void InterfaceController::finalizeShapeCreation(Shape *shape) {
    m_canvas->addToDisplayFile(shape);
    m_shapes.push_back(shape);
    m_interface->appendObject(shape->get_name());
    this->update(shape);
}


void InterfaceController::transform(const std::string &obj) {
    TMatrixBuilder *builder = TMatrixBuilder::instance();
    Shape *shape = this->findShape(obj);
    if (shape) {
        const Coord<double> centroid = shape->getCentroid();
        TMatrix *matrix = builder->buildMatrix(centroid.getX(), centroid.getY());
        shape->transform(matrix);
        this->update(shape);
    } else {
        std::cout << "Couldn't find specified object!" << std::endl;
    }
    builder->reset();
}


Shape* InterfaceController::findShape(const std::string &obj) {
    Shape *shapeToReturn = nullptr;
    auto shape = m_shapes.cbegin();
    while (shape != m_shapes.cend()) {
        if ((*shape)->get_name() == obj) {
            shapeToReturn = *shape;
        }
        shape++;
    }
    return shapeToReturn;
}


void InterfaceController::update(Shape *shape) {
    shape->clipToWindow(&m_window);
    this->toViewport(shape);
    m_canvas->invalidateCanvas();
}


// Called whenever the Window is translated or rotated.
void InterfaceController::updateShapes() {
    auto shape = m_shapes.begin();
    while (shape != m_shapes.end()) {
        this->update((*shape));
        shape++;
    }
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
    xViewport = xWindow - m_window.getXMin();
    xViewport *= (m_xViewportMax - m_xViewportMin)/(m_window.getXMax() - m_window.getXMin());
    xViewport += m_xViewportMin;
    return (int) xViewport;
}


int InterfaceController::yWindowToViewport(const double yWindow) {
    double yViewport;
    yViewport = (m_yViewportMax - m_yViewportMin) / (m_window.getYMax() - m_window.getYMin());
    yViewport *= (yWindow - m_window.getYMin());
    yViewport = m_yViewportMax - yViewport;
    return (int) yViewport;
}


void InterfaceController::moveWindow(int moveX, int moveY) {
    m_window.moveWindow(moveX, moveY);
    this->updateShapes();
}


void InterfaceController::zoomWindow(int inOrOut) {
    m_window.zoomWindow(inOrOut);
    this->updateShapes();
}
