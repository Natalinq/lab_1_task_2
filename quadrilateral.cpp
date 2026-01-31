#include "quadrilateral.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>


static std::vector<QPointF> generateParallelogramVertices(double width, double height, double angleDeg) {
    if (width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument(
            "Ширина и высота должны быть положительными. Ширина: " +
            std::to_string(width) + ", высота: " + std::to_string(height)
            );
    }

    std::vector<QPointF> verts = {
        QPointF(-width / 2.0,  height / 2.0),
        QPointF( width / 2.0,  height / 2.0),
        QPointF( width / 2.0, -height / 2.0),
        QPointF(-width / 2.0, -height / 2.0)
    };

    if (std::abs(angleDeg) > 1e-6) {
        double angleRad = angleDeg * M_PI / 180.0;
        double cosA = std::cos(angleRad);
        double sinA = std::sin(angleRad);

        for (QPointF &v : verts) {
            double newX = v.x() * cosA - v.y() * sinA;
            double newY = v.x() * sinA + v.y() * cosA;
            v = QPointF(newX, newY);
        }
    }

    return verts;
}

Quadrilateral::Quadrilateral(double x, double y, double width, double height, double angleDeg)
    : Polygon(generateParallelogramVertices(width, height, angleDeg))
{
    double dx = x - centerX;
    double dy = y - centerY;
    move(dx, dy);
}

Quadrilateral::Quadrilateral(const QPointF &v1, const QPointF &v2, const QPointF &v3, const QPointF &v4)
    : Polygon({v1, v2, v3, v4})
{
    if (vertexCount() != 4) {
        throw std::logic_error("Quadrilateral должен иметь ровно 4 вершины");
    }
}

double Quadrilateral::getSideLength(int sideIndex) const {
    if (sideIndex < 0 || sideIndex > 3) {
        throw std::out_of_range(
            "Индекс стороны должен быть в диапазоне [0, 3]. Передано: " +
            std::to_string(sideIndex)
            );
    }

    int v1 = sideIndex;
    int v2 = (sideIndex + 1) % 4;

    const QPointF &p1 = vertex(v1);
    const QPointF &p2 = vertex(v2);

    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return std::sqrt(dx * dx + dy * dy);
}

double Quadrilateral::getDiagonalLength(int diagonalIndex) const {
    if (diagonalIndex < 0 || diagonalIndex > 1) {
        throw std::out_of_range(
            "Индекс диагонали должен быть 0 или 1. Передано: " +
            std::to_string(diagonalIndex)
            );
    }

    int v1 = (diagonalIndex == 0) ? 0 : 1;
    int v2 = (diagonalIndex == 0) ? 2 : 3;

    const QPointF &p1 = vertex(v1);
    const QPointF &p2 = vertex(v2);

    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return std::sqrt(dx * dx + dy * dy);
}

double Quadrilateral::getAngle(int vertexIndex) const {
    if (vertexIndex < 0 || vertexIndex > 3) {
        throw std::out_of_range(
            "Индекс вершины должен быть в диапазоне [0, 3]. Передано: " +
            std::to_string(vertexIndex)
            );
    }

    int prevIdx = (vertexIndex + 3) % 4;
    int nextIdx = (vertexIndex + 1) % 4;

    const QPointF &v0 = vertex(prevIdx);
    const QPointF &v1 = vertex(vertexIndex);
    const QPointF &v2 = vertex(nextIdx);

    QPointF vec1(v0.x() - v1.x(), v0.y() - v1.y());
    QPointF vec2(v2.x() - v1.x(), v2.y() - v1.y());

    double len1 = std::sqrt(vec1.x() * vec1.x() + vec1.y() * vec1.y());
    double len2 = std::sqrt(vec2.x() * vec2.x() + vec2.y() * vec2.y());

    if (len1 < 1e-10 || len2 < 1e-10) {
        return 0.0;
    }

    double dot = vec1.x() * vec2.x() + vec1.y() * vec2.y();
    double cosAngle = dot / (len1 * len2);
    cosAngle = std::max(-1.0, std::min(1.0, cosAngle));

    double angleRad = std::acos(cosAngle);
    double angleDeg = angleRad * 180.0 / M_PI;

    return 180.0 - angleDeg;
}

bool Quadrilateral::isConvex() const {
    if (vertexCount() != 4) return false;

    for (int i = 0; i < 4; ++i) {
        double angle = getAngle(i);
        if (angle >= 180.0 - 1e-6) {
            return false;
        }
    }
    return true;
}

bool Quadrilateral::isParallelogram() const {
    if (vertexCount() != 4) return false;

    double side0 = getSideLength(0);
    double side1 = getSideLength(1);
    double side2 = getSideLength(2);
    double side3 = getSideLength(3);

    const double tolerance = 1e-6;
    bool oppositeSidesEqual =
        (std::abs(side0 - side2) < tolerance) &&
        (std::abs(side1 - side3) < tolerance);

    if (!oppositeSidesEqual) return false;

    const QPointF &v0 = vertex(0);
    const QPointF &v1 = vertex(1);
    const QPointF &v2 = vertex(2);
    const QPointF &v3 = vertex(3);

    QPointF midDiag1((v0.x() + v2.x()) / 2.0, (v0.y() + v2.y()) / 2.0);
    QPointF midDiag2((v1.x() + v3.x()) / 2.0, (v1.y() + v3.y()) / 2.0);

    double dx = midDiag1.x() - midDiag2.x();
    double dy = midDiag1.y() - midDiag2.y();
    double dist = std::sqrt(dx * dx + dy * dy);

    return dist < tolerance;
}

bool Quadrilateral::areSidesPerpendicular(int sideIndex1, int sideIndex2) const {
    if (sideIndex1 < 0 || sideIndex1 > 3 || sideIndex2 < 0 || sideIndex2 > 3) {
        throw std::out_of_range("Индекс стороны должен быть в диапазоне [0, 3]");
    }

    int v1_start = sideIndex1;
    int v1_end = (sideIndex1 + 1) % 4;
    QPointF vec1(vertex(v1_end).x() - vertex(v1_start).x(),
                 vertex(v1_end).y() - vertex(v1_start).y());

    int v2_start = sideIndex2;
    int v2_end = (sideIndex2 + 1) % 4;
    QPointF vec2(vertex(v2_end).x() - vertex(v2_start).x(),
                 vertex(v2_end).y() - vertex(v2_start).y());

    double dot = vec1.x() * vec2.x() + vec1.y() * vec2.y();
    double len1 = std::sqrt(vec1.x() * vec1.x() + vec1.y() * vec1.y());
    double len2 = std::sqrt(vec2.x() * vec2.x() + vec2.y() * vec2.y());

    if (len1 < 1e-10 || len2 < 1e-10) return false;

    double cosAngle = std::abs(dot) / (len1 * len2);
    return cosAngle < 0.0175;
}
