#include "Frame.h"

Frame::Frame() :
    _t(-1)
{

}

double Frame::GetTime() const
{
    return _t;
}

void Frame::SetTime(double t)
{
    _t = t;
}

const Point2d &Frame::GetPoint() const
{
    return _point;
}

void Frame::SetPoint(const Point2d &point)
{
    _point = point;
}

void Frame::SetPoint(double x, double y)
{
    _point = Point2d(x, y);
}
