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

//        if (f.isAllSegmentsAreSet())
    }

//    def derivative(data, time):
//        new_data = np.ndarray(data.shape)
//        new_data[:] = np.nan
//        new_data[:, :, 1:-1] = (data[:, :, 2:] - data[:, :, 0:-2]) / (time[2:] - time[0:-2])
//        return new_data

}
