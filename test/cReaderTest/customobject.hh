#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include <iostream>
#include <vector>

class CustomObject
{
    int myInt;
    double myDouble;
    bool myBool;
    std::vector<double> myVector;

public:
    CustomObject();
    ~CustomObject();
    int getMyInt() const;
    void setMyInt(int value);
    double getMyDouble() const;
    void setMyDouble(double value);
    bool getMyBool() const;
    void setMyBool(bool value);
    std::vector<double> getMyVector() const;
    void setMyVector(const std::vector<double> &value);
};

#endif // CUSTOMOBJECT_H
