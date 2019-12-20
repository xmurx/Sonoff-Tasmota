#include <stddef.h>

#include "label.h"
#include "renderer.h"
#include "view.h"

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
    _view(NULL),
    _orientation(Centered),
    _text((const char*)NULL),
    _defaultFontSize(6,8),
    _scaleFactor(2)
  {
    _text.reserve(16);
  }

  Label::Label(Renderer* renderer, SSD1306View* parentView, Orientation orientation)
  : Label()
  {
    _renderer = renderer;
    _view = parentView;
    _orientation = orientation;
  }

  Label::~Label()
  {
  }

  void Label::Init(Renderer* renderer, SSD1306View* parent, Orientation orientation)
  {
    _renderer = renderer,
    _view = parent;
    _orientation = orientation;
  }

  void Label::SetTextSize(TextSize size)
  {
    _scaleFactor = (uint32_t)size;
  }

  size_t Label::Show(const char* text)
  {
    size_t size = 0;
    if(_renderer)
    {
      _text = text;
      CalculatePosition();
      _renderer->setCursor(_cursor.x, _cursor.y);
      _renderer->clearDisplay();
      _renderer->setTextSize(_scaleFactor);
      size = _renderer->print(_text);
      _renderer->Updateframe();
      return size;
    }
    return size;
  }

  size_t Label::Text(const String & text)
  {
    return Show(text.c_str());
  }

  size_t Label::Text(const char* text)
  {
    return Show(text);
  }

  void Label::CalculatePosition()
  {
    switch (_orientation)
    {
      case Centered:
      {
        _cursor.x = _view->VerticalCenterLine() - ((_text.length()/2)*(_defaultFontSize._widht * _scaleFactor));
        _cursor.y = _view->HorizontalCenterLine() - ((_defaultFontSize._hight * _scaleFactor)/2);
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