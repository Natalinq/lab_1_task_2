#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "quadrilateral.h"

class Rectangle : public Quadrilateral {
private:
    double width;
    double height;

    void updateDimensions();

public:
    Rectangle(double x, double y, double w, double h);
    Rectangle(const QPointF &topLeft, const QPointF &bottomRight);

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    void setWidth(double w);
    void setHeight(double h);

    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;

    double area() const override { return width * height; }
    double perimeter() const override { return 2.0 * (width + height); }

    std::vector<QPointF> getCorners() const;
    bool isSquare(double tolerance = 1e-6) const { return std::abs(width - height) < tolerance; }
};

#endif // RECTANGLE_H
