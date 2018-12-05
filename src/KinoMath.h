#ifndef KINOMATH_H
#define KINOMATH_H

#include <vector>

class ProportionalModel;
class KinoveaReader;
class Frame;

class KinoMath
{
public:
    static std::vector<Frame> computeCoMi(const ProportionalModel &model, const KinoveaReader &kino);
    static std::vector<Frame> computeCoM(const ProportionalModel &model, const KinoveaReader &kino, std::vector<Frame>& comi);
    static std::vector<Frame> computeGrf(const std::vector<Frame> comAcceleration, double masse);

    static std::vector<Frame> computeJointAngles(const ProportionalModel&, const KinoveaReader&);

    static std::vector<Frame> computeDerivative(const std::vector<Frame> &);

    static double toDegree(double angleRadian);
    static double toRadian(double angleDegree);
};

#endif // KINOMATH_H
