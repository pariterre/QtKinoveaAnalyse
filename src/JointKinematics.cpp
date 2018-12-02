#include "JointKinematics.h"

JointKinematics::JointKinematics()
{

}

void JointKinematics::computeJointAngles(const ProportionalModel &, const KinoveaReader &)
{
    // arccos( dot(P1-O, P2-O) / (norm(P1) * norm(P2)) )


    //    angles = {}
    //    for (n, s) in segments.items():
    //        v1 = data[s[0]] - data[s[2]]
    //        v2 = data[s[1]] - data[s[2]]

    //        dot = np.einsum('ij,ij->j', v1, v2)
    //        denom = linalg.norm(v1, axis=0) * linalg.norm(v2, axis=0)
    //        angles[n] = np.arccos(dot / denom)

    //    return angles
}
