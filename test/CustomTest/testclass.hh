#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <stdexcept>

class TestException : public std::exception
{
    virtual const char* what() const throw() {
        return "My exception happened";
    }
};

class TestClass
{
    int myInt;
    double myDouble;
    std::string myString;
public:
    TestClass();
    int getMyInt() const;
    void setMyInt(int value);
    double getMyDouble() const;
    void setMyDouble(double value);
    std::string getMyString() const;
    void setMyString(const std::string &value);

    void throwException();
    void noThrowException();
    void throwCustomException();
};

#endif // TESTCLASS_H
