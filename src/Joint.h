#ifndef JOINT_H
#define JOINT_H

#include "Body.h"

class Joint : public GenericBody
{
public:
    Joint(const Landmark& origin = Landmark(),
          const Landmark& begin = Landmark(),
          const Landmark& end = Landmark(),
          const std::string& name = "");

    const Landmark& GetOrigin() const;
    const Landmark& GetBegin() const;
    const Landmark& GetEnd() const;

    void SetOrigin(const Landmark &origin);
    void SetBegin(const Landmark &begin);
    void SetEnd(const Landmark &end);
protected:
    Landmark _origin;
    Landmark _begin;
    Landmark _end;

};
#endif // JOINT_H
