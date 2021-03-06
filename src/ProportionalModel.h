#ifndef PROPORTIONAL_MODEL_H
#define PROPORTIONAL_MODEL_H

#include <vector>

#include <XmlReader.h>
class Joint;
class Landmark;
class Segment;

class ProportionalModel : public XmlReader
{
public:
    ProportionalModel(const std::string & = "");
    void readXml(const std::string &path);

    const std::vector<Landmark>& GetLandmarks() const;
    const Landmark& GetLandmark(const std::string& name) const;
    size_t GetLandmarkIdx(const std::string& name) const;

    const std::vector<Segment>& GetSegments() const;
    const Segment& GetSegment(const std::string& name);

    const std::vector<Joint>& GetJoints() const;
    const std::vector<Landmark>& GetStickLink() const;
protected:
    std::string _modelName;
    double _fileVersion;

    std::vector<Landmark> _landmarks;
    std::vector<Segment> _segments;
    std::vector<Landmark> _stickLink;
    std::vector<Joint> _joints;
};

#endif // PROPORTIONAL_MODEL_H
