#ifndef PROPORTIONAL_MODEL_H
#define PROPORTIONAL_MODEL_H

#include "QString"
#include "tinyxml2.h"
#include "exception"
#include <iostream>
#include <vector>

class ProportionalModel
{
public:
    ProportionalModel(QString& path);

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

protected:
    tinyxml2::XMLNode * getNodeProtected(tinyxml2::XMLNode *, const std::string&);
    tinyxml2::XMLNode * getNextChildProtected(tinyxml2::XMLNode *, const std::string&);
    std::string getValueProtected(tinyxml2::XMLNode *);

    std::string _modelName;
    double _fileVersion;

    std::vector<Landmark> _landmarks;
    const Landmark& GetLandmark(const std::string& name);
    std::vector<Segment> _segments;
    std::vector<Landmark> _stickLink;
};

#endif // PROPORTIONAL_MODEL_H
