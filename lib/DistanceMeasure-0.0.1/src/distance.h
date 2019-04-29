#ifndef _DISTANCE_H_
#define _DISTANCE_H_

#include <Arduino.h>
#include <string>

class Sensor
{
public:

    virtual double Read() = 0;
    virtual bool Write()  = 0;
    virtual std::string Name()    = 0;
};

class Distance: public Sensor
{
public:

    Distance();
    virtual ~Distance();

    bool Init(uint32_t startAngle, uint32_t amplitude, uint32_t offset);
    
    virtual double Read();
    virtual bool Write();
    virtual std::string Name();

    std::string ToString(double value);

private:

    uint32_t _angle;
    uint32_t _amplitude;
    uint32_t _offset;
};

#endif