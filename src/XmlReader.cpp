#include "XmlReader.h"

XmlReader::~XmlReader()
{

}


tinyxml2::XMLNode *XmlReader::FirstChildElementProtected(tinyxml2::XMLNode *node, const std::string &tagName)
{
    tinyxml2::XMLNode * newNode = node->FirstChildElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

tinyxml2::XMLNode *XmlReader::NextSiblingElementProtected(tinyxml2::XMLNode *node, const std::string &tagName)
{
    tinyxml2::XMLNode * newNode = node->NextSiblingElement(tagName.c_str());
    if (newNode)
        return newNode;
    else
        throw std::ios_base::failure(tagName + " tag not found");
}

std::string XmlReader::FirstChildToTextValueProtected(tinyxml2::XMLNode *node)
{
    tinyxml2::XMLNode * newNode = node->FirstChild();
    if (newNode)
        return newNode->ToText()->Value();
    else
        throw std::ios_base::failure("Value not found");
}
