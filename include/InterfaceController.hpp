// InterfaceController.hpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#ifndef InterfaceController_H
#define InterfaceController_H

#include <list>
#include <string>
#include "Window.hpp"
#include "ShapeType.hpp"
#include "TMatrix.hpp"

// Forward declarations
class MyIGS;
class Canvas;
class TransformationDialog;
class Shape;
class ShapeDrawer;
template <class T> class Coord;

class InterfaceController {
protected:
    std::list<Shape*> m_shapes;
    MyIGS * const m_interface;
    Canvas * const m_canvas;
    Window m_window;

    int m_xViewportMin; // TODO : change to getter method from Canvas
    int m_xViewportMax;
    int m_yViewportMin;
    int m_yViewportMax;

    TMatrix m_gtm;  // Global Transformation Matrix

public:
    InterfaceController(MyIGS *interface, Canvas *canvas);
    virtual ~InterfaceController();
    void set_canvas(Canvas &canvas);

    void create_shape(ShapeType);
    void finalize_shape(Shape *shape);
    void transform(const std::string &obj);
    Shape* find_shape(const std::string &obj);
    void update(Shape *shape);
    void update_shapes();
    void draw_shapes(ShapeDrawer &drawer);

    // void to_viewport(Shape *shape);
    // int x_wWindowToViewport(const double xWindow);
    // int yWindowToViewport(const double yWindow);

    // Transformation methods
    void move_window(int moveX, int moveY);
    void scale_window(double factor);
    void translate(const TransformationDialog &dialog);
    void rotate_about_centroid(const TransformationDialog &dialog);
    void rotate(const TransformationDialog &dialog);
    void scale(const TransformationDialog &dialog);
};

#endif  // InterfaceController_H