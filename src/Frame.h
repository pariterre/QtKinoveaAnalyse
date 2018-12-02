#ifndef FRAME_H
#define FRAME_H

#include "Point2d.h"

class Frame
{
public:
    Frame();

    double GetTime() const;
    void SetTime(double t);

    const Point2d& GetPoint() const;
    void SetPoint(const Point2d& point);
    void SetPoint(double x, double y);
protected:
    double _t;
    Point2d _point;
};

#endif  // FRAME_H
