#ifndef PROPORTIONAL_MODEL_H
#define PROPORTIONAL_MODEL_H

#include "XmlReader.h"

class ProportionalModel : public XmlReader
{
public:
    ProportionalModel(const std::string & = "");

    class GenericObject
    {
    public:        
        const std::string& GetName() const;
        void SetName(const std::string& name);
    protected:
        std::string _name;
    };

    class Landmark : public GenericObject {

    };

    class Segment : public GenericObject {
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
        Landmark _proximal;
        Landmark _distal;
        double _relativeMass;
        double _centerOfMassFromProximal;
    };

    void readXml(const std::string &path);

    const std::vector<Landmark> GetLandmarks() const;
    const Landmark& GetLandmark(const std::string& name);

    const std::vector<Segment> GetSegments() const;
    const Segment& GetSegment(const std::string& name);
protected:
    std::string _modelName;
    double _fileVersion;

    std::vector<Landmark> _landmarks;
    std::vector<Segment> _segments;
    std::vector<Landmark> _stickLink;
};

#endif // PROPORTIONAL_MODEL_H
