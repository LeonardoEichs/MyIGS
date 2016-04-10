// ShapeDrawer.cpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#include <iostream>
#include "ShapeDrawer.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Wireframe.hpp"
#include "Coord.hpp"

void ShapeDrawer::draw(Point *point) {
    const Coord<int> *p = (point->viewport_coordinates())->front();
    m_cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    m_cr->set_line_width(2.0);
    m_cr->move_to(p->x(), p->y());
    m_cr->line_to(p->x(), p->y());
    m_cr->stroke();
}

void ShapeDrawer::draw(Line *line) {
    m_cr->set_line_cap(Cairo::LINE_CAP_SQUARE);
    m_cr->set_line_width(1.0);

    const std::list<const Coord<int>*> *coordinates = line->viewport_coordinates();
    auto coord = coordinates->cbegin();
    m_cr->move_to((*coord)->x(), (*coord)->y());
    coord++;
    m_cr->line_to((*coord)->x(), (*coord)->y());
    m_cr->stroke();
}

void ShapeDrawer::draw(Wireframe *wireframe) {
    int x, y, x0, y0;

    m_cr->set_line_cap(Cairo::LINE_CAP_SQUARE);
    m_cr->set_line_width(1.0);
    
    const std::list<const Coord<int>*> *coordinates = wireframe->viewport_coordinates();
    auto coord = coordinates->cbegin();
    x = x0 = (*coord)->x();
    y = y0 = (*coord)->y();
    m_cr->move_to(x,y);
    coord++;
    while (coord != coordinates->cend()) {
        x = (*coord)->x();
        y = (*coord)->y();
        std::cout << "line_to -> (" << x << "," << y << ")" << std::endl;
        m_cr->line_to(x,y);
        coord++;
    }
    // Go back to first point
    m_cr->line_to(x0,y0);
    m_cr->stroke();
}