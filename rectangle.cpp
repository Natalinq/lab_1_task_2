#include "rectangle.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>

Rectangle::Rectangle(double x, double y, double w, double h)
    : Quadrilateral(
          QPointF(x - w / 2.0, y - h / 2.0),
          QPointF(x + w / 2.0, y - h / 2.0),
          QPointF(x + w / 2.0, y + h / 2.0),
          QPointF(x - w / 2.0, y + h / 2.0)
          ),
    width(w),
    height(h)
{
    if (w <= 0.0 || h <= 0.0) {
        throw std::invalid_argument(
            "Ширина и высота прямоугольника должны быть положительными. "
            "Ширина: " + std::to_string(w) + ", высота: " + std::to_string(h)
            );
    }

#ifdef QT_DEBUG
    if (std::abs(centerX - x) > 1e-6 || std::abs(centerY - y) > 1e-6) {
        qDebug() << "Предупреждение: центр масс не совпадает с заданным центром:"
                 << QPointF(centerX, centerY) << "vs" << QPointF(x, y);
    }
#endif
}

Rectangle::Rectangle(const QPointF &topLeft, const QPointF &bottomRight)
    : Quadrilateral(
          topLeft,
          QPointF(bottomRight.x(), topLeft.y()),
          bottomRight,
          QPointF(topLeft.x(), bottomRight.y())
          )
{
    width = getSideLength(0);
    height = getSideLength(1);

    if (width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument(
            "Некорректные координаты углов: ширина=" + std::to_string(width) +
            ", высота=" + std::to_string(height)
            );
    }
}

void Rectangle::updateDimensions() {
    double side0 = getSideLength(0);
    double side1 = getSideLength(1);
    double side2 = getSideLength(2);
    double side3 = getSideLength(3);

    width = (side0 + side2) / 2.0;
    height = (side1 + side3) / 2.0;
}

void Rectangle::setWidth(double w) {
    if (w <= 0.0) {
        throw std::invalid_argument(
            "Ширина прямоугольника должна быть положительной. Передано: " +
            std::to_string(w)
            );
    }

    double factor = w / width;
    double cx = centerX;

    vertices[0].rx() = cx - (cx - vertices[0].x()) * factor;
    vertices[1].rx() = cx + (vertices[1].x() - cx) * factor;
    vertices[2].rx() = cx + (vertices[2].x() - cx) * factor;
    vertices[3].rx() = cx - (cx - vertices[3].x()) * factor;

    width = w;
}

void Rectangle::setHeight(double h) {
    if (h <= 0.0) {
        throw std::invalid_argument(
            "Высота прямоугольника должна быть положительной. Передано: " +
            std::to_string(h)
            );
    }

    double factor = h / height;
    double cy = centerY;

    vertices[0].ry() = cy - (cy - vertices[0].y()) * factor;
    vertices[1].ry() = cy - (cy - vertices[1].y()) * factor;
    vertices[2].ry() = cy + (vertices[2].y() - cy) * factor;
    vertices[3].ry() = cy + (vertices[3].y() - cy) * factor;

    height = h;
}

void Rectangle::move(double dx, double dy) {
    Quadrilateral::move(dx, dy);
}

void Rectangle::rotate(double angleDeg, double originX, double originY) {
    Quadrilateral::rotate(angleDeg, originX, originY);
    updateDimensions();
}

void Rectangle::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    Quadrilateral::scale(factor, originX, originY);
    width *= factor;
    height *= factor;
}

std::vector<QPointF> Rectangle::getCorners() const {
    return {
        vertices[0],
        vertices[1],
        vertices[2],
        vertices[3]
    };
}
