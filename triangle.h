#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "polygon.h"

class Triangle : public Polygon {
public:

    Triangle(const QPointF &v1, const QPointF &v2, const QPointF &v3);

    Triangle(double x, double y, double sideLength);

    double getSideA() const;
    double getSideB() const;
    double getSideC() const;
};

#endif // TRIANGLE_H
