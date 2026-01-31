#ifndef STAR_H
#define STAR_H

#include "polygon.h"

class Star : public Polygon {
protected:
    int points;
    double outerRadius;
    double innerRadius;

    static std::vector<QPointF> generateStarVertices(double x, double y, int points,
                                                     double outerRadius, double innerRadius);

    double calculateGeometricArea() const;

public:
    Star(double x, double y, int p, double outerR, double innerR);

    int getPointsCount() const { return points; }
    double getOuterRadius() const { return outerRadius; }
    double getInnerRadius() const { return innerRadius; }

    double area() const override { return calculateGeometricArea(); }
    double perimeter() const override;

    void move(double dx, double dy) override { Polygon::move(dx, dy); }
    void rotate(double angleDeg, double originX, double originY) override { Polygon::rotate(angleDeg, originX, originY); }
    void scale(double factor, double originX, double originY) override { Polygon::scale(factor, originX, originY); }
};

class Star5 : public Star {
public:
    Star5(double x, double y, double outerR, double innerR = 0.38196601125);
};

class Star6 : public Star {
private:
    static std::vector<QPointF> generateStar6Vertices(double x, double y, double outerRadius);

public:
    Star6(double x, double y, double outerR);
};

class Star8 : public Star {
public:
    Star8(double x, double y, double outerR, double innerR = 0.41421356237);
};

#endif // STAR_H
