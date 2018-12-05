#ifndef JOINT_H
#define JOINT_H

#include <memory>
#include <vector>
#include <iostream>

#include <GenericBody.h>
class Landmark;

class Joint : public GenericBody
{
public:
    Joint(const std::string& name = "");
    Joint(const Landmark& origin,
          const Landmark& begin,
          const Landmark& end,
          const std::string& name = "");

    const Landmark& GetOrigin() const;
    const Landmark& GetBegin() const;
    const Landmark& GetEnd() const;

    void SetOrigin(const Landmark &origin);
    void SetBegin(const Landmark &begin);
    void SetEnd(const Landmark &end);

    double GetAngleDegree() const;
    double GetAngle() const;
    void SetAngle(double angle);
protected:
    double _angle;
    std::shared_ptr<Landmark> _origin;
    std::shared_ptr<Landmark> _begin;
    std::shared_ptr<Landmark> _end;

};
#endif // JOINT_H
