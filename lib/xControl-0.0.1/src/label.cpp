#include "label.h"
#include "renderer.h"

namespace xControl
{

//------------------------------------------------------
// label
//------------------------------------------------------  

  Label::Label(Renderer* renderer, const SSD1306View* parentView)
  : _renderer(renderer),
    _view(parentView)
  {
    
  }

  Label::~Label()
  {

  }

  size_t Label::Print(const String & data)
  {
    if(_renderer)
    {
      return _renderer->print(data);
    }
    return 0;
  }

  size_t Label::Print(const char data[])
  {
    if(_renderer)
    {
      return _renderer->print(data);
    }
    return 0;
  }

  size_t Label::Print(char data)
  {
    if(_renderer)
    {
      return _renderer->print(data);
    }
    return 0;
  }

  size_t Label::Print(unsigned char data, int format)
  {
    if(_renderer)
    {
      return _renderer->print(data, format);
    }
    return 0;
  }

  size_t Label::Print(int value, int format)
  {
    if(_renderer)
    {
      return _renderer->print(value, format);
    }
    return 0;
  }

  size_t Label::Print(unsigned int value, int format)
  {
    if(_renderer)
    {
      return _renderer->print(value, format);
    }
    return 0;
  }

  size_t Label::Print(long value, int format)
  {
    if(_renderer)
    {
      return _renderer->print(value, format);
    }
    return 0;
  }

  size_t Label::Print(unsigned long value, int format)
  {
    if(_renderer)
    {
      return _renderer->print(value, format);
    }
    return 0;
  }

  size_t Label::Print(double value, int pression)
  {
    if(_renderer)
    {
      return _renderer->print(value, pression);
    }
    return 0;
  }
} // end of namespace