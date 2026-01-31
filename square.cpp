#include "square.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>

Square::Square(double x, double y, double s)
    : Quadrilateral(
          QPointF(x - s / 2.0, y - s / 2.0),
          QPointF(x + s / 2.0, y - s / 2.0),
          QPointF(x + s / 2.0, y + s / 2.0),
          QPointF(x - s / 2.0, y + s / 2.0)
          ),
    side(s)
{
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона квадрата должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

#ifdef QT_DEBUG
    bool allSidesEqual = true;
    double s0 = Quadrilateral::getSideLength(0);
    for (int i = 1; i < 4; ++i) {
        double si = Quadrilateral::getSideLength(i);
        if (std::abs(s0 - si) > 1e-6) {
            allSidesEqual = false;
            break;
        }
    }

    bool allAngles90 = true;
    for (int i = 0; i < 4; ++i) {
        if (std::abs(getAngle(i) - 90.0) > 1e-6) {
            allAngles90 = false;
            break;
        }
    }

    if (!allSidesEqual || !allAngles90) {
        qDebug() << "Предупреждение: инварианты квадрата нарушены при создании!";
        qDebug() << "  Все стороны равны:" << allSidesEqual;
        qDebug() << "  Все углы 90°:" << allAngles90;
    }
#endif
}

Square::Square(const QPointF &topLeft, const QPointF &bottomRight)
    : Quadrilateral(
          topLeft,
          QPointF(bottomRight.x(), topLeft.y()),
          bottomRight,
          QPointF(topLeft.x(), bottomRight.y())
          )
{
    double width = Quadrilateral::getSideLength(0);
    double height = Quadrilateral::getSideLength(1);

    if (std::abs(width - height) > 1e-6) {
        throw std::invalid_argument(
            "Точки не образуют квадрат: ширина=" + std::to_string(width) +
            ", высота=" + std::to_string(height)
            );
    }

    if (width <= 0.0) {
        throw std::invalid_argument("Размеры квадрата должны быть положительными");
    }

    side = width;
}

void Square::updateSide() {
    double s0 = Quadrilateral::getSideLength(0);
    double s1 = Quadrilateral::getSideLength(1);
    double s2 = Quadrilateral::getSideLength(2);
    double s3 = Quadrilateral::getSideLength(3);
    side = (s0 + s1 + s2 + s3) / 4.0;
}

void Square::setSide(double s) {
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона квадрата должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

    double factor = s / side;
    scale(factor, centerX, centerY);
}

void Square::move(double dx, double dy) {
    Quadrilateral::move(dx, dy);
}

void Square::rotate(double angleDeg, double originX, double originY) {
    Quadrilateral::rotate(angleDeg, originX, originY);
    updateSide();
}

void Square::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    Quadrilateral::scale(factor, originX, originY);
    side *= factor;
}

bool Square::hasRightAngles(double tolerance) const {
    for (int i = 0; i < 4; ++i) {
        if (std::abs(getAngle(i) - 90.0) > tolerance) {
            return false;
        }
    }
    return true;
}
