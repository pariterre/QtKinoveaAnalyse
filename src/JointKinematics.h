#ifndef JOINT_KINEMATICS_H
#define JOINT_KINEMATICS_H

#include <math.h>

#include "KinoveaReader.h"
#include "ProportionalModel.h"
#include "Point2d.h"

class JointKinematics
{
public:
    JointKinematics();
    void computeJointAngles(const ProportionalModel&, const KinoveaReader&);
protected:
    std::vector<Frame> _joints;
};

#endif // JOINT_KINEMATICS_H
