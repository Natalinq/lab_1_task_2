#include "circle.h"
#include <cmath>
#include <stdexcept>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Circle::Circle(double x, double y, double r)
    : Shape(x, y), radius(r)
{
    if (radius <= 0.0) {
        throw std::invalid_argument(
            "Радиус круга должен быть положительным. Передано: " +
            std::to_string(radius)
            );
    }
}

double Circle::area() const {
    return M_PI * radius * radius;
}

double Circle::perimeter() const {
    return 2.0 * M_PI * radius;
}

void Circle::move(double dx, double dy) {
    centerX += dx;
    centerY += dy;
}

void Circle::rotate(double angleDeg, double originX, double originY) {
    QPointF newCenter = rotatePoint(QPointF(centerX, centerY), angleDeg, originX, originY);
    centerX = newCenter.x();
    centerY = newCenter.y();
}

void Circle::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    radius *= factor;

    QPointF newCenter = scalePoint(QPointF(centerX, centerY), factor, originX, originY);
    centerX = newCenter.x();
    centerY = newCenter.y();
}

void Circle::draw(QPainter &painter) const {
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::blue, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPointF(centerX, centerY), radius, radius);

    painter.setPen(QPen(Qt::red, 4));
    painter.drawPoint(QPointF(centerX, centerY));
}

void Circle::setRadius(double r) {
    if (r <= 0.0) {
        throw std::invalid_argument(
            "Радиус круга должен быть положительным. Передано: " +
            std::to_string(r)
            );
    }
    radius = r;
}
