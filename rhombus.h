#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "quadrilateral.h"

class Rhombus : public Quadrilateral {
private:
    double sideLength;
    double acuteAngle;

    void updateParameters();

public:
    Rhombus(double x, double y, double side, double angle = 60.0);
    Rhombus(double x, double y, double diag1, double diag2, bool useDiagonals);

    double getSide() const { return sideLength; }
    double getAcuteAngle() const { return acuteAngle; }
    double getObtuseAngle() const { return 180.0 - acuteAngle; }
    double getDiagonal1() const;
    double getDiagonal2() const;

    void setSide(double side);
    void setAcuteAngle(double angle);

    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;

    double area() const override;
    double perimeter() const override { return 4.0 * sideLength; }

    bool isSquare(double tolerance = 1e-6) const { return std::abs(acuteAngle - 90.0) < tolerance; }
};

#endif // RHOMBUS_H
