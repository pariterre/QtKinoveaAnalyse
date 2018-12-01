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

class KinoveaReader : public XmlReader
{
public:
    class Frame
    {
    public:
        Frame();

        double GetTime() const;
        void SetTime(double t);

        const Point2d& GetPoint() const;
        void SetPoint(const Point2d& point);
        void SetPoint(double x, double y);
    protected:
        double _t;
        Point2d _point;
    };

    KinoveaReader();

    void readXml(const std::string& path);
    void readXml(const std::string& path, const ProportionalModel& model);

    const std::vector<std::vector<Frame>> GetFrames() const;
protected:
    double parseTime(const std::string& time);

    std::vector<std::vector<Frame>> _frames;
};

#endif // KINOVEA_READER_H
