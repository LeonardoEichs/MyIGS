// Writer.hpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#ifndef WRITER_HPP
#define WRITER_HPP

// Forward declarations
class Point;
class Line;
class Wireframe;
class Curve2D;

class Writer {
public:
    virtual ~Writer() {}
    virtual void write_to_file(Point &p) = 0;
    virtual void write_to_file(Line &l) = 0;
    virtual void write_to_file(Wireframe &w) = 0;
    virtual void write_to_file(Curve2D &curve) = 0;
    virtual void write_to_file() = 0;
};

#endif  // WRITER_HPP