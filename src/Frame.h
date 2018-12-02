#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "Point2d.h"

class Frame
{
public:
    Frame(size_t nSegments);

    double GetTime() const;
    void SetTime(double t);

    const Point2d& GetPoint2d(const std::string& name) const;
    const Point2d& GetPoint2d(size_t segmentIdx) const;
    void SetPointName(size_t segmentIdx, const std::string& name);
    void SetPoint2d(size_t segmentIdx, const Point2d& point);
    void SetPoint2d(size_t segmentIdx, double x, double y);

    bool isAllSegmentsAreSet() const;
protected:
    double _t;
    std::vector<Point2d> _points;
    std::vector<bool> _pointIsSet;
};

#endif  // FRAME_H
