#ifndef __LABEL_H__
#define __LABEL_H__

#include <stdint.h>

#include "common.h"
#include "images.h"
#include "viewable.h"

class Renderer;
namespace xControl
{

//------------------------------------------------------
// Label (R is the used Renderer -> depending on runtime environment)
//------------------------------------------------------

class Label : public Viewable
{
public:

  enum Orientation { Centered, LeftJustified, RightJustified };
  enum TextSize { Default = 1, Factor2 = 2, Factor3 = 3 };
  struct CursorPosition
  {
    CursorPosition();
    int32_t x;
    int32_t y;
  };

  struct FontSize
  {
    FontSize(uint32_t widht, uint32_t hight);
    FontSize();
    uint32_t _widht;
    uint32_t _hight;
  };

  Label();  
  Label(Renderer* renderer, uint32_t width, uint32_t height, Orientation orientation = Centered);
  virtual ~Label();

  void Init(Renderer* renderer, uint32_t width, uint32_t height, Orientation orientation = Centered);
  void SetTextSize(TextSize size);
  void Text(const char* text);

  void SetIcon(xControl::Image& icon);
  void RemoveIcon();

private:

  void CalculatePosition();
  void CheckRange();
  void Show();
  void SetDefaultValues();
  int32_t HorizontalCenterLine();
  int32_t VerticalCenterLine();

  xControl::Image* _icon;
  xControl::Image _defaultEmptyImage;

  Orientation _orientation;
  CursorPosition _cursor;
  FontSize _defaultFontSize;
  uint32_t _scaleFactor;

  enum {BufferSize = 16 };
  char _text[BufferSize];
};

} // end of namespace

#endif //__LABEL_H__