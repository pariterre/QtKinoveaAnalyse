#include "Joint.h"

Joint::Joint(const Landmark& origin, const Landmark& begin, const Landmark& end, const std::string& name) :
    GenericBody (name),
    _origin(origin),
    _begin(begin),
    _end(end)
{

}

const Landmark &Joint::GetOrigin() const
{
    return _origin;
}

const Landmark &Joint::GetBegin() const
{
    return _begin;
}

const Landmark &Joint::GetEnd() const
{
    return _end;
}

void Joint::SetOrigin(const Landmark &origin)
{
    _origin = origin;
}

void Joint::SetBegin(const Landmark &begin)
{
    _begin = begin;
}

void Joint::SetEnd(const Landmark &end)
{
    _end = end;
}
