#include "rhombus.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>

Rhombus::Rhombus(double x, double y, double side, double angle)
    : Quadrilateral(
          QPointF(x - side * std::cos(angle * M_PI / 360.0), y),                    // v0: левая вершина
          QPointF(x, y - side * std::sin(angle * M_PI / 360.0)),                    // v1: верхняя вершина
          QPointF(x + side * std::cos(angle * M_PI / 360.0), y),                    // v2: правая вершина
          QPointF(x, y + side * std::sin(angle * M_PI / 360.0))                     // v3: нижняя вершина
          ),
    sideLength(side),
    acuteAngle(angle)
{
    if (side <= 0.0) {
        throw std::invalid_argument(
            "Сторона ромба должна быть положительной. Передано: " +
            std::to_string(side)
            );
    }

    if (angle <= 0.0 || angle >= 90.0) {
        throw std::invalid_argument(
            "Острый угол ромба должен быть в диапазоне (0°, 90°). Передано: " +
            std::to_string(angle) + "°"
            );
    }

#ifdef QT_DEBUG
    // Проверка равенства сторон (инвариант ромба)
    double s0 = Quadrilateral::getSideLength(0);  // ← ЯВНЫЙ ВЫЗОВ БАЗОВОГО МЕТОДА
    double s1 = Quadrilateral::getSideLength(1);
    double s2 = Quadrilateral::getSideLength(2);
    double s3 = Quadrilateral::getSideLength(3);
    double maxDiff = std::max({std::abs(s0-s1), std::abs(s1-s2), std::abs(s2-s3), std::abs(s3-s0)});

    if (maxDiff > 1e-6) {
        qDebug() << "Предупреждение: стороны ромба не равны! Макс. разница:" << maxDiff;
    }
#endif
}

Rhombus::Rhombus(double x, double y, double diag1, double diag2, bool useDiagonals)
    : Quadrilateral(
          QPointF(x - diag1 / 2.0, y),      // v0: левая вершина
          QPointF(x, y - diag2 / 2.0),      // v1: верхняя вершина
          QPointF(x + diag1 / 2.0, y),      // v2: правая вершина
          QPointF(x, y + diag2 / 2.0)       // v3: нижняя вершина
          )
{
    Q_UNUSED(useDiagonals);

    if (diag1 <= 0.0 || diag2 <= 0.0) {
        throw std::invalid_argument(
            "Диагонали ромба должны быть положительными. Диагональ1: " +
            std::to_string(diag1) + ", Диагональ2: " + std::to_string(diag2)
            );
    }

    // Вычисляем сторону и острый угол из диагоналей
    sideLength = std::sqrt((diag1 / 2.0) * (diag1 / 2.0) + (diag2 / 2.0) * (diag2 / 2.0));
    acuteAngle = 2.0 * std::atan2(diag2, diag1) * 180.0 / M_PI;

    if (acuteAngle > 90.0) {
        acuteAngle = 180.0 - acuteAngle;
    }
}

void Rhombus::updateParameters() {
    // Используем ЯВНЫЙ ВЫЗОВ базового метода для получения длин сторон
    double s0 = Quadrilateral::getSideLength(0);
    double s1 = Quadrilateral::getSideLength(1);
    double s2 = Quadrilateral::getSideLength(2);
    double s3 = Quadrilateral::getSideLength(3);
    sideLength = (s0 + s1 + s2 + s3) / 4.0;

    double angle0 = getAngle(0);
    double angle1 = getAngle(1);
    double angle2 = getAngle(2);
    double angle3 = getAngle(3);

    acuteAngle = std::min({angle0, angle1, angle2, angle3});

    if (acuteAngle > 90.0) {
        acuteAngle = 180.0 - acuteAngle;
    }
}

double Rhombus::getDiagonal1() const {
    return getDiagonalLength(0);
}

double Rhombus::getDiagonal2() const {
    return getDiagonalLength(1);
}

void Rhombus::setSide(double side) {  // ← ИСПРАВЛЕНО ИМЯ МЕТОДА
    if (side <= 0.0) {
        throw std::invalid_argument(
            "Сторона ромба должна быть положительной. Передано: " +
            std::to_string(side)
            );
    }

    double factor = side / sideLength;
    scale(factor, centerX, centerY);
}

void Rhombus::setAcuteAngle(double angle) {
    if (angle <= 0.0 || angle >= 90.0) {
        throw std::invalid_argument(
            "Острый угол ромба должен быть в диапазоне (0°, 90°). Передано: " +
            std::to_string(angle) + "°"
            );
    }

    double cx = centerX;
    double cy = centerY;
    double currentSide = sideLength;

    vertices[0] = QPointF(cx - currentSide * std::cos(angle * M_PI / 360.0), cy);
    vertices[1] = QPointF(cx, cy - currentSide * std::sin(angle * M_PI / 360.0));
    vertices[2] = QPointF(cx + currentSide * std::cos(angle * M_PI / 360.0), cy);
    vertices[3] = QPointF(cx, cy + currentSide * std::sin(angle * M_PI / 360.0));

    acuteAngle = angle;
}

void Rhombus::move(double dx, double dy) {
    Quadrilateral::move(dx, dy);
}

void Rhombus::rotate(double angleDeg, double originX, double originY) {
    Quadrilateral::rotate(angleDeg, originX, originY);
    updateParameters();
}

void Rhombus::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    Quadrilateral::scale(factor, originX, originY);
    sideLength *= factor;
}

double Rhombus::area() const {
    return (getDiagonal1() * getDiagonal2()) / 2.0;
}
