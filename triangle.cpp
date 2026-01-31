#include "triangle.h"
#include <cmath>
#include <stdexcept>

Triangle::Triangle(const QPointF &v1, const QPointF &v2, const QPointF &v3)
    : Polygon({v1, v2, v3})
{
}

Triangle::Triangle(double x, double y, double sideLength)
    : Polygon({

          QPointF(0.0, sideLength * std::sqrt(3.0) / 3.0),
          QPointF(-sideLength / 2.0, -sideLength * std::sqrt(3.0) / 6.0),
          QPointF(sideLength / 2.0, -sideLength * std::sqrt(3.0) / 6.0)
      })
{
    if (sideLength <= 0.0) {
        throw std::invalid_argument(
            "Сторона треугольника должна быть положительной. Передано: " +
            std::to_string(sideLength)
            );
    }

    move(x - centerX, y - centerY);
}

double Triangle::getSideA() const {
    if (vertexCount() < 3) return 0.0;
    const QPointF &v1 = vertex(0);
    const QPointF &v2 = vertex(1);
    double dx = v2.x() - v1.x();
    double dy = v2.y() - v1.y();
    return std::sqrt(dx * dx + dy * dy);
}

double Triangle::getSideB() const {
    if (vertexCount() < 3) return 0.0;
    const QPointF &v1 = vertex(1);
    const QPointF &v2 = vertex(2);
    double dx = v2.x() - v1.x();
    double dy = v2.y() - v1.y();
    return std::sqrt(dx * dx + dy * dy);
}

double Triangle::getSideC() const {
    if (vertexCount() < 3) return 0.0;
    const QPointF &v1 = vertex(2);
    const QPointF &v2 = vertex(0);
    double dx = v2.x() - v1.x();
    double dy = v2.y() - v1.y();
    return std::sqrt(dx * dx + dy * dy);
}
