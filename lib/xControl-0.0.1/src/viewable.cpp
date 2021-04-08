#include "viewable.h"

namespace xControl
{
  Viewable::Viewable() :
  _renderer(NULL),
  _width(0),
  _height(0)
  {
  }

  Viewable::Viewable(Renderer* renderer, uint32_t width, uint32_t height) :
  _renderer(renderer),
  _width(width),
  _height(height)
  {
  }

  Viewable::~Viewable()
  {
  }

  void Viewable::Init(Renderer* renderer, uint32_t widht, uint32_t height)
  {
    _renderer = renderer;
    _width = widht;
    _height = height;
  }

  void Viewable::Show()
  {
  }
} // namespace xControl