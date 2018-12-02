#ifndef CENTER_OF_MASS_H
#define CENTER_OF_MASS_H

#include "KinoveaReader.h"
#include "ProportionalModel.h"
#include "Point2d.h"

class CenterOfMass
{
public:
    CenterOfMass(const ProportionalModel&, const KinoveaReader&);
    void computeCoMi(const ProportionalModel &model, const KinoveaReader &kino);
    void computeCoM(const ProportionalModel &model, const KinoveaReader &kino);


protected:
    std::vector<std::vector<Point2d>> _comi;
    bool _isComiComputed;
    std::vector<Point2d> _com;
};

#endif // CENTER_OF_MASS_H
