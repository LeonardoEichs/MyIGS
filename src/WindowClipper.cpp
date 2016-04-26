// WindowClipper.cpp
// Authors: Leonardo Vailatti
//          Makhles R. Lange

#include <iostream>
#include "WindowClipper.hpp"
#include "Coord.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Wireframe.hpp"
#include "BezierCurve.hpp"


WindowClipper::WindowClipper() :
    m_lineClipping(LineClipping::CS),
    m_polygonClipping(PolygonClipping::SH)
{   
}


void WindowClipper::clip_to_area(Point &p) {
    double x = p.xnc();
    double y = p.ync();
    DCoordVector newVertice;

    if (x >= X_MIN && x <= X_MAX && y >= Y_MIN && y <= Y_MAX) {
        std::cout << "Clipped point inside (" << x << "," << y << ")" << std::endl;
        newVertice.push_back(new Coord<double>(x, y));
    } else {
        std::cout << "Point outside." << std::endl;
    }
    p.update_normalized_coords(newVertice);
}


void WindowClipper::clip_to_area(Line &line) {
    switch(m_lineClipping) {
        case LineClipping::CS: this->cohen_sutherland_clipping(line); break;
        case LineClipping::LB: this->liang_barsky_clipping(line); break;
        case LineClipping::NLN: this->nicholl_lee_nicholl_clipping(line); break;
    }
}


void WindowClipper::clip_to_area(Wireframe &wf) {
    switch(m_polygonClipping) {
        case PolygonClipping::SH: this->SH_clipping(wf); break;
        case PolygonClipping::WA: this->weiler_atherton_clipping(wf); break;
    }
}


void WindowClipper::clip_to_area(BezierCurve &bc) {
    const DCoordVector norm_coords = bc.normalized_coords();
    DCoordVector outVertices;

    bool previous_inside = false;    // Whether the last point was inside the window
    const int steps = 100;
    const double dt = 1.0 / (double) steps;

    double x0, x1, x2, x3;  // Bézier control points
    double y0, y1, y2, y3;  // Bézier control points
    double b0, b1, b2, b3;  // Bernstein basis polynomials
    double aux1, aux2, t2;  // Auxiliary variables
    double x, y;            // Current point coordinates
    Coord<double> current;
    Coord<double> previous;
    Coord<double> *new_coord;  // Intersection at an edge

    std::cout << "Clipping Bézier curve..." << std::endl;

    // Loop over the control points
    for (unsigned i = 0; i < norm_coords.size(); i+=4) {
        std::cout << "  First curve:" << std::endl;

        x0 = norm_coords[i+0]->x();
        x1 = norm_coords[i+1]->x();
        x2 = norm_coords[i+2]->x();
        x3 = norm_coords[i+3]->x();
        y0 = norm_coords[i+0]->y();
        y1 = norm_coords[i+1]->y();
        y2 = norm_coords[i+2]->y();
        y3 = norm_coords[i+3]->y();

        std::cout << "  > Bernstein points read." << std::endl;

        for (double t = 0.0; t < 1.0; t+=dt) {
            aux1 = 1.0 - t;
            aux2 = aux1 * aux1;
            t2 = t * t;
            b0 = aux1 * aux2;
            b1 = 3.0 * t * aux2;
            b2 = 3.0 * t2 * aux1;
            b3 = t * t2;
            x = (x0 * b0) + (x1 * b1) + (x2 * b2) + (x3 * b3);
            y = (y0 * b0) + (y1 * b1) + (y2 * b2) + (y3 * b3);

             std::cout << "  > t = " << t << std::endl;
             std::cout << "  > x = " << x << std::endl;
             std::cout << "  > y = " << y << std::endl;

            if (coord_inside(x, y)) {
                std::cout << "  > Current point is inside. " << std::endl;
                if (t == 0.0) {
                    // First point doesn't have a previous point, so we set it to true.
                    previous_inside = true;
                }
                if (previous_inside) {
                    // Both current and previous points are inside the clipping window.
                    std::cout << "  > Previous point was inside. " << std::endl;
                    new_coord = new Coord<double>(x, y);
                    outVertices.push_back(new_coord);
                } else {
                    // Line is entering the window.
                    std::cout << "  > Previous point was outside. " << std::endl;
                    current.set_x(x);
                    current.set_y(y);
                    new_coord = WindowClipper::SH_intersect(&current, &previous);
                    outVertices.push_back(new_coord);
                    std::cout << "  > New intersection at (" << new_coord->x() << ","
                            << new_coord->y() << ")" << std::endl;
                    new_coord = new Coord<double>(x, y);
                    outVertices.push_back(new_coord);
                }
            } else {
                std::cout << "  > Current point is outside. " << std::endl;
                if (t == 0.0) {
                    // First point doesn't have a previous point, so we set it to false.
                    previous_inside = false;
                }
                if (previous_inside) {
                    // Line is leaving the window
                    std::cout << "  > Previous point was inside. " << std::endl;
                    current.set_x(x);
                    current.set_y(y);
                    new_coord = WindowClipper::SH_intersect(&current, &previous);
                    outVertices.push_back(new_coord);
                    std::cout << "  > New intersection at (" << new_coord->x() << ","
                            << new_coord->y() << ")" << std::endl;
                } else {
                    std::cout << "  > Previous point was outside. " << std::endl;
                }
            }
            previous.set_x(x);
            previous.set_y(y);
        }
    }

    bc.update_normalized_coords(outVertices);
}


bool WindowClipper::coord_inside(double x, double y) {
    bool inside = false;
    // If the point is outside the clipping window, then
    // set which edge is closer to it in counterclockwise fashion.
    if (x >= X_MIN) {
        if (y <= Y_MAX) {
            if (x <= X_MAX) {
                if (y >= Y_MIN) {
                    inside = true;
                } else {
                    m_edge = WindowClipper::Boundary::TOP_EDGE;
                }
            } else {
                m_edge = WindowClipper::Boundary::RIGHT_EDGE;
            }
        } else {
            m_edge = WindowClipper::Boundary::BOTTOM_EDGE;    
        }
    } else {
        m_edge = WindowClipper::Boundary::LEFT_EDGE;
    }
    return inside;
}



void WindowClipper::set_line_clipping_method(LineClipping type) {
    m_lineClipping = type;
}


void WindowClipper::set_polygon_clipping_method(PolygonClipping type) {
    m_polygonClipping = type;
}


void WindowClipper::cohen_sutherland_clipping(Line &line) {
    int outcode1 = computeOutcode(line.p1()->xnc(), line.p1()->ync());
    int outcode2 = computeOutcode(line.p2()->xnc(), line.p2()->ync());
    bool accept = false;
    double x1 = line.p1()->xnc();
    double x2 = line.p2()->xnc();
    double y1 = line.p1()->ync();
    double y2 = line.p2()->ync();

    while(true) {
        if (!(outcode1 | outcode2)) {
            accept = true;
            break;
        } else if (outcode1 & outcode2) {
            break;
        } else {
            double x, y;

            int outcodeOut = outcode1 ? outcode1 : outcode2;

            if ( outcodeOut & TOP) {
                x = x1 + (x2 - x1) * (Y_MAX - y1) / (y2 - y1);
                y = Y_MAX;
            } else if (outcodeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (Y_MIN - y2) / (y2 - y1);
                y = Y_MIN;
            } else if (outcodeOut & RIGHT) {
                y = y1 + (y2 - y1) * (X_MAX - x1) / (x2 - x1);
                x = X_MAX;
            } else if(outcodeOut & LEFT) {
                y = y1 + (y2 - y1) * (X_MIN - x1) / (x2 - x1);
                x = X_MIN;
            }

            if (outcodeOut == outcode1) {
                x1 = x;
                y1 = y;
                outcode1 = computeOutcode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                outcode2 = computeOutcode(x2, y2);
            }
        }
    }

    if (accept) {
        std::cout << "X1: " << x1 << std::endl;
        std::cout << "Y1: " << y1 << std::endl;
        std::cout << "X2: " << x2 << std::endl;
        std::cout << "Y2: " << y2 << std::endl;
    }

    DCoordVector newVertices;
    newVertices.push_back(new Coord<double>(x1, y1));
    newVertices.push_back(new Coord<double>(x2, y2));
    line.update_normalized_coords(newVertices);
}


int WindowClipper::computeOutcode(double x, double y) {
    int code;

    code = INSIDE;
    if (x < X_MIN)
        code |= LEFT;
    else if (x > X_MAX)
        code |= RIGHT;
    if (y < Y_MIN)
        code |= BOTTOM;
    else if (y > Y_MAX)
        code |= TOP;

    return code;
}






void WindowClipper::liang_barsky_clipping(Line &line) {
    double t0 = 0.0;
    double t1 = 1.0;
    double xdelta = line.p2()->xnc() - line.p1()->xnc();
    double ydelta = line.p2()->ync() - line.p1()->ync();
    double p, q, r;

    for (int edge = 0; edge < 4; edge++) {
        if (edge == 0) {
            p = -xdelta;
            q = -(X_MIN - line.p1()->xnc());
        }
        if (edge == 1) {
            p = xdelta;
            q = (X_MAX - line.p1()->xnc());
        }
        if (edge == 2) {
            p = -ydelta;
            q = -(Y_MIN - line.p1()->ync());
        }
        if (edge == 3) {
            p = ydelta;
            q = (Y_MAX - line.p1()->ync());
        }

        r = q/p;
        if (p == 0 && q < 0) return;  // Don't draw line at all. (Parallel line outside)

        if (p < 0) {
            if (r > t1) {
                return; // Don't draw line at all
            } else if (r > t0) {
                t0 = r;  // Line is clipped
            }
        } else if (p > 0) {
            if (r < t0) {
                return; // Don't draw line at all
            } else if (r < t1) {
                t1 = r;
            }
        }
    }

    double x1 = line.p1()->xnc() + t0 * xdelta;
    double y1 = line.p1()->ync() + t0 * ydelta;
    double x2 = line.p1()->xnc() + t1 * xdelta;
    double y2 = line.p1()->ync() + t1 * ydelta;

    std::cout << "X1: " << x1 << std::endl;
    std::cout << "Y1: " << y1 << std::endl;
    std::cout << "X2: " << x2 << std::endl;
    std::cout << "Y2: " << y2 << std::endl;

    DCoordVector newVertices;
    newVertices.push_back(new Coord<double>(x1, y1));
    newVertices.push_back(new Coord<double>(x2, y2));
    line.update_normalized_coords(newVertices);
}



void WindowClipper::nicholl_lee_nicholl_clipping(Line &line) {
    // TODO
}


void WindowClipper::SH_clipping(Wireframe &wf) {

    const DCoordVector coords = wf.normalized_coords();
    DCoordVector inVertices(coords);
    DCoordVector outVertices;

    // Call the algorithm for each window edge, switching in and out vectors
    m_edge = WindowClipper::Boundary::LEFT_EDGE;
    std::cout << "Clipping at the LEFT edge..." << std::endl;
    SH_polygon_clipping(inVertices, outVertices);

    inVertices.clear();
    m_edge = WindowClipper::Boundary::BOTTOM_EDGE;
    std::cout << "Clipping at the BOTTOM edge..." << std::endl;
    SH_polygon_clipping(outVertices, inVertices);

    outVertices.clear();
    m_edge = WindowClipper::Boundary::RIGHT_EDGE;
    std::cout << "Clipping at the RIGHT edge..." << std::endl;
    SH_polygon_clipping(inVertices, outVertices);
    
    inVertices.clear();
    m_edge = WindowClipper::Boundary::TOP_EDGE;
    std::cout << "Clipping at the TOP edge..." << std::endl;
    SH_polygon_clipping(outVertices, inVertices);

    // Clipped polygon is stored in inVertices
    wf.update_normalized_coords(inVertices);
}


void WindowClipper::SH_polygon_clipping(const DCoordVector &inVertices, DCoordVector &outVertices) {
    Coord<double> *s;
    Coord<double> *p;
    Coord<double> *i;

    s = inVertices.back();

    for(unsigned j = 0; j < inVertices.size(); j++) {
        p = inVertices[j];
        if (this->SH_inside(p)) {
            if (this->SH_inside(s)) {
                // The whole line is inside the clipping area
                outVertices.push_back(p);
            } else {
                // Line is entering the clipping area
                i = this->SH_intersect(p, s);
                outVertices.push_back(i);
                outVertices.push_back(p);
            }
        } else {
            if (this->SH_inside(s)) {
                // Line is leaving the clipping area
                i = this->SH_intersect(p, s);
                outVertices.push_back(i);
            }
        }
        s = p;
    }
}


bool WindowClipper::SH_inside(Coord<double> *p) {
    bool inside = false;
    switch(m_edge) {
        case WindowClipper::Boundary::LEFT_EDGE:
        {
            inside = ((-1.0 - p->x()) <= 0.0);
            break;
        }
        case WindowClipper::Boundary::BOTTOM_EDGE:
        {
            inside = ((-1.0 - p->y()) <= 0.0);
            break;
        }
        case WindowClipper::Boundary::RIGHT_EDGE:
        {
            inside = ((p->x() - 1.0) <= 0.0);
            break;
        }
        case WindowClipper::Boundary::TOP_EDGE:
        {
            inside = ((p->y() - 1.0) <= 0.0);
            break;
        }
    }
    return inside;
}


Coord<double>* WindowClipper::SH_intersect(Coord<double> *p, Coord<double> *s) {
    double x, y;
    double x0 = s->x();
    double y0 = s->y();
    double x1 = p->x();
    double y1 = p->y();
    double dydx = (y1 - y0) / (x1 - x0);
    double dxdy = 1.0 / dydx;


    switch(m_edge) {
        case WindowClipper::Boundary::LEFT_EDGE:
        {
            x = X_MIN;
            y = y0 - dydx * (x0 - X_MIN);
            break;
        }
        case WindowClipper::Boundary::BOTTOM_EDGE:
        {
            x = x0 - dxdy * (y0 - Y_MIN);
            y = Y_MIN;
            break;
        }
        case WindowClipper::Boundary::RIGHT_EDGE:
        {
            x = X_MAX;
            y = y0 - dydx * (x0 - X_MAX);
            break;
        }
        case WindowClipper::Boundary::TOP_EDGE:
        {
            x = x0 - dxdy * (y0 - Y_MAX);
            y = Y_MAX;
            break;
        }
    }
    return new Coord<double>(x, y);
}


void WindowClipper::weiler_atherton_clipping(Wireframe &wf) {
    // TODO
}
