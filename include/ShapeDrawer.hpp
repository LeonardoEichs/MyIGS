// ShapeDrawer.hpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#ifndef SHAPE_DRAWER_H
#define SHAPE_DRAWER_H

#include <cairomm/context.h>
#include "AbstractDrawer.hpp"

// Forware declaration
template <class T> class Coord;

typedef Cairo::RefPtr<Cairo::Context> CairoCtx;


class ShapeDrawer : public AbstractDrawer {
protected:
    Cairo::RefPtr<Cairo::Context> m_cr;

public:
    ShapeDrawer() {}
    ~ShapeDrawer() {}

    // AbstractDrawer functions
    void draw(Point &point) override;
    void draw(Line &line) override;
    void draw(Wireframe &wireframe) override;
    void draw(Curve2D &curve) override;

    // Own functions
    void set_cairo_context(const CairoCtx &cr) { m_cr = cr; }
};

#endif  // SHAPE_DRAWER_H