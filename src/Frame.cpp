#include "Frame.h"

Frame::Frame(size_t nSegments) :
    _t(-1)
{
    _points.resize(nSegments);
    _pointIsSet.resize(nSegments);
}

double Frame::GetTime() const
{
    return _t;
}

void Frame::SetTime(double t)
{
    _t = t;
}

const Point2d &Frame::GetPoint2d(const std::string &name) const
{
    for (size_t p = 0; p < _points.size(); ++p){
        if (!_points[p].GetName().compare(name))
            return _points[p];
    }
    throw std::runtime_error("Point not found");
}

const Point2d &Frame::GetPoint2d(size_t segmentIdx) const
{
    return _points.at(segmentIdx);
}

void Frame::SetPointName(size_t segmentIdx, const std::string &name)
{
    _points.at(segmentIdx).SetName(name);
}

void Frame::SetPoint2d(size_t segmentIdx, const Point2d &point)
{
    _points.at(segmentIdx) = point;
    _pointIsSet.at(segmentIdx) = true;
}

void Frame::SetPoint2d(size_t segmentIdx, double x, double y)
{
    _points.at(segmentIdx) = Point2d(x, y);
    _pointIsSet.at(segmentIdx) = true;
}

bool Frame::isAllSegmentsAreSet() const
{
    for (bool isSet : _pointIsSet)
        if (!isSet)
            return false;
    return true;
}
