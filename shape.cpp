#include "shape.h"
#include <cmath>

QPointF rotatePoint(const QPointF &point, double angleDeg, double originX, double originY) {
    double angleRad = angleDeg * M_PI / 180.0;

    double dx = point.x() - originX;
    double dy = point.y() - originY;

    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    double newX = originX + dx * cosA - dy * sinA;
    double newY = originY + dx * sinA + dy * cosA;

    return QPointF(newX, newY);
}

QPointF scalePoint(const QPointF &point, double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    double dx = point.x() - originX;
    double dy = point.y() - originY;

    double newX = originX + dx * factor;
    double newY = originY + dy * factor;

    return QPointF(newX, newY);
}

double distanceBetweenPoints(const QPointF &p1, const QPointF &p2) {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return std::sqrt(dx * dx + dy * dy);
}
