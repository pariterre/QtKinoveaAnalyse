#ifndef KINOVEA_READER_H
#define KINOVEA_READER_H

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <cmath>
#include <vector>

#include <XmlReader.h>
class ProportionalModel;
class Frame;

class KinoveaReader : public XmlReader
{
public:
    KinoveaReader();

    void readXml(const std::string& path);
    void readXml(const std::string& path, const ProportionalModel& model);

    const std::vector<Frame>& GetFrames() const;
protected:
    double parseTime(const std::string& time);

    std::vector<Frame> _frames;
};

#endif // KINOVEA_READER_H
