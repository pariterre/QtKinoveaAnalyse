#include "ProportionalModel.h"


ProportionalModel::ProportionalModel(const std::string &path) :
    _modelName(""),
    _fileVersion(-1)
{
    if (path.compare(""))
        readXml(path);
}

void ProportionalModel::readXml(const std::string &path)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(path.c_str());
    tinyxml2::XMLNode * root(FirstChildElementProtected(&xml, "ProportionalModel"));

    // Read header
    tinyxml2::XMLNode * header = FirstChildElementProtected(root, "Header");
    _fileVersion = stod(FirstChildToTextValueProtected(FirstChildElementProtected(header, "Version")));
    if (_fileVersion < 1.0)
        throw std::ios_base::failure("Wrong file version");
    _modelName = FirstChildToTextValueProtected(FirstChildElementProtected(header, "Title"));

    // Read the landmarks
    _landmarks.clear();
    tinyxml2::XMLNode * landmarkNode = FirstChildElementProtected(FirstChildElementProtected(root, "Landmarks"), "Landmark");
    while (true){
        if (!landmarkNode)
            break;
        ProportionalModel::Landmark landmark;
        landmark.SetName(FirstChildToTextValueProtected(FirstChildElementProtected(landmarkNode, "Name")));
        _landmarks.push_back(landmark);

        // Move to next sibling
        landmarkNode = landmarkNode->NextSiblingElement("Landmark");
    }

    // Read the segments
    _segments.clear();
    tinyxml2::XMLNode * segmentNode = FirstChildElementProtected(FirstChildElementProtected(root, "Segments"), "Segment");
    while (true){
        if (!segmentNode)
            break;
        ProportionalModel::Segment segment;
        segment.SetName(FirstChildToTextValueProtected(FirstChildElementProtected(segmentNode, "Name")));
        segment.SetProximal(GetLandmark(FirstChildToTextValueProtected(FirstChildElementProtected(segmentNode, "Proximal"))));
        segment.SetDistal(GetLandmark(FirstChildToTextValueProtected(FirstChildElementProtected(segmentNode, "Distal"))));
        segment.SetRelativeMass(stod(FirstChildToTextValueProtected(FirstChildElementProtected(segmentNode, "RelativeMass"))));
        segment.SetCenterOfMassFromProximal(stod(FirstChildToTextValueProtected(FirstChildElementProtected(segmentNode, "CenterOfMassFromProximal"))));
        _segments.push_back(segment);

        // Move to next sibling
        segmentNode = segmentNode->NextSiblingElement("Segment");
    }

    // Read the stick figure links
    _stickLink.clear();
    tinyxml2::XMLNode * stickLinkNode = FirstChildElementProtected(FirstChildElementProtected(root, "StickFigure"), "Vertice");
    while (true){
        if (!stickLinkNode)
            break;
        ProportionalModel::Landmark vertice;
        vertice.SetName(FirstChildToTextValueProtected(stickLinkNode));
        _stickLink.push_back(vertice);

        // Move to next sibling
        stickLinkNode = stickLinkNode->NextSiblingElement("Vertice");
    }
}

const std::vector<ProportionalModel::Landmark> ProportionalModel::GetLandmarks() const
{
    return _landmarks;
}

const ProportionalModel::Landmark &ProportionalModel::GetLandmark(const std::string &name)
{
    for (ProportionalModel::Landmark& landmark : _landmarks)
        if (!landmark.GetName().compare(name))
            return landmark;
    throw std::ios_base::failure("Landmark not found");
}

const std::vector<ProportionalModel::Segment> ProportionalModel::GetSegments() const
{
    return _segments;
}

const ProportionalModel::Segment &ProportionalModel::GetSegment(const std::string &name)
{
    for (ProportionalModel::Segment& segment : _segments)
        if (!segment.GetName().compare(name))
            return segment;
    throw std::ios_base::failure("Segment not found");
}

const std::string &ProportionalModel::GenericObject::GetName() const
{
    return _name;
}

void ProportionalModel::GenericObject::SetName(const std::string &name)
{
    _name = name;
}

ProportionalModel::Segment::Segment() :
    _proximal(ProportionalModel::Landmark()),
    _distal(ProportionalModel::Landmark()),
    _relativeMass(0),
    _centerOfMassFromProximal(0)
{

}

const ProportionalModel::Landmark &ProportionalModel::Segment::GetProximal() const
{
    return _proximal;
}

void ProportionalModel::Segment::SetProximal(const ProportionalModel::Landmark &proximal)
{
    _proximal = proximal;
}

const ProportionalModel::Landmark &ProportionalModel::Segment::GetDistal() const
{
    return _distal;
}

void ProportionalModel::Segment::SetDistal(const ProportionalModel::Landmark &distal)
{
    _distal = distal;
}

double ProportionalModel::Segment::GetRelativeMass() const
{
    return _relativeMass;
}

void ProportionalModel::Segment::SetRelativeMass(double relativeMass)
{
    _relativeMass = relativeMass;
}

double ProportionalModel::Segment::GetCenterOfMassFromProximal() const
{
    return _centerOfMassFromProximal;
}

void ProportionalModel::Segment::SetCenterOfMassFromProximal(double com)
{
    _centerOfMassFromProximal = com;
}
