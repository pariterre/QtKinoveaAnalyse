#ifndef KINOVEA_READER_H
#define KINOVEA_READER_H

#include "XmlReader.h"

class KinoveaReader : public XmlReader
{
public:
    KinoveaReader(const std::string & = "");

    void readXml(const std::string& path);
protected:

};

#endif // KINOVEA_READER_H
