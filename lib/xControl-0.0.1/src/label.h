#pragma once

#include "Arduino.h"
#include "WString.h"

#include "images.h"

class Renderer; //forward declaration

namespace xControl
{

class SSD1306View; //forward declaration

//------------------------------------------------------
// Label
//------------------------------------------------------

class Label
{
public:
  enum Orientation { Centered, LeftJustified, RightJustified };
  enum TextSize { Default = 1, Factor2 = 2, Factor3 = 3 };
  struct CursorPosition
  {
    CursorPosition();
    uint32_t x;
    uint32_t y;
  };

  struct FontSize
  {
    FontSize(uint32_t widht, uint32_t hight);
    FontSize();
    uint32_t _widht;
    uint32_t _hight;
  };

  Label();  
  Label(Renderer* renderer, SSD1306View* parentView, Orientation orientation = Centered);

  virtual ~Label();

  void Init(Renderer* renderer, SSD1306View* parent, Orientation orientation = Centered);

  virtual void SetTextSize(TextSize size);
  virtual size_t Text(const String& text);
  virtual size_t Text(const char* text);

private:

  void CalculatePosition();
  size_t Show(const char* data);

  Renderer* _renderer;
  SSD1306View* _view;
  const View::Image* _icon;
  Orientation _orientation;
  CursorPosition _cursor;
  FontSize _defaultFontSize;
  uint32_t _scaleFactor;
  String _text;
};

} // end of namespace