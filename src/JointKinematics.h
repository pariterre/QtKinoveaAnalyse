#ifndef JOINT_KINEMATICS_H
#define JOINT_KINEMATICS_H

#include "KinoveaReader.h"
#include "ProportionalModel.h"
#include "Point2d.h"

class JointKinematics
{
public:
    JointKinematics(const ProportionalModel&, const KinoveaReader&);

protected:
};

#endif // JOINT_KINEMATICS_H
