#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <iostream>
#include <vector>

class Point2d;
class Joint;

class Frame
{
public:
    Frame(size_t nSegments, size_t nJoints);

    double GetTime() const;
    void SetTime(double t);

    const Point2d& GetGrf() const;
    void SetGrf(Point2d grf);
    bool isGrfSet() const;

    size_t GetNumberOfPoints2d() const;
    void SetPointName(size_t segmentIdx, const std::string& name);
    const Point2d& GetPoint2d(const std::string& name) const;
    const Point2d& GetPoint2d(size_t segmentIdx) const;
    void SetPoint2d(size_t segmentIdx, const Point2d& point);
    void SetPoint2d(size_t segmentIdx, double x, double y);
    bool isAllSegmentsAreSet() const;

    size_t GetNumberOfJoints() const;
    void SetJointName(size_t jointIdx, const std::string& name);
    const Joint& GetJoint(const std::string& name) const;
    const Joint& GetJoint(size_t jointIdx) const;
    void SetJoint(size_t jointIdx, const Joint& joint);
    bool isAllJointsAreSet() const;
protected:
    double _t;

    std::shared_ptr<Point2d> _grf; // Ground reaction forces
    bool _grfIsSet;

    std::vector<Point2d> _points;
    std::vector<bool> _pointIsSet;

    std::vector<Joint> _joints;
    std::vector<bool> _jointIsSet;
};

#endif  // FRAME_H
