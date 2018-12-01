#include "CenterOfMass.h"

CenterOfMass::CenterOfMass(const ProportionalModel &model, const KinoveaReader &kino)
{
    computeCoMi(model, kino);
}

void CenterOfMass::computeCoMi(const ProportionalModel &model, const KinoveaReader &kino)
{
    for (ProportionalModel::Segment segment : model.GetSegments()){
        segment.GetProximal().GetName();
        for (std::vector<KinoveaReader::Frame> frame : kino.GetFrames()){
        }
    }
//     com_i = {}
//     for (k, m) in winter_table.items():
//         prox = data[m[0]]
//         dist = data[m[1]]
//         com_i[k] = np.reshape(prox + m[3] * (dist - prox), (prox.shape[0], 1, prox.shape[1]))
//     return com_i

}
