#include "CenterOfMass.h"

CenterOfMass::CenterOfMass(const ProportionalModel &model, const KinoveaReader &kino) :
    _isComiComputed(false)
{
    computeCoM(model, kino);
}

void CenterOfMass::computeCoMi(const ProportionalModel &model, const KinoveaReader &kino)
{
    _comi.clear();
    for (ProportionalModel::Segment segment : model.GetSegments()){
        size_t proxIdx(model.GetLandmarkIdx(segment.GetProximal().GetName()));
        size_t distIdx(model.GetLandmarkIdx(segment.GetDistal().GetName()));

        std::vector<KinoveaReader::Frame> framesProx(kino.GetFrames()[proxIdx]);
        std::vector<KinoveaReader::Frame> framesDist(kino.GetFrames()[distIdx]);
        std::vector<Point2d> comi_seg;
        size_t nFrames = framesProx.size();
        for (size_t f = 0; f < nFrames; ++f){
            comi_seg.push_back(framesProx[f].GetPoint() + segment.GetCenterOfMassFromProximal() * (framesDist[f].GetPoint() - framesProx[f].GetPoint()));
        }
        _comi.push_back(comi_seg);
    }
    _isComiComputed = true;
}

void CenterOfMass::computeCoM(const ProportionalModel &model, const KinoveaReader &kino)
{
    if (!_isComiComputed)
        computeCoMi(model, kino);

    _com.clear();
    if (kino.GetFrames().size() == 0)
        return;
    size_t nSegments(model.GetSegments().size());
    size_t nFrames(kino.GetFrames()[0].size());
    _com.resize(nFrames);

    for (size_t f=0; f<nFrames; ++f)
        for (size_t s=0; s<nSegments; ++s){
            Point2d tp(model.GetSegments()[s].GetRelativeMass() * _comi[s][f]);
            _com[f] += tp;
        }
}
