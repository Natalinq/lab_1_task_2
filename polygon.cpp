#include "polygon.h"
#include <cmath>
#include <numeric>
#include <QDebug>

QPointF Polygon::calculateCenterOfMass() const {
    if (vertices.size() < 3) {
        return QPointF(0.0, 0.0);
    }

    double totalArea = 0.0;
    double weightedCenterX = 0.0;
    double weightedCenterY = 0.0;

    const QPointF &anchor = vertices[0];

    for (size_t i = 1; i < vertices.size() - 1; ++i) {
        const QPointF &v1 = anchor;
        const QPointF &v2 = vertices[i];
        const QPointF &v3 = vertices[i + 1];

        double area = 0.5 * std::abs(
                          (v2.x() - v1.x()) * (v3.y() - v1.y()) -
                          (v3.x() - v1.x()) * (v2.y() - v1.y())
                          );

        if (area < 1e-10) {
            continue;
        }


        double triCenterX = (v1.x() + v2.x() + v3.x()) / 3.0;
        double triCenterY = (v1.y() + v2.y() + v3.y()) / 3.0;

        weightedCenterX += area * triCenterX;
        weightedCenterY += area * triCenterY;
        totalArea += area;
    }

    if (totalArea > 1e-10) {
        weightedCenterX /= totalArea;
        weightedCenterY /= totalArea;
        return QPointF(weightedCenterX, weightedCenterY);
    }

    double avgX = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                  [](double sum, const QPointF &v) { return sum + v.x(); }) / vertices.size();
    double avgY = std::accumulate(vertices.begin(), vertices.end(), 0.0,
                                  [](double sum, const QPointF &v) { return sum + v.y(); }) / vertices.size();

    return QPointF(avgX, avgY);
}

double Polygon::calculateArea() const {
    if (vertices.size() < 3) return 0.0;

    double area = 0.0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        area += vertices[i].x() * vertices[j].y();
        area -= vertices[j].x() * vertices[i].y();
    }

    return std::abs(area) * 0.5;
}

double Polygon::calculatePerimeter() const {
    if (vertices.size() < 3) return 0.0;

    double perimeter = 0.0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        double dx = vertices[j].x() - vertices[i].x();
        double dy = vertices[j].y() - vertices[i].y();
        perimeter += std::sqrt(dx * dx + dy * dy);
    }

    return perimeter;
}

Polygon::Polygon(const std::vector<QPointF> &verts)
    : Shape(0.0, 0.0), vertices(verts)
{
    if (vertices.size() < 3) {
        throw std::invalid_argument(
            "Многоугольник должен иметь минимум 3 вершины. Передано: " +
            std::to_string(vertices.size())
            );
    }

    QPointF cm = calculateCenterOfMass();
    centerX = cm.x();
    centerY = cm.y();
}

Polygon::Polygon(double x, double y, const std::vector<QPointF> &verts)
    : Shape(x, y), vertices(verts)
{
    if (vertices.size() < 3) {
        throw std::invalid_argument(
            "Многоугольник должен иметь минимум 3 вершины. Передано: " +
            std::to_string(vertices.size())
            );
    }

    QPointF realCM = calculateCenterOfMass();

    double dx = x - realCM.x();
    double dy = y - realCM.y();

    for (QPointF &v : vertices) {
        v.rx() += dx;
        v.ry() += dy;
    }

    centerX = x;
    centerY = y;
}

void Polygon::move(double dx, double dy) {

    centerX += dx;
    centerY += dy;

    for (QPointF &v : vertices) {
        v.rx() += dx;
        v.ry() += dy;
    }
}

void Polygon::rotate(double angleDeg, double originX, double originY) {

    QPointF newCenter = rotatePoint(QPointF(centerX, centerY), angleDeg, originX, originY);
    centerX = newCenter.x();
    centerY = newCenter.y();

    for (QPointF &v : vertices) {
        v = rotatePoint(v, angleDeg, originX, originY);
    }
}

void Polygon::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    QPointF newCenter = scalePoint(QPointF(centerX, centerY), factor, originX, originY);
    centerX = newCenter.x();
    centerY = newCenter.y();

    for (QPointF &v : vertices) {
        v = scalePoint(v, factor, originX, originY);
    }
}

void Polygon::draw(QPainter &painter) const {
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::darkGreen, 2));
    painter.setBrush(QColor(144, 238, 144, 100));

    if (vertices.size() >= 3) {
        QPolygonF polygon;
        for (const QPointF &v : vertices) {
            polygon << v;
        }
        painter.drawPolygon(polygon);
    }

    painter.setPen(QPen(Qt::red, 3));
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPointF(centerX, centerY), 5, 5);

    painter.drawLine(QPointF(centerX - 8, centerY), QPointF(centerX + 8, centerY));
    painter.drawLine(QPointF(centerX, centerY - 8), QPointF(centerX, centerY + 8));

    painter.setPen(QPen(Qt::blue, 2));
    painter.setBrush(Qt::blue);
    for (const QPointF &v : vertices) {
        painter.drawEllipse(v, 4, 4);
    }
}

const QPointF& Polygon::vertex(size_t index) const {
    if (index >= vertices.size()) {
        throw std::out_of_range(
            "Индекс вершины выходит за границы. Запрошено: " +
            std::to_string(index) + ", максимум: " + std::to_string(vertices.size() - 1)
            );
    }
    return vertices[index];
}

void Polygon::setVertex(size_t index, const QPointF &point) {
    if (index >= vertices.size()) {
        throw std::out_of_range(
            "Индекс вершины выходит за границы. Запрошено: " +
            std::to_string(index) + ", максимум: " + std::to_string(vertices.size() - 1)
            );
    }
    vertices[index] = point;

    QPointF cm = calculateCenterOfMass();
    centerX = cm.x();
    centerY = cm.y();
}

void Polygon::addVertex(const QPointF &point) {
    vertices.push_back(point);

    QPointF cm = calculateCenterOfMass();
    centerX = cm.x();
    centerY = cm.y();
}

void Polygon::removeVertex(size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range(
            "Индекс вершины выходит за границы. Запрошено: " +
            std::to_string(index) + ", максимум: " + std::to_string(vertices.size() - 1)
            );
    }
    if (vertices.size() <= 3) {
        throw std::invalid_argument("Нельзя удалить вершину — многоугольник должен иметь минимум 3 вершины");
    }
    vertices.erase(vertices.begin() + static_cast<std::ptrdiff_t>(index));

    QPointF cm = calculateCenterOfMass();
    centerX = cm.x();
    centerY = cm.y();
}

void Polygon::setVertices(const std::vector<QPointF> &newVertices) {
    if (newVertices.size() < 3) {
        throw std::invalid_argument(
            "Многоугольник должен иметь минимум 3 вершины. Передано: " +
            std::to_string(newVertices.size())
            );
    }
    vertices = newVertices;

    QPointF cm = calculateCenterOfMass();
    centerX = cm.x();
    centerY = cm.y();
}
