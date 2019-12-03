#include "Arduino.h"
#include "view.h"
#include "images.h"
#include "Print.h"

class Renderer; // forward declaration

namespace xControl
{

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
  enum Orientation { centered, leftJustified, rightJustified };
  Label(Renderer* renderer, const SSD1306View* parentView);
  virtual ~Label();

  virtual size_t Print(const String & data);
  virtual size_t Print(const char data[]);
  virtual size_t Print(char data);
  virtual size_t Print(unsigned char data, int = DEC);
  virtual size_t Print(int value, int format = DEC);
  virtual size_t Print(unsigned int value, int format = DEC);
  virtual size_t Print(long value, int format = DEC);
  virtual size_t Print(unsigned long value, int format = DEC);
  virtual size_t Print(double value, int pression = 2);

private:

  Renderer* _renderer;
  const SSD1306View* _view;
  const View::Image* icon;
};

} // end of namespace