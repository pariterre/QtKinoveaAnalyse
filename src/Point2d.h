#ifndef POINT_2D_H
#define POINT_2D_H

#include <math.h>
#include "Body.h"

class Point2d : public GenericBody
{
public:
    Point2d(double x = 0, double y = 0, const std::string& name = "");

    double GetX() const;
    void SetX(double x);

    double GetY() const;
    void SetY(double y);

    Point2d operator+=(const Point2d& a);
    double norm() const;
protected:
    double _x;
    double _y;

};
Point2d operator*(double, const Point2d& a);
double operator*(const Point2d& a, const Point2d& b); // dot product
Point2d operator+(const Point2d& a, const Point2d& b);
Point2d operator-(const Point2d& a, const Point2d& b);


#endif // POINT_2D_H
