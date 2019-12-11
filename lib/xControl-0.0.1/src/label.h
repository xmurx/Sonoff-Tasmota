#include "Arduino.h"
#include "view.h"
#include "images.h"
#include "Print.h"
#include "WString.h"

class Renderer; // forward declaration

namespace xControl
{

//------------------------------------------------------
// IPrintable
//------------------------------------------------------  

class IPrintable
{
public:

  virtual size_t Print(const String & data) = 0;
  virtual size_t Print(const char data[]) = 0;
  virtual size_t Print(char data) = 0;
  virtual size_t Print(unsigned char data, int = DEC) = 0;
  virtual size_t Print(int value, int format = DEC) = 0;
  virtual size_t Print(unsigned int value, int format = DEC) = 0;
  virtual size_t Print(long value, int format = DEC) = 0;
  virtual size_t Print(unsigned long value, int format = DEC) = 0;
  virtual size_t Print(double value, int pression = 2) = 0;

};

//------------------------------------------------------
// Label
//------------------------------------------------------

class Label : public IPrintable
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
  
  Label(Renderer* renderer, SSD1306View& parentView, Orientation orientation = Centered);
  virtual ~Label();

  virtual void SetTextSize(TextSize size);
  virtual size_t Text(const String& text);
  virtual size_t Text(const char* text);

private:

  void CalculatePosition();

  Renderer* _renderer;
  SSD1306View& _view;
  const View::Image* _icon;
  Orientation _orientation;
  CursorPosition _cursor;
  FontSize _defaultFontSize;
  uint32_t _scaleFactor;
  String _text;
};

} // end of namespace