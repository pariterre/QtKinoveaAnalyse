#ifndef KINOVEA_READER_H
#define KINOVEA_READER_H

#include "XmlReader.h"
#include "ProportionalModel.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <cmath>

class KinoveaReader : public XmlReader
{
public:
    class Frame
    {
    public:
        Frame();

        double GetTime() const;
        void SetTime(double t);

        double GetX() const;
        void SetX(double x);

        double GetY() const;
        void SetY(double y);

    protected:
        double _t;
        double _x;
        double _y;
    };

    KinoveaReader();

    void readXml(const std::string& path);
    void readXml(const std::string& path, const ProportionalModel& model);
protected:
    double parseTime(const std::string& time);

    std::vector<std::vector<Frame>> _frames;
};

#endif // KINOVEA_READER_H
