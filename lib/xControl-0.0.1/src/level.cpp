#include "level.h"
#include "logging.h"

namespace xControl
{
  Level::Level() :
  Viewable()
  {
    SetDefaults();
  }

  Level::Level(Renderer* renderer, uint32_t width, uint32_t height) :
  Viewable(renderer, width, height)
  {
    SetDefaults();
  }

  Level::~Level() {}

  void Level::SetDefaults()
  {
    _level = 0;
    _lastLevel = 0;
  }

  void Level::Init(Renderer* renderer, uint32_t width, uint32_t height)
  {
    Viewable::Init(renderer, width, height);
  }

  void Level::Set(uint32_t levelInPercent)
  {
    _level = levelInPercent;
  }

  void Level::Show()
  {
    //    if(_lastLevel != _level)
    //Logging::Debug("Level::Show() - level: %d, last level: %d", _level, _lastLevel);
    {
      enum
      {
        width = 128,
        height = 32
      };
      _lastLevel = _level;
      _renderer->clearDisplay();
      _renderer->drawChar(0, 3, 'E', 1, 0, 2);
      _renderer->drawRect(12, 0, _width - 24, _height - 12, 1);
      _renderer->drawChar(118, 3, 'F', 1, 0, 2);
      _renderer->fillRect(15, 3, (100 - _level), 26 - 12, 1);

      _renderer->setTextSize(1);
      _renderer->setCursor(28, 23);
      _renderer->print("Abwassertank");
      _renderer->Updateframe();
    }
  }
} // end of namespace xControl