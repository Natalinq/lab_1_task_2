#include "hexagon.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>

Hexagon::Hexagon(double x, double y, double s)
    : Polygon({
          QPointF(x + s, y),
          QPointF(x + s * 0.5, y - s * std::sqrt(3.0) * 0.5),
          QPointF(x - s * 0.5, y - s * std::sqrt(3.0) * 0.5),
          QPointF(x - s, y),
          QPointF(x - s * 0.5, y + s * std::sqrt(3.0) * 0.5),
          QPointF(x + s * 0.5, y + s * std::sqrt(3.0) * 0.5)
      }),
    side(s)
{
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона шестиугольника должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

#ifdef QT_DEBUG
    bool allSidesEqual = hasEqualSides();
    bool allAngles120 = hasEqualAngles();

    if (!allSidesEqual || !allAngles120) {
        qDebug() << "Предупреждение: инварианты шестиугольника нарушены при создании!";
        qDebug() << "  Все стороны равны:" << allSidesEqual;
        qDebug() << "  Все углы 120°:" << allAngles120;
    }
#endif
}

Hexagon::Hexagon(double x, double y, double radius, bool useRadius)
    : Polygon({
          QPointF(x + radius, y),
          QPointF(x + radius * 0.5, y - radius * std::sqrt(3.0) * 0.5),
          QPointF(x - radius * 0.5, y - radius * std::sqrt(3.0) * 0.5),
          QPointF(x - radius, y),
          QPointF(x - radius * 0.5, y + radius * std::sqrt(3.0) * 0.5),
          QPointF(x + radius * 0.5, y + radius * std::sqrt(3.0) * 0.5)
      })
{
    Q_UNUSED(useRadius);

    if (radius <= 0.0) {
        throw std::invalid_argument(
            "Радиус описанной окружности должен быть положительным. Передано: " +
            std::to_string(radius)
            );
    }

    side = radius;
}

void Hexagon::updateSide() {
    double sum = 0.0;
    size_t n = vertexCount();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        const QPointF &v1 = vertex(i);
        const QPointF &v2 = vertex(j);

        double dx = v2.x() - v1.x();
        double dy = v2.y() - v1.y();
        double length = std::sqrt(dx * dx + dy * dy);

        sum += length;
    }

    side = sum / n;
}

void Hexagon::setSide(double s) {
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона шестиугольника должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

    double factor = s / side;
    scale(factor, centerX, centerY);
}

void Hexagon::move(double dx, double dy) {
    Polygon::move(dx, dy);
}

void Hexagon::rotate(double angleDeg, double originX, double originY) {
    Polygon::rotate(angleDeg, originX, originY);
    updateSide();
}

void Hexagon::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    Polygon::scale(factor, originX, originY);
    side *= factor;
}

double Hexagon::getVertexAngle(size_t vertexIndex) const {
    size_t n = vertexCount();
    if (n < 3 || vertexIndex >= n) return 0.0;

    size_t prevIdx = (vertexIndex + n - 1) % n;
    size_t nextIdx = (vertexIndex + 1) % n;

    const QPointF &v0 = vertex(prevIdx);
    const QPointF &v1 = vertex(vertexIndex);
    const QPointF &v2 = vertex(nextIdx);

    QPointF vec1(v0.x() - v1.x(), v0.y() - v1.y());
    QPointF vec2(v2.x() - v1.x(), v2.y() - v1.y());

    double len1 = std::sqrt(vec1.x() * vec1.x() + vec1.y() * vec1.y());
    double len2 = std::sqrt(vec2.x() * vec2.x() + vec2.y() * vec2.y());

    if (len1 < 1e-10 || len2 < 1e-10) return 0.0;

    double dot = vec1.x() * vec2.x() + vec1.y() * vec2.y();

    double cosAngle = dot / (len1 * len2);
    cosAngle = std::max(-1.0, std::min(1.0, cosAngle));

    double angleRad = std::acos(cosAngle);

    double internalAngleDeg = 180.0 - (angleRad * 180.0 / M_PI);

    return internalAngleDeg;
}

bool Hexagon::hasEqualSides(double tolerance) const {
    if (vertexCount() != 6) return false;

    const QPointF &v0 = vertex(0);
    const QPointF &v1 = vertex(1);
    double dx = v1.x() - v0.x();
    double dy = v1.y() - v0.y();
    double side0 = std::sqrt(dx * dx + dy * dy);

    for (size_t i = 1; i < 6; ++i) {
        size_t j = (i + 1) % 6;
        const QPointF &vi = vertex(i);
        const QPointF &vj = vertex(j);

        dx = vj.x() - vi.x();
        dy = vj.y() - vi.y();
        double sideI = std::sqrt(dx * dx + dy * dy);

        if (std::abs(side0 - sideI) > tolerance) {
            return false;
        }
    }

    return true;
}

bool Hexagon::hasEqualAngles(double tolerance) const {
    if (vertexCount() != 6) return false;
    for (size_t i = 0; i < 6; ++i) {
        double angle = getVertexAngle(i);
        if (std::abs(angle - 120.0) > tolerance) {
            return false;
        }
    }

    return true;
}
