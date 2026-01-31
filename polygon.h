#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include <vector>


class Polygon : public Shape {
protected:
    std::vector<QPointF> vertices;

    QPointF calculateCenterOfMass() const;

    double calculateArea() const;

    double calculatePerimeter() const;

public:

    explicit Polygon(const std::vector<QPointF> &verts);

    Polygon(double x, double y, const std::vector<QPointF> &verts);

    double area() const override { return calculateArea(); }
    double perimeter() const override { return calculatePerimeter(); }
    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;
    void draw(QPainter &painter) const override;

    size_t vertexCount() const { return vertices.size(); }
    const QPointF& vertex(size_t index) const;
    const std::vector<QPointF>& getVertices() const { return vertices; }
    void setVertex(size_t index, const QPointF &point);
    void addVertex(const QPointF &point);
    void removeVertex(size_t index);


    void setVertices(const std::vector<QPointF> &newVertices);
};

#endif // POLYGON_H
