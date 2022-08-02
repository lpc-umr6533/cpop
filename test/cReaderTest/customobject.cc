#include "customobject.hh"

int CustomObject::getMyInt() const
{
    return myInt;
}

void CustomObject::setMyInt(int value)
{
    myInt = value;
}

double CustomObject::getMyDouble() const
{
    return myDouble;
}

void CustomObject::setMyDouble(double value)
{
    myDouble = value;
}

bool CustomObject::getMyBool() const
{
    return myBool;
}

void CustomObject::setMyBool(bool value)
{
    myBool = value;
}

std::vector<double> CustomObject::getMyVector() const
{
    return myVector;
}

void CustomObject::setMyVector(const std::vector<double> &value)
{
    myVector = value;
}

CustomObject::CustomObject()
{
    
}

CustomObject::~CustomObject()
{
}

