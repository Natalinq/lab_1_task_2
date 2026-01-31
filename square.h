#ifndef SQUARE_H
#define SQUARE_H
#include "quadrilateral.h"


class Square : public Quadrilateral {
private:
    double side;
    void updateSide();

public:

    Square(double x, double y, double s);

    Square(const QPointF &topLeft, const QPointF &bottomRight);

    double getSide() const { return side; }

    void setSide(double s);

    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;

    double area() const override { return side * side; }
    double perimeter() const override { return 4.0 * side; }

    double getDiagonal() const { return side * std::sqrt(2.0); }

    bool hasRightAngles(double tolerance = 1e-6) const;
};

#endif // SQUARE_H
