#ifndef GENERIC_BODY_H
#define GENERIC_BODY_H

#include <vector>
#include <iostream>

class GenericBody
{
public:
    GenericBody(const std::string& name = "");
    const std::string& GetName() const;
    void SetName(const std::string& name);
protected:
    std::string _name;
};

#endif  // GENERIC_BODY_H
