// Scaling.hpp
// Authors: Leonardo Vailatti Eichstaedt
//          Makhles Reuter Lange

#ifndef SCALING_HPP
#define SCALING_HPP

#include "TMatrix.hpp"

class Scaling : public TMatrix {
private:
    double m_dx;  // Scaling along the x axis
    double m_dy;  // Scaling along the y axis

public:
    Scaling(unsigned order, double dx, double dy);
    virtual ~Scaling() {}
    void buildTMatrix(const double ref_x, const double ref_y) override;
};

#endif  // SCALING_HPP