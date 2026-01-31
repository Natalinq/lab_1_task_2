#ifndef HEXAGON_H
#define HEXAGON_H

#include "polygon.h"

class Hexagon : public Polygon {
private:
    double side;

    void updateSide();

public:
    Hexagon(double x, double y, double s);
    Hexagon(double x, double y, double radius, bool useRadius);

    double getSide() const { return side; }
    void setSide(double s);

    double getCircumradius() const { return side; }
    double getInradius() const { return side * std::sqrt(3.0) / 2.0; }

    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;

    double area() const override { return (3.0 * std::sqrt(3.0) / 2.0) * side * side; }
    double perimeter() const override { return 6.0 * side; }

    bool hasEqualSides(double tolerance = 1e-6) const;
    bool hasEqualAngles(double tolerance = 1e-6) const;

    double getVertexAngle(size_t vertexIndex) const;
};

#endif // HEXAGON_H
