#ifndef _IMAGES_H_
#define _IMAGES_H_

#include <stdint.h>
#include "common.h"

namespace xControl
{
//------------------------------------------------------
// class Image
//------------------------------------------------------

class Image
{
public:
    Image();
    Image( const uint8_t* rawData, uint16_t dataSize, uint16_t width, uint16_t height );
    ~Image();

    uint16_t Width();
    uint16_t Height();
    uint16_t Size();
    const uint8_t* PROGMEM Data();
    bool IsValid();

private:

    const uint8_t* _data;
    uint16_t _size;
    uint16_t _width; 
    uint16_t _height;
};

Image Splash();
Image Background();
Image Temperature();
Image Humidity();
Image Pressure();

} // end of namespace

#endif