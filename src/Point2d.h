#ifndef POINT_2D_H
#define POINT_2D_H

class Point2d
{
public:
    Point2d(double x = 0, double y = 0);

    double GetX() const;
    void SetX(double x);

    double GetY() const;
    void SetY(double y);

protected:
    double _x;
    double _y;

};
Point2d operator+(const Point2d& a, const Point2d& b);
Point2d operator-(const Point2d& a, const Point2d& b);

#endif // POINT_2D_H
