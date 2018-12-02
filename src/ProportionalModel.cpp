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
        Landmark landmark;
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
        Segment segment;
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
        Landmark vertice;
        vertice.SetName(FirstChildToTextValueProtected(stickLinkNode));
        _stickLink.push_back(vertice);

        // Move to next sibling
        stickLinkNode = stickLinkNode->NextSiblingElement("Vertice");
    }
}

const std::vector<Landmark> &ProportionalModel::GetLandmarks() const
{
    return _landmarks;
}

const Landmark &ProportionalModel::GetLandmark(const std::string &name) const
{
    for (const Landmark& landmark : _landmarks)
        if (!landmark.GetName().compare(name))
            return landmark;
    throw std::ios_base::failure("Landmark not found");
}

size_t ProportionalModel::GetLandmarkIdx(const std::string &name) const
{
    for (size_t i=0; i<_landmarks.size(); ++i)
        if (!_landmarks[i].GetName().compare(name))
            return i;
    throw std::ios_base::failure("Landmark not found");
}

const std::vector<Segment> ProportionalModel::GetSegments() const
{
    return _segments;
}

const Segment &ProportionalModel::GetSegment(const std::string &name)
{
    for (Segment& segment : _segments)
        if (!segment.GetName().compare(name))
            return segment;
    throw std::ios_base::failure("Segment not found");
}
