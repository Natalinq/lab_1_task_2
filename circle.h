#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
private:
    double radius;

public:

    Circle(double x = 0.0, double y = 0.0, double r = 1.0);


    double area() const override;
    double perimeter() const override;
    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;
    void draw(QPainter &painter) const override;


    void setRadius(double r);

    double getRadius() const { return radius; }
};

#endif // CIRCLE_H
