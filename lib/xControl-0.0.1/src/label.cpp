#include <stddef.h>
#include <string.h>

#include "label.h"
#include "renderer.h"

namespace xControl
{
  //------------------------------------------------------
  // label -> FontSize
  //------------------------------------------------------  
  Label::FontSize::FontSize(uint32_t widht, uint32_t hight)
  : _widht(widht),
    _hight(hight)
  {
  }

  Label::FontSize::FontSize()
  : _widht(0),
    _hight(0)
  {
  }

  //------------------------------------------------------
  // label -> CursorPosition
  //------------------------------------------------------  

  Label::CursorPosition::CursorPosition()
  : x(0),
    y(0)
  {
  }

  //------------------------------------------------------
  // label
  //------------------------------------------------------  

  Label::Label()
  : _renderer(NULL),
    _orientation(Centered),
    _defaultFontSize(6,8),
    _scaleFactor(2),
    _width(0),
    _height(0),
    _icon(&_defaultEmptyImage)
  {
    memset(_text, 0, sizeof(_text));
  }

  Label::Label(Renderer* renderer, uint32_t width, uint32_t height, Orientation orientation)
  : Label()
  {
    _renderer = renderer;
    _width = width;
    _height = height;
    _orientation = orientation;
  }

  Label::~Label()
  {
  }

  void Label::Init(Renderer* renderer, uint32_t width, uint32_t height, Orientation orientation)
  {
    _renderer = renderer,
    _orientation = orientation;
    _height = height;
    _width  = width;
  }

  void Label::SetTextSize(TextSize size)
  {
    _scaleFactor = (uint32_t)size;
  }

  int32_t Label::HorizontalCenterLine()
  {
    return _height / 2;
  }

  int32_t Label::VerticalCenterLine()
  {
    return _width / 2;
  }

  void Label::Show()
  {
    if(_renderer)
    {
      CalculatePosition();

      _renderer->clearDisplay();      
      if(_icon->IsValid())
      {
        _renderer->drawBitmap(0, 0, _icon->Data(), _icon->Width(), _icon->Height(), 1);
      }
      _renderer->setCursor(_cursor.x, _cursor.y);
      _renderer->setTextSize(_scaleFactor);
      _renderer->print(_text);
      _renderer->Updateframe();
    }
  }

  void Label::Text(const char* text)
  {
    size_t textSize = strlen(text);
    memset( _text, 0, BufferSize);
    strncpy(_text, text, ((textSize > BufferSize) ? BufferSize-1 : textSize));
    Show();
  }

  void Label::SetIcon(xControl::Image& icon)
  {
    _icon = &icon;
  }
  void Label::RemoveIcon()
  {
    _icon = &_defaultEmptyImage;
  }

  void Label::CalculatePosition()
  {
    switch (_orientation)
    {
      case Centered:
      {
        enum{IncreasedAccuracy = 10};
        int32_t halfNumberOfChars = (int32_t)(strlen(_text)*IncreasedAccuracy/2);
        int32_t singleCharWidht = _defaultFontSize._widht * _scaleFactor;
        _cursor.x = VerticalCenterLine() - (halfNumberOfChars * singleCharWidht / IncreasedAccuracy);
        _cursor.y = HorizontalCenterLine() - (_defaultFontSize._hight * _scaleFactor * IncreasedAccuracy / 2 / IncreasedAccuracy);

        if( _cursor.x < 0 || _cursor.y < 0)
        {
          _cursor.x = 0;
          _cursor.y = 0;
        }

        if(_icon->IsValid())
        {
          _cursor.x = _icon->Width() + 1;
        }
        break;
      } 
      default:
      {
        _cursor.x = 0;
        _cursor.y = 0;
        break;
      }
    } 
  }
} // end of namespace