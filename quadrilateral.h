#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "polygon.h"


class Quadrilateral : public Polygon {
public:

    Quadrilateral(const QPointF &v1, const QPointF &v2, const QPointF &v3, const QPointF &v4);

    Quadrilateral(double x, double y, double width, double height, double angleDeg = 0.0);

    double getSideLength(int sideIndex) const;

    double getDiagonalLength(int diagonalIndex) const;

    double getAngle(int vertexIndex) const;

    bool isConvex() const;

    bool isParallelogram() const;

    bool areSidesPerpendicular(int sideIndex1, int sideIndex2) const;
};

#endif // QUADRILATERAL_H
