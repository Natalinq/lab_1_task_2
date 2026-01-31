#include "heart.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>
#include <QPainterPath>
#include <QBrush>
#include <QColor>

std::vector<QPointF> Heart::generateHeartVertices(double x, double y, double s, int res) {
    if (res < 3) {
        throw std::invalid_argument("Разрешение сердца должно быть >= 3. Передано: " + std::to_string(res));
    }

    if (s <= 0.0) {
        throw std::invalid_argument("Размер сердца должен быть положительным. Передано: " + std::to_string(s));
    }

    std::vector<QPointF> verts;
    verts.reserve(res);

    const double scale = s / 17.0;

    for (int i = 0; i < res; ++i) {
        double t = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(res);

        double sin_t = std::sin(t);
        double cos_t = std::cos(t);

        double x_param = 16.0 * sin_t * sin_t * sin_t;
        double y_param = 13.0 * cos_t
                         - 5.0 * std::cos(2.0 * t)
                         - 2.0 * std::cos(3.0 * t)
                         - std::cos(4.0 * t);

        double vx = x + x_param * scale;
        double vy = y - y_param * scale;

        verts.emplace_back(vx, vy);
    }

    return verts;
}

Heart::Heart(double x, double y, double s, int res)
    : Shape(x, y), size(s), resolution(res)
{
    if (s <= 0.0) {
        throw std::invalid_argument("Размер сердца должен быть положительным. Передано: " + std::to_string(s));
    }

    if (res < 3) {
        throw std::invalid_argument("Разрешение сердца должно быть >= 3. Передано: " + std::to_string(res));
    }

    vertices = generateHeartVertices(x, y, s, res);
}

double Heart::calculateArea() const {
    if (vertices.size() < 3) return 0.0;

    double totalArea = 0.0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;

        const QPointF &v0 = QPointF(centerX, centerY);
        const QPointF &v1 = vertices[i];
        const QPointF &v2 = vertices[j];

        double area = 0.5 * std::abs(
                          (v1.x() - v0.x()) * (v2.y() - v0.y()) -
                          (v2.x() - v0.x()) * (v1.y() - v0.y())
                          );

        totalArea += area;
    }

    return totalArea;
}

double Heart::calculatePerimeter() const {
    if (vertices.size() < 2) return 0.0;

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

void Heart::move(double dx, double dy) {
    centerX += dx;
    centerY += dy;

    for (QPointF &v : vertices) {
        v.rx() += dx;
        v.ry() += dy;
    }
}

void Heart::rotate(double angleDeg, double originX, double originY) {
    if (std::abs(angleDeg) < 1e-6) return;

    QPointF newCenter = rotatePoint(QPointF(centerX, centerY), angleDeg, originX, originY);
    centerX = newCenter.x();
    centerY = newCenter.y();

    for (QPointF &v : vertices) {
        v = rotatePoint(v, angleDeg, originX, originY);
    }
}

void Heart::scale(double factor, double originX, double originY) {
    if (std::abs(factor - 1.0) < 1e-6) return;

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

    size *= factor;
}

void Heart::draw(QPainter &painter) const {
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;

    if (!vertices.empty()) {
        path.moveTo(vertices[0]);
        for (size_t i = 1; i < vertices.size(); ++i) {
            path.lineTo(vertices[i]);
        }
        path.closeSubpath();
    }

    painter.fillPath(path, QBrush(QColor(255, 105, 180, 200)));

    painter.setPen(QPen(QColor(220, 20, 60), 2));
    painter.drawPath(path);

    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPointF(centerX, centerY), 4, 4);
}

void Heart::setSize(double s) {
    if (s <= 0.0) {
        throw std::invalid_argument("Размер сердца должен быть положительным. Передано: " + std::to_string(s));
    }

    double factor = s / size;
    scale(factor, centerX, centerY);
}

void Heart::setResolution(int res) {
    if (res < 3) {
        throw std::invalid_argument("Разрешение сердца должно быть >= 3. Передано: " + std::to_string(res));
    }

    resolution = res;
    vertices = generateHeartVertices(centerX, centerY, size, res);
}
