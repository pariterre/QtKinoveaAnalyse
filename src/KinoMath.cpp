#include "KinoMath.h"

std::vector<Frame> KinoMath::computeCoMi(const ProportionalModel &model, const KinoveaReader &kino)
{
    std::vector<Frame> all_comi;
    for (size_t f = 0; f < kino.GetFrames().size(); ++f){
        Frame comi(model.GetSegments().size(), 0);

        Frame frame(kino.GetFrames()[f]);
        comi.SetTime(frame.GetTime());
        for (size_t s = 0; s < model.GetSegments().size(); ++s){
            Segment segment(model.GetSegments()[s]);
            const Point2d& proximal(frame.GetPoint2d(segment.GetProximal().GetName()));
            const Point2d& distal(frame.GetPoint2d(segment.GetDistal().GetName()));
            Point2d p(proximal + segment.GetCenterOfMassFromProximal() * (distal - proximal));
            p.SetName(segment.GetName());
            comi.SetPoint2d(s, p);
        }
        all_comi.push_back(comi);
    }
    return all_comi;
}

std::vector<Frame> KinoMath::computeCoM(const ProportionalModel &model, const KinoveaReader &kino, std::vector<Frame> &all_comi)
{
    std::vector<Frame> all_com;

    if (all_comi.size() == 0)
        all_comi = computeCoMi(model, kino);

    for (size_t f = 0; f < kino.GetFrames().size(); ++f){
        Frame com(1, 0);

        Frame frame(kino.GetFrames()[f]);
        com.SetTime(frame.GetTime());
        for (size_t s=0; s<model.GetSegments().size(); ++s){
            Point2d tp(model.GetSegments().at(s).GetRelativeMass() * all_comi.at(f).GetPoint2d(s));
            com.SetPoint2d(0, com.GetPoint2d(0) + tp);
        }
        com.SetPointName(0, "CenterOfMass");
        all_com.push_back(com);
    }
    return all_com;
}

std::vector<Frame> KinoMath::computeGrf(const std::vector<Frame> comAcceleration, double masse)
{
    std::vector<Frame> all_grf;
    for (size_t f = 0; f < comAcceleration.size(); ++f){
        Frame grf(0, 0);
        grf.SetTime(comAcceleration[f].GetTime());
        grf.SetGrf(comAcceleration[f].GetPoint2d(0) * masse);
        all_grf.push_back(grf);
    }
    return all_grf;
}

std::vector<Frame> KinoMath::computeJointAngles(const ProportionalModel &model, const KinoveaReader &kino)
{
    std::vector<Frame> all_joints;
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
    all_joints.push_back(joints);
    }
    return all_joints;
}


std::vector<Frame> KinoMath::computeDerivative(const std::vector<Frame> &frames)
{
    std::vector<Frame> newFrames = frames;
    newFrames.pop_back(); // Derivative looses the last element
    for (size_t f = 0; f < newFrames.size(); ++f){
        if (frames[f].isAllSegmentsAreSet()){
            for (size_t p = 0; p < frames[f].GetNumberOfPoints2d(); ++p){
                Point2d point_0 = frames[f + 0].GetPoint2d(p);
                Point2d point_1 = frames[f + 1].GetPoint2d(p); // This is safe thanks to the pop_back
                Point2d num(point_1 - point_0);
                num.SetName(point_0.GetName());
                double denom(frames[f+1].GetTime() - frames[f+0].GetTime());
                newFrames[f].SetPoint2d(p, num / denom);
            }
        }

        if (frames[f].isAllJointsAreSet()){
            for (size_t j = 0; j < frames[f].GetNumberOfJoints(); ++j){
                Joint joint_0 = frames[f + 0].GetJoint(j);
                Joint joint_1 = frames[f + 1].GetJoint(j); // This is safe thanks to the pop_back
                double num(joint_1.GetAngle() - joint_0.GetAngle());
                double denom(frames[f+1].GetTime() - frames[f+0].GetTime());
                joint_0.SetAngle( num / denom );
                newFrames[f].SetJoint(j, joint_0);
            }
        }
    }
    return newFrames;
}

double KinoMath::toDegree(double angleRadian)
{
    return angleRadian * 180 / M_PI;
}

double KinoMath::toRadian(double angleDegree)
{
    return angleDegree / 180 * M_PI;
}
