#include "testclass.hh"

int TestClass::getMyInt() const
{
    return myInt;
}

void TestClass::setMyInt(int value)
{
    myInt = value;
}

double TestClass::getMyDouble() const
{
    return myDouble;
}

void TestClass::setMyDouble(double value)
{
    myDouble = value;
}

std::string TestClass::getMyString() const
{
    return myString;
}

void TestClass::setMyString(const std::string &value)
{
    myString = value;
}

void TestClass::throwException()
{
    throw std::runtime_error("Error");
}

void TestClass::noThrowException()
{

}

void TestClass::throwCustomException()
{
    throw TestException();
}

TestClass::TestClass()
{
    setMyInt(0);
    setMyDouble(0);
    setMyString("");
}

