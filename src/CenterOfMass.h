#ifndef CENTER_OF_MASS_H
#define CENTER_OF_MASS_H

#include "KinoveaReader.h"
#include "ProportionalModel.h"
#include "Point2d.h"

class CenterOfMass
{
public:
    CenterOfMass(const ProportionalModel&, const KinoveaReader&);

protected:
    void computeCoMi(const ProportionalModel &model, const KinoveaReader &kino);
    std::vector<Point2d> _comi;
};

#endif // CENTER_OF_MASS_H
