#ifndef CENTER_OF_MASS_H
#define CENTER_OF_MASS_H

#include "KinoveaReader.h"
#include "ProportionalModel.h"
#include "Point2d.h"

class CenterOfMass
{
public:
    CenterOfMass();
    void computeCoMi(const ProportionalModel &model, const KinoveaReader &kino);
    void computeCoM(const ProportionalModel &model, const KinoveaReader &kino);

    const std::vector<Frame> &GetComi() const;
    const std::vector<Frame> &GetCom() const;
protected:
    std::vector<Frame> _comi;
    bool _isComComputed;
    std::vector<Frame> _com;
};

#endif // CENTER_OF_MASS_H
