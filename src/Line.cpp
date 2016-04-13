// Line.cpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#include <iostream>
#include "Line.hpp"
#include "Point.hpp"
#include "Coord.hpp"
#include "Window.hpp"
#include "AbstractDrawer.hpp"
#include "TMatrix.hpp"

Line::Line(const std::string name, Point *p1, Point *p2) :
        Shape(name, ShapeType::LINE),
        m_p1(p1),
        m_p2(p2) {
}


Line::~Line() {
    auto coord = m_ncList.begin();
    while (coord != m_ncList.end()) {
        delete *coord;
        coord++;
    }
    m_ncList.clear();
}


const Coord<double> Line::get_centroid() {
    double x = (m_p1->xwc() + m_p2->xwc()) / 2.0;
    double y = (m_p1->ywc() + m_p2->ywc()) / 2.0;
    return Coord<double>(x, y);
}


// Visitor
void Line::accept(AbstractDrawer *drawer) {
    drawer->draw(this);
}


void Line::transform(TMatrix &matrix) {
    m_p1->transform(matrix);
    m_p2->transform(matrix);
}


void Line::normalize(TMatrix &matrix) {
    m_p1->normalize(matrix);
    m_p2->normalize(matrix);
}


void Line::clip_to_window(Window &window) {
    std::cout << "Clipping to window." << std::endl;

    /* Temporary implementation */
    Coord<double> *coord;
    m_ncList.clear();
    coord = new Coord<double>(m_p1->xnc(), m_p1->ync());
    m_ncList.push_back(coord);
    coord = new Coord<double>(m_p2->xnc(), m_p2->ync());
    m_ncList.push_back(coord);
    std::cout << "x1nc = " << m_p1->xnc() << std::endl;
    std::cout << "y1nc = " << m_p1->ync() << std::endl;
    std::cout << "x2nc = " << m_p2->xnc() << std::endl;
    std::cout << "y2nc = " << m_p2->ync() << std::endl;
    std::cout << "-----------------------------" << std::endl;
}


void Line::write_to_file(std::ofstream &file) {

}