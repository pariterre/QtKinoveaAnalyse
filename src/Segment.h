#ifndef SEGMENT_H
#define SEGMENT_H

#include <memory>

#include <GenericBody.h>
class Landmark;

class Segment : public GenericBody {
public:
    Segment();

    const Landmark& GetProximal() const;
    void SetProximal(const Landmark& proximal);

    const Landmark& GetDistal() const;
    void SetDistal(const Landmark& distal);

    double GetRelativeMass() const;
    void SetRelativeMass(double relativeMass);

    double GetCenterOfMassFromProximal() const;
    void SetCenterOfMassFromProximal(double com);

protected:
    std::shared_ptr<Landmark> _proximal;
    std::shared_ptr<Landmark> _distal;
    double _relativeMass;
    double _centerOfMassFromProximal;
};


#endif  // SEGMENT_H
