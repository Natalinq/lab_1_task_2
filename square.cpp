#include "square.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>

Square::Square(double x, double y, double s)
    : Quadrilateral(
          // Вершины квадрата строятся симметрично относительно центра (против часовой стрелки):
          QPointF(x - s / 2.0, y - s / 2.0),  // v0: верхний левый
          QPointF(x + s / 2.0, y - s / 2.0),  // v1: верхний правый
          QPointF(x + s / 2.0, y + s / 2.0),  // v2: нижний правый
          QPointF(x - s / 2.0, y + s / 2.0)   // v3: нижний левый
          ),
    side(s)
{
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона квадрата должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

#ifdef QT_DEBUG
    // Проверка инвариантов при создании
    bool allSidesEqual = true;
    double s0 = Quadrilateral::getSideLength(0);
    for (int i = 1; i < 4; ++i) {
        double si = Quadrilateral::getSideLength(i);
        if (std::abs(s0 - si) > 1e-6) {
            allSidesEqual = false;
            break;
        }
    }

    bool allAngles90 = true;
    for (int i = 0; i < 4; ++i) {
        if (std::abs(getAngle(i) - 90.0) > 1e-6) {
            allAngles90 = false;
            break;
        }
    }

    if (!allSidesEqual || !allAngles90) {
        qDebug() << "Предупреждение: инварианты квадрата нарушены при создании!";
        qDebug() << "  Все стороны равны:" << allSidesEqual;
        qDebug() << "  Все углы 90°:" << allAngles90;
    }
#endif
}

Square::Square(const QPointF &topLeft, const QPointF &bottomRight)
    : Quadrilateral(
          topLeft,                                      // v0: верхний левый
          QPointF(bottomRight.x(), topLeft.y()),        // v1: верхний правый
          bottomRight,                                  // v2: нижний правый
          QPointF(topLeft.x(), bottomRight.y())         // v3: нижний левый
          )
{
    // Проверяем, что это действительно квадрат (ширина == высота)
    double width = Quadrilateral::getSideLength(0);   // Верхняя сторона
    double height = Quadrilateral::getSideLength(1);  // Правая сторона

    if (std::abs(width - height) > 1e-6) {
        throw std::invalid_argument(
            "Точки не образуют квадрат: ширина=" + std::to_string(width) +
            ", высота=" + std::to_string(height)
            );
    }

    if (width <= 0.0) {
        throw std::invalid_argument("Размеры квадрата должны быть положительными");
    }

    side = width;

    // Центр масс должен совпадать с центром прямоугольника из точек
    // (автоматически вычислен в конструкторе Quadrilateral)
}

void Square::updateSide() {
    // Пересчитываем сторону как среднее всех 4 сторон
    double s0 = Quadrilateral::getSideLength(0);
    double s1 = Quadrilateral::getSideLength(1);
    double s2 = Quadrilateral::getSideLength(2);
    double s3 = Quadrilateral::getSideLength(3);
    side = (s0 + s1 + s2 + s3) / 4.0;
}

void Square::setSide(double s) {
    if (s <= 0.0) {
        throw std::invalid_argument(
            "Сторона квадрата должна быть положительной. Передано: " +
            std::to_string(s)
            );
    }

    // Масштабируем относительно центра с коэффициентом = новая_сторона / старая_сторона
    double factor = s / side;
    scale(factor, centerX, centerY);
    // updateSide() вызван внутри scale()
}

void Square::move(double dx, double dy) {
    Quadrilateral::move(dx, dy);
    // Сторона не меняется при перемещении
}

void Square::rotate(double angleDeg, double originX, double originY) {
    Quadrilateral::rotate(angleDeg, originX, originY);
    updateSide();  // Пересчитываем сторону после поворота

    // Инварианты сохраняются:
    // - Все стороны остаются равными (поворот сохраняет расстояния)
    // - Все углы остаются 90° (поворот сохраняет углы)
}

void Square::scale(double factor, double originX, double originY) {
    if (factor <= 0.0) {
        throw std::invalid_argument(
            "Коэффициент масштабирования должен быть положительным. "
            "Передано: " + std::to_string(factor)
            );
    }

    Quadrilateral::scale(factor, originX, originY);
    side *= factor;
    // Инварианты сохраняются:
    // - Все стороны масштабируются одинаково → остаются равными
    // - Углы не меняются при масштабировании → остаются 90°
}

bool Square::hasRightAngles(double tolerance) const {
    for (int i = 0; i < 4; ++i) {
        if (std::abs(getAngle(i) - 90.0) > tolerance) {
            return false;
        }
    }
    return true;
}
