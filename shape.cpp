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



// === ПРИМЕР ИСПОЛЬЗОВАНИЯ В НАСЛЕДНИКАХ ===

/*
 * Пример реализации для класса Circle (в circle.cpp):
 *
 * void Circle::move(double dx, double dy) {
 *     // Сдвигаем центр масс
 *     centerX += dx;
 *     centerY += dy;
 *     // Радиус не меняется при перемещении
 * }
 *
 * void Circle::rotate(double angleDeg, double originX, double originY) {
 *     // Круг симметричен — поворот не меняет форму, только положение центра
 *     QPointF newCenter = rotatePoint(QPointF(centerX, centerY), angleDeg, originX, originY);
 *     centerX = newCenter.x();
 *     centerY = newCenter.y();
 * }
 *
 * void Circle::scale(double factor, double originX, double originY) {
 *     // Масштабируем радиус и центр
 *     radius *= factor;
 *     QPointF newCenter = scalePoint(QPointF(centerX, centerY), factor, originX, originY);
 *     centerX = newCenter.x();
 *     centerY = newCenter.y();
 * }
 *
 * void Circle::draw(QPainter &painter) const {
 *     painter.drawEllipse(QPointF(centerX, centerY), radius, radius);
 * }
 */

/*
 * Пример реализации для класса Polygon (в polygon.cpp):
 *
 * void Polygon::move(double dx, double dy) {
 *     // Сдвигаем центр масс
 *     centerX += dx;
 *     centerY += dy;
 *
 *     // Сдвигаем ВСЕ вершины многоугольника
 *     for (QPointF &vertex : vertices) {
 *         vertex.rx() += dx;  // rx() возвращает неконстантную ссылку на x
 *         vertex.ry() += dy;  // ry() возвращает неконстантную ссылку на y
 *     }
 * }
 *
 * void Polygon::rotate(double angleDeg, double originX, double originY) {
 *     // Поворачиваем центр масс
 *     QPointF newCenter = rotatePoint(QPointF(centerX, centerY), angleDeg, originX, originY);
 *     centerX = newCenter.x();
 *     centerY = newCenter.y();
 *
 *     // Поворачиваем ВСЕ вершины
 *     for (QPointF &vertex : vertices) {
 *         vertex = rotatePoint(vertex, angleDeg, originX, originY);
 *     }
 * }
 *
 * void Polygon::scale(double factor, double originX, double originY) {
 *     // Масштабируем центр масс
 *     QPointF newCenter = scalePoint(QPointF(centerX, centerY), factor, originX, originY);
 *     centerX = newCenter.x();
 *     centerY = newCenter.y();
 *
 *     // Масштабируем ВСЕ вершины
 *     for (QPointF &vertex : vertices) {
 *         vertex = scalePoint(vertex, factor, originX, originY);
 *     }
 * }
 *
 * void Polygon::draw(QPainter &painter) const {
 *     painter.drawPolygon(vertices.data(), static_cast<int>(vertices.size()));
 * }
 */
