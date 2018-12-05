#include <XmlReader.h>

#include <exception>
#include <tinyxml2.h>

XmlReader::~XmlReader()
{

}


tinyxml2::XMLElement *XmlReader::FirstChildElementProtected(tinyxml2::XMLElement *node, const std::string &tagName)
{
    tinyxml2::XMLElement * newNode = node->FirstChildElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

tinyxml2::XMLElement *XmlReader::NextSiblingElementProtected(tinyxml2::XMLElement *node, const std::string &tagName)
{
    tinyxml2::XMLElement * newNode = node->NextSiblingElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

std::string XmlReader::FirstChildToTextValueProtected(tinyxml2::XMLElement *node)
{
    tinyxml2::XMLNode * newNode = node->FirstChild();
    if (newNode)
        return newNode->ToText()->Value();
    else
        throw std::ios_base::failure("Value not found");
}

std::string XmlReader::AttributeProperErrorMessage(tinyxml2::XMLElement *node, const std::string &attributeName)
{
    try {
        return node->Attribute(attributeName.c_str());
    } catch (std::logic_error) {
        throw std::ios_base::failure(attributeName + " attribute not found");
    }
}
