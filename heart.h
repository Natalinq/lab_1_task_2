#ifndef HEART_H
#define HEART_H

#include "shape.h"
#include <vector>


class Heart : public Shape {
private:
    double size;
    int resolution;
    std::vector<QPointF> vertices;

    static std::vector<QPointF> generateHeartVertices(double x, double y, double s, int res);

    double calculateArea() const;

    double calculatePerimeter() const;

public:

    Heart(double x = 0.0, double y = 0.0, double s = 50.0, int res = 50);

    double area() const override { return calculateArea(); }
    double perimeter() const override { return calculatePerimeter(); }

    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;
    void draw(QPainter &painter) const override;

    double getSize() const { return size; }
    void setSize(double s);

    int getResolution() const { return resolution; }
    void setResolution(int res);

    const std::vector<QPointF>& getVertices() const { return vertices; }
};

#endif // HEART_H
