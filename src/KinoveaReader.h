#ifndef KINOVEA_READER_H
#define KINOVEA_READER_H

#include "XmlReader.h"
#include "ProportionalModel.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <cmath>

#include "Point2d.h"
#include "Frame.h"

class KinoveaReader : public XmlReader
{
public:
    KinoveaReader();

    void readXml(const std::string& path);
    void readXml(const std::string& path, const ProportionalModel& model);

    const std::vector<std::vector<Frame>> GetFrames() const;
protected:
    double parseTime(const std::string& time);

    std::vector<std::vector<Frame>> _frames;
};

#endif // KINOVEA_READER_H
