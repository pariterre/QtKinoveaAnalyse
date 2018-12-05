#include <GenericBody.h>

GenericBody::GenericBody(const std::string &name) :
    _name(name)
{

}

const std::string &GenericBody::GetName() const
{
    return _name;
}

void  GenericBody::SetName(const std::string &name)
{
    _name = name;
}

