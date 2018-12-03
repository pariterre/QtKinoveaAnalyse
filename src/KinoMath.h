#ifndef KINOMATH_H
#define KINOMATH_H

#include "ProportionalModel.h"
#include "KinoveaReader.h"
#include "Frame.h"

class KinoMath
{
public:
    static std::vector<Frame> computeCoMi(const ProportionalModel &model, const KinoveaReader &kino);
    static std::vector<Frame> computeCoM(const ProportionalModel &model, const KinoveaReader &kino, std::vector<Frame>& comi);

    static std::vector<Frame> computeJointAngles(const ProportionalModel&, const KinoveaReader&);

    static std::vector<Frame> computeDerivative(const std::vector<Frame> &);
};

#endif // KINOMATH_H
