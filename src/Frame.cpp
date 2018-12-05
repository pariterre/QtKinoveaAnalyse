#include <Frame.h>

#include <Point2d.h>
#include <Joint.h>

Frame::Frame(size_t nSegments, size_t nJoints) :
    _t(-1),
    _grf(new Point2d()),
    _grfIsSet(false)
{
    _points.resize(nSegments);
    _pointIsSet.resize(nSegments);
    _joints.resize(nJoints);
    _jointIsSet.resize(nJoints);
}

double Frame::GetTime() const
{
    return _t;
}

void Frame::SetTime(double t)
{
    _t = t;
}

const Point2d &Frame::GetGrf() const
{
    return *_grf;
}

void Frame::SetGrf(Point2d grf)
{
    *_grf = grf;
    _grfIsSet = true;
}

bool Frame::isGrfSet() const
{
    return _grfIsSet;
}

size_t Frame::GetNumberOfPoints2d() const
{
    return _points.size();
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

size_t Frame::GetNumberOfJoints() const
{
    return _joints.size();
}

void Frame::SetJointName(size_t jointIdx, const std::string &name)
{
    _joints.at(jointIdx).SetName(name);
}

const Joint &Frame::GetJoint(const std::string &name) const
{
    for (size_t j = 0; j < _joints.size(); ++j){
        if (!_joints[j].GetName().compare(name))
            return _joints[j];
    }
    throw std::runtime_error("Joint not found");
}

const Joint &Frame::GetJoint(size_t jointIdx) const
{
    return _joints.at(jointIdx);
}

void Frame::SetJoint(size_t jointIdx, const Joint &joint)
{
    _joints.at(jointIdx) = joint;
    _jointIsSet.at(jointIdx) = true;
}

bool Frame::isAllJointsAreSet() const
{
    for (bool isSet : _jointIsSet)
        if (!isSet)
            return false;
    return true;
}

