#ifndef SQUARE_H
#define SQUARE_H

#include "quadrilateral.h"

/**
 * @brief Класс квадрата — четырёхугольник с равными сторонами и прямыми углами
 *
 * ИНВАРИАНТЫ (гарантируются при всех операциях):
 *   1. Все четыре стороны равны по длине
 *   2. Все четыре угла строго равны 90°
 *   3. Диагонали равны, перпендикулярны и делятся пополам в центре масс
 *
 * ⚠️ КРИТИЧЕСКИ ВАЖНО: Квадрат НЕ наследуется от Rectangle!
 *    Причина: у Rectangle есть методы setWidth()/setHeight(), которые позволяют
 *    независимо менять стороны. Вызов этих методов на квадрате нарушит инвариант
 *    "все стороны равны". Это нарушает принцип подстановки Лискова (LSP).
 *
 * Правильная иерархия:
 *   Shape
 *    └── Polygon
 *         └── Quadrilateral
 *              ├── Rectangle  ← прямоугольник (противоположные стороны равны, углы 90°)
 *              ├── Rhombus    ← ромб (все стороны равны, углы ≠ 90° в общем случае)
 *              └── Square     ← квадрат (все стороны равны + все углы 90°)
 *
 * Квадрат — это НЕ частный случай прямоугольника в ООП-смысле, а независимая фигура,
 * которая одновременно удовлетворяет свойствам и прямоугольника, и ромба.
 */
class Square : public Quadrilateral {
private:
    double side;  ///< Длина стороны квадрата (все стороны равны)

    /**
     * @brief Обновление длины стороны после преобразований
     *
     * Пересчитывает сторону как среднее всех 4 сторон для устойчивости к погрешностям.
     * Вызывается после поворота и масштабирования.
     */
    void updateSide();

public:
    /**
     * @brief Конструктор по центру и длине стороны
     * @param x Координата X центра масс
     * @param y Координата Y центра масс
     * @param s Длина стороны (> 0)
     * @throw std::invalid_argument Если сторона <= 0
     *
     * Вершины строятся симметрично относительно центра:
     *   v0: верхний левый
     *   v1: верхний правый
     *   v2: нижний правый
     *   v3: нижний левый
     */
    Square(double x, double y, double s);

    /**
     * @brief Конструктор по двум противоположным углам
     * @param topLeft Верхний левый угол
     * @param bottomRight Нижний правый угол
     * @throw std::invalid_argument Если точки не образуют квадрат
     */
    Square(const QPointF &topLeft, const QPointF &bottomRight);

    // Геттер стороны (единый для всех сторон)
    double getSide() const { return side; }

    // Сеттер стороны (ЕДИНСТВЕННЫЙ метод изменения размера!)
    /**
     * @brief Установка новой длины стороны с сохранением инвариантов
     * @param s Новая длина стороны (> 0)
     * @throw std::invalid_argument Если сторона <= 0
     *
     * ⚠️ ЗАПРЕЩЕНЫ методы setWidth()/setHeight() — они нарушат инвариант квадрата!
     *    Чтобы явно запретить их использование, можно объявить как удалённые:
     *      void setWidth(double) = delete;
     *      void setHeight(double) = delete;
     */
    void setSide(double s);

    // Переопределение методов преобразований с сохранением инвариантов
    void move(double dx, double dy) override;
    void rotate(double angleDeg, double originX, double originY) override;
    void scale(double factor, double originX, double originY) override;

    // Реализация методов из Shape
    double area() const override { return side * side; }
    double perimeter() const override { return 4.0 * side; }

    // Дополнительные методы
    /**
     * @brief Получение длины диагонали квадрата
     * @return Диагональ = сторона × √2
     */
    double getDiagonal() const { return side * std::sqrt(2.0); }

    /**
     * @brief Проверка, что все углы равны 90° (для отладки)
     * @param tolerance Допуск для сравнения (по умолчанию 1e-6)
     * @return true если все углы ≈ 90°
     */
    bool hasRightAngles(double tolerance = 1e-6) const;
};

#endif // SQUARE_H
