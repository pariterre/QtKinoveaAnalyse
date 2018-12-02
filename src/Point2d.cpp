#include "Point2d.h"

Point2d::Point2d(double x, double y) :
    _x(x),
    _y(y)
{

}

double Point2d::GetX() const
{
    return _x;
}

void Point2d::SetX(double x)
{
    _x = x;
}

double Point2d::GetY() const
{
    return _y;
}

void Point2d::SetY(double y)
{
    _y =  y;
}

Point2d operator+(const Point2d &a, const Point2d &b)
{
    return Point2d(a.GetX() + b.GetX(), a.GetY() + b.GetY());
}

Point2d operator-(const Point2d &a, const Point2d &b)
{
    return Point2d(a.GetX() - b.GetX(), a.GetY() - b.GetY());
}


Point2d operator*(double val, const Point2d &a)
{
    return Point2d(val * a.GetX(), val * a.GetY());
}

Point2d Point2d::operator+=(const Point2d &a)
{
    this->_x += a._x;
    this->_y += a._y;
    return *this;
}


