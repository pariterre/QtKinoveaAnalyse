#include "Body.h"

GenericBody::GenericBody(const std::string &name) :
    _name(name)
{

}

const std::string &GenericBody::GetName() const
{
    return _name;
}

void  GenericBody::SetName(const std::string &name)
{
    _name = name;
}

Segment::Segment() :
    _proximal(Landmark()),
    _distal(Landmark()),
    _relativeMass(0),
    _centerOfMassFromProximal(0)
{

}

const Landmark &Segment::GetProximal() const
{
    return _proximal;
}

void Segment::SetProximal(const Landmark &proximal)
{
    _proximal = proximal;
}

const Landmark &Segment::GetDistal() const
{
    return _distal;
}

void Segment::SetDistal(const Landmark &distal)
{
    _distal = distal;
}

double Segment::GetRelativeMass() const
{
    return _relativeMass;
}

void Segment::SetRelativeMass(double relativeMass)
{
    _relativeMass = relativeMass;
}

double Segment::GetCenterOfMassFromProximal() const
{
    return _centerOfMassFromProximal;
}

void Segment::SetCenterOfMassFromProximal(double com)
{
    _centerOfMassFromProximal = com;
}
