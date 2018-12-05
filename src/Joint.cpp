#include <Joint.h>

#include <KinoMath.h>
#include <Landmark.h>

Joint::Joint(const std::string &name) :
    GenericBody (name),
    _angle(0),
    _origin(new Landmark()),
    _begin(new Landmark()),
    _end(new Landmark())
{

}

Joint::Joint(const Landmark& origin,
             const Landmark& begin,
             const Landmark& end,
             const std::string& name) :
    GenericBody (name),
    _angle(0),
    _origin(new Landmark(origin)),
    _begin(new Landmark(begin)),
    _end(new Landmark(end))
{

}

const Landmark &Joint::GetOrigin() const
{
    return *_origin;
}

const Landmark &Joint::GetBegin() const
{
    return *_begin;
}

const Landmark &Joint::GetEnd() const
{
    return *_end;
}

void Joint::SetOrigin(const Landmark &origin)
{
    *_origin = origin;
}

void Joint::SetBegin(const Landmark &begin)
{
    *_begin = begin;
}

void Joint::SetEnd(const Landmark &end)
{
    *_end = end;
}

double Joint::GetAngleDegree() const
{
    return KinoMath::toDegree(GetAngle());
}

double Joint::GetAngle() const
{
    return _angle;
}

void Joint::SetAngle(double angle)
{
    _angle = angle;
}
