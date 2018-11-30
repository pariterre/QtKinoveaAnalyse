#include "Winter.h"

Winter::Winter(QString &path)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(path.toStdString().c_str());

    // Read header
    tinyxml2::XMLNode * header = getNodeProtected(&xml, "Header");
    _fileVersion = stod(getValueProtected(getNodeProtected(header, "Version")));
    if (_fileVersion < 1.0)
        throw std::ios_base::failure("Wrong file version");
    _modelName = getValueProtected(getNodeProtected(header, "Title"));

    // Read the landmarks
    _landmarks.clear();
    tinyxml2::XMLNode * landmarkNode = getNodeProtected(getNodeProtected(&xml, "Landmarks"), "Landmark");
    while (true){
        if (!landmarkNode)
            break;
        Winter::Landmark landmark;
        landmark.SetName(getValueProtected(getNodeProtected(landmarkNode, "Name")));
        _landmarks.push_back(landmark);

        // Move to next sibling
        landmarkNode = landmarkNode->NextSiblingElement("Landmark");
    }

    // Read the segments
    _segments.clear();
    tinyxml2::XMLNode * segmentNode = getNodeProtected(getNodeProtected(&xml, "Segments"), "Segment");
    while (true){
        if (!segmentNode)
            break;
        Winter::Segment segment;
        segment.SetName(getValueProtected(getNodeProtected(segmentNode, "Name")));
        segment.SetProximal(GetLandmark(getValueProtected(getNodeProtected(segmentNode, "Proximal"))));
        segment.SetDistal(GetLandmark(getValueProtected(getNodeProtected(segmentNode, "Distal"))));
        segment.SetRelativeMass(stod(getValueProtected(getNodeProtected(segmentNode, "RelativeMass"))));
        segment.SetCenterOfMassFromProximal(stod(getValueProtected(getNodeProtected(segmentNode, "CenterOfMassFromProximal"))));
        _segments.push_back(segment);

        // Move to next sibling
        segmentNode = segmentNode->NextSiblingElement("Segment");
    }

    // Read the stick figure links
    _stickLink.clear();
    tinyxml2::XMLNode * stickLinkNode = getNodeProtected(getNodeProtected(&xml, "StickFigure"), "Vertice");
    while (true){
        if (!stickLinkNode)
            break;
        Winter::Landmark vertice;
        vertice.SetName(getValueProtected(stickLinkNode));
        _stickLink.push_back(vertice);

        // Move to next sibling
        stickLinkNode = stickLinkNode->NextSiblingElement("Vertice");
    }
}

tinyxml2::XMLNode *Winter::getNodeProtected(tinyxml2::XMLNode *node, const std::string &tagName)
{
    tinyxml2::XMLNode * newNode = node->FirstChildElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

tinyxml2::XMLNode *Winter::getNextChildProtected(tinyxml2::XMLNode *node, const std::string &tagName)
{
    tinyxml2::XMLNode * newNode = node->NextSiblingElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

std::string Winter::getValueProtected(tinyxml2::XMLNode *node)
{
    tinyxml2::XMLNode * newNode = node->FirstChild();
    if (newNode)
        return newNode->ToText()->Value();
    else
        throw std::ios_base::failure("Value not found");
}

const Winter::Landmark &Winter::GetLandmark(const std::string &name)
{
    for (Winter::Landmark& landmark : _landmarks)
        if (!landmark.GetName().compare(name))
            return landmark;
    throw std::ios_base::failure("Landmark not found");
}

const std::string &Winter::GenericObject::GetName() const
{
    return _name;
}

void Winter::GenericObject::SetName(const std::string &name)
{
    _name = name;
}

const Winter::Landmark &Winter::Segment::GetProximal() const
{
    return _proximal;
}

void Winter::Segment::SetProximal(const Winter::Landmark &proximal)
{
    _proximal = proximal;
}

const Winter::Landmark &Winter::Segment::GetDistal() const
{
    return _distal;
}

void Winter::Segment::SetDistal(const Winter::Landmark &distal)
{
    _distal = distal;
}

double Winter::Segment::GetRelativeMass() const
{
    return _relativeMass;
}

void Winter::Segment::SetRelativeMass(double relativeMass)
{
    _relativeMass = relativeMass;
}

double Winter::Segment::GetCenterOfMassFromProximal() const
{
    return _centerOfMassFromProximal;
}

void Winter::Segment::SetCenterOfMassFromProximal(double com)
{
    _centerOfMassFromProximal = com;
}
