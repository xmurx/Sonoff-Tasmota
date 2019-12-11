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

  Label::Label(Renderer* renderer, SSD1306View& parentView, Orientation orientation)
  : _renderer(renderer),
    _view(parentView),
    _orientation(orientation),
    _text((const char*)NULL),
    _defaultFontSize(6,8),
    _scaleFactor(1)
  {
    _text.reserve(16);
  }

  Label::~Label()
  {
  }

  void Label::SetTextSize(TextSize size)
  {
    _scaleFactor = (uint32_t)size;
  }

  size_t Label::Text(const String & text)
  {
    if(_renderer)
    {
      _text = text;
      CalculatePosition();
      _renderer->setTextSize(_scaleFactor);      
      return _renderer->print(_text);
    }
    return 0;
  }

  size_t Label::Text(const char* text)
  {
    if(_renderer)
    {
      _text = text;
      CalculatePosition();
      _renderer->setTextSize(_scaleFactor);
      return _renderer->print(_text);
    }
    return 0;
  }

  void Label::CalculatePosition()
  {
    switch (_orientation)
    {
      case Centered:
      {
        _cursor.x = _view.VerticalCenterLine() - ((_text.length()/2)*(_defaultFontSize._widht * _scaleFactor));
        _cursor.y = _view.HorizontalCenterLine() - ((_defaultFontSize._hight * _scaleFactor)/2);
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