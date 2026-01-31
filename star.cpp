#include "star.h"
#include <cmath>
#include <stdexcept>
#include <vector>


std::vector<QPointF> Star::generateStarVertices(double x, double y, int points,
                                                double outerRadius, double innerRadius)
{
    if (points < 3) {
        throw std::invalid_argument("Звезда должна иметь минимум 3 конца. Передано: " + std::to_string(points));
    }

    if (outerRadius <= 0.0) {
        throw std::invalid_argument("Внешний радиус должен быть положительным. Передано: " + std::to_string(outerRadius));
    }

    if (innerRadius <= 0.0 || innerRadius >= outerRadius) {
        throw std::invalid_argument(
            "Внутренний радиус должен быть в диапазоне (0, внешний радиус). "
            "Внешний: " + std::to_string(outerRadius) + ", внутренний: " + std::to_string(innerRadius)
            );
    }

    std::vector<QPointF> verts;
    verts.reserve(2 * points);

    double angleStep = 2.0 * M_PI / (2 * points);

    for (int i = 0; i < 2 * points; ++i) {
        double angle = -M_PI_2 + i * angleStep;

        double radius = (i % 2 == 0) ? outerRadius : innerRadius;
        double vx = x + radius * std::cos(angle);
        double vy = y + radius * std::sin(angle);

        verts.emplace_back(vx, vy);
    }

    return verts;
}

Star::Star(double x, double y, int p, double outerR, double innerR)
    : Polygon(generateStarVertices(x, y, p, outerR, innerR)),
    points(p),
    outerRadius(outerR),
    innerRadius(innerR)
{
}

double Star::calculateGeometricArea() const {
    if (vertexCount() < 3) return 0.0;

    double totalArea = 0.0;
    size_t n = vertexCount();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;

        const QPointF &v0 = QPointF(centerX, centerY);
        const QPointF &v1 = vertex(i);
        const QPointF &v2 = vertex(j);

        double area = 0.5 * std::abs(
                          (v1.x() - v0.x()) * (v2.y() - v0.y()) -
                          (v2.x() - v0.x()) * (v1.y() - v0.y())
                          );

        totalArea += area;
    }

    return totalArea;
}

double Star::perimeter() const {
    if (vertexCount() < 3) return 0.0;

    double perimeter = 0.0;
    size_t n = vertexCount();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        const QPointF &v1 = vertex(i);
        const QPointF &v2 = vertex(j);

        double dx = v2.x() - v1.x();
        double dy = v2.y() - v1.y();
        perimeter += std::sqrt(dx * dx + dy * dy);
    }

    return perimeter;
}

Star5::Star5(double x, double y, double outerR, double innerR)
    : Star(x, y, 5, outerR, innerR)
{
}

std::vector<QPointF> Star6::generateStar6Vertices(double x, double y, double outerRadius)
{
    if (outerRadius <= 0.0) {
        throw std::invalid_argument("Радиус звезды Давида должен быть положительным");
    }

    std::vector<QPointF> verts;
    verts.reserve(12);

    double angles1[3] = { -M_PI_2, -M_PI_2 + 2.0 * M_PI / 3.0, -M_PI_2 + 4.0 * M_PI / 3.0 };
    double angles2[3] = { -M_PI_2 + M_PI / 3.0, -M_PI_2 + M_PI, -M_PI_2 + 5.0 * M_PI / 3.0 };

    for (int i = 0; i < 6; ++i) {
        double angle = (i % 2 == 0) ? angles1[i/2] : angles2[i/2];
        double vx = x + outerRadius * std::cos(angle);
        double vy = y + outerRadius * std::sin(angle);
        verts.emplace_back(vx, vy);

        double innerAngle = angle + M_PI / 6.0;
        double ivx = x + (outerRadius * 0.5) * std::cos(innerAngle);
        double ivy = y + (outerRadius * 0.5) * std::sin(innerAngle);
        verts.emplace_back(ivx, ivy);
    }

    return verts;
}

Star6::Star6(double x, double y, double outerR)
    : Star(x, y, 6, outerR, outerR * 0.5)
{
    std::vector<QPointF> verts = generateStar6Vertices(x, y, outerR);
    setVertices(verts);

    outerRadius = outerR;
    innerRadius = outerR * 0.5;
}

Star8::Star8(double x, double y, double outerR, double innerR)
    : Star(x, y, 8, outerR, innerR)
{
}
