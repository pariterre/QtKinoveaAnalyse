#ifndef XML_READER_H
#define XML_READER_H

#include "tinyxml2.h"
#include "exception"
#include <iostream>
#include <vector>

class XmlReader
{
public:
    virtual ~XmlReader();
    virtual void readXml(const std::string& path) = 0;
protected:
    tinyxml2::XMLNode * getNodeProtected(tinyxml2::XMLNode *, const std::string&);
    tinyxml2::XMLNode * getNextChildProtected(tinyxml2::XMLNode *, const std::string&);
    std::string getValueProtected(tinyxml2::XMLNode *);

};

#endif // XML_READER_H
