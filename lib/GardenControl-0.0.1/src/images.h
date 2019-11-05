#ifndef _IMAGES_H_
#define _IMAGES_H_

#include "Arduino.h"

namespace XView
{

//------------------------------------------------------
// class Image
//------------------------------------------------------

class Image
{
public:

    Image( const uint8_t* rawData, uint16_t dataSize, uint16_t width, uint16_t height );
    uint16_t Width();
    uint16_t Height();
    uint16_t Size();

    const uint8_t* PROGMEM Data();

private:

    const uint8_t* _data;
    uint16_t _size;
    uint16_t _width; 
    uint16_t _height;
};

Image Splash();
Image Background();

}

#endif
