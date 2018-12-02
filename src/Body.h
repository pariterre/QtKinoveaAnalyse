#ifndef BODY_H
#define BODY_H

#include <vector>
#include <iostream>

class GenericBody
{
public:
    GenericBody(const std::string& name = "");
    const std::string& GetName() const;
    void SetName(const std::string& name);
protected:
    std::string _name;
};

class Landmark : public GenericBody {

};

class Segment : public GenericBody {
public:
    Segment();

    const Landmark& GetProximal() const;
    void SetProximal(const Landmark& proximal);

    const Landmark& GetDistal() const;
    void SetDistal(const Landmark& distal);

    double GetRelativeMass() const;
    void SetRelativeMass(double relativeMass);

    double GetCenterOfMassFromProximal() const;
    void SetCenterOfMassFromProximal(double com);

protected:
    Landmark _proximal;
    Landmark _distal;
    double _relativeMass;
    double _centerOfMassFromProximal;
};

#endif  // BODY_H
