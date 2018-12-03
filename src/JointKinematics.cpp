#include "JointKinematics.h"

JointKinematics::JointKinematics()
{

}

void JointKinematics::computeJointAngles(const ProportionalModel &model, const KinoveaReader &kino)
{
    _joints.clear();
    for (size_t f = 0; f < kino.GetFrames().size(); ++f){
        Frame frame(kino.GetFrames()[f]);
        Frame joints(0, model.GetJoints().size());
        joints.SetTime(frame.GetTime());
        for (size_t j = 0; j < model.GetJoints().size(); ++j){
            Joint joint(model.GetJoints()[j]);
            Point2d origin(frame.GetPoint2d(joint.GetOrigin().GetName()));
            Point2d p1(frame.GetPoint2d(joint.GetBegin().GetName()));
            Point2d p2(frame.GetPoint2d(joint.GetEnd().GetName()));

            Point2d p1_orig(p1 - origin);
            Point2d p2_orig(p2 - origin);
            joint.SetAngle( acos( (p1_orig * p2_orig) / (p1_orig.norm() * p2_orig.norm()) ) );
            joints.SetJoint(j, joint);
        }
    _joints.push_back(joints);
    }
}
