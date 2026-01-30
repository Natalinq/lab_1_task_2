#ifndef SHAPE_H
#define SHAPE_H

#include <QPointF>
#include <QPainter>
#include <vector>
#include <cmath>
#include <stdexcept>


class Shape {
protected:
    double centerX;
    double centerY;

public:

    Shape(double x = 0.0, double y = 0.0) : centerX(x), centerY(y) {}

    virtual ~Shape() = default;


    virtual double area() const = 0;

    virtual double perimeter() const = 0;

    virtual void move(double dx, double dy) = 0;

    virtual void rotate(double angleDeg, double originX, double originY) = 0;

    virtual void scale(double factor, double originX, double originY) = 0;

    virtual void draw(QPainter &painter) const = 0;


    virtual QPointF centerOfMass() const {
        return QPointF(centerX, centerY);
    }

    void setCenterOfMass(double x, double y) {
        double dx = x - centerX;
        double dy = y - centerY;
        move(dx, dy);
    }

    void setCenterOfMass(const QPointF &point) {
        setCenterOfMass(point.x(), point.y());
    }

    double getCenterX() const { return centerX; }
    double getCenterY() const { return centerY; }
};



QPointF rotatePoint(const QPointF &point, double angleDeg, double originX, double originY);

QPointF scalePoint(const QPointF &point, double factor, double originX, double originY);

double distanceBetweenPoints(const QPointF &p1, const QPointF &p2);

#endif // SHAPE_H
