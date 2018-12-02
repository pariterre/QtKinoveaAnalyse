#include "CenterOfMass.h"

CenterOfMass::CenterOfMass() :
    _isComComputed(false)
{

}

void CenterOfMass::computeCoMi(const ProportionalModel &model, const KinoveaReader &kino)
{
    _comi.clear();
    for (size_t f = 0; f < kino.GetFrames().size(); ++f){
        Frame comi(model.GetSegments().size());

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
        _comi.push_back(comi);
    }
}

void CenterOfMass::computeCoM(const ProportionalModel &model, const KinoveaReader &kino)
{
    computeCoMi(model, kino);
    _com.clear();

    for (size_t f = 0; f < kino.GetFrames().size(); ++f){
        Frame com(1);

        Frame frame(kino.GetFrames()[f]);
        com.SetTime(frame.GetTime());
        for (size_t s=0; s<model.GetSegments().size(); ++s){
            Point2d tp(model.GetSegments().at(s).GetRelativeMass() * _comi.at(f).GetPoint2d(s));
            com.SetPoint2d(0, com.GetPoint2d(0) + tp);
        }
        com.SetPointName(0, "CenterOfMass");
        _com.push_back(com);
    }
}

const std::vector<Frame> &CenterOfMass::GetComi() const
{
    if (_isComComputed)
        return _comi;
    else
        throw std::runtime_error("Call computeCoMi before this function");
}

const std::vector<Frame> &CenterOfMass::GetCom() const
{
    if (_isComComputed)
        return _com;
    else
        throw std::runtime_error("Call computeCoM before this function");
}
