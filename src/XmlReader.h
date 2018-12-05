#ifndef XML_READER_H
#define XML_READER_H

#include <iostream>

namespace tinyxml2 {
class XMLElement;
}

class XmlReader
{
public:
    virtual ~XmlReader();
    virtual void readXml(const std::string& path) = 0;
protected:
    tinyxml2::XMLElement *FirstChildElementProtected(tinyxml2::XMLElement *, const std::string&);
    tinyxml2::XMLElement * NextSiblingElementProtected(tinyxml2::XMLElement *, const std::string&);
    std::string FirstChildToTextValueProtected(tinyxml2::XMLElement *);
    std::string AttributeProperErrorMessage(tinyxml2::XMLElement *, const std::string& attributeName);
};

#endif // XML_READER_H
