#include "distance.h"
#include <cmath>

using namespace std;

Distance::Distance()
: _angle(0),
  _amplitude(1),
  _offset(0)
{
}

Distance::~Distance()
{
}

bool Distance::Init(uint32_t startAngle, uint32_t amplitude, uint32_t offset)
{
    _angle = startAngle;
    _amplitude = amplitude;
    _offset = offset;
    return false;
}

double Distance::Read()
{
    double value = (std::sin(2*PI*_angle/360)*_amplitude)+_offset;
    _angle = (_angle + 10) % 360;
    return value;
}

bool Distance::Write()
{
    //no write possible
    return false;
}

string Distance::ToString(double value)
{
    enum{ precision = 2, width = 7};
    char convertBuffer[32] = {0};
    dtostrf(value,width,precision,convertBuffer);
    return convertBuffer;
}

string Distance::Name()
{
    return "VDM001";
}