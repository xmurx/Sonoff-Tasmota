#ifndef __VIEWABLE_H__
#define __VIEWABLE_H__

#include "renderer.h"

namespace xControl
{
  class Viewable
  {
  public:

    Viewable();
    Viewable(Renderer* renderer, uint32_t width, uint32_t height);
    virtual ~Viewable();

    virtual void Init(Renderer* renderer, uint32_t widht, uint32_t height);
    virtual void Show();

  protected:

    Renderer* _renderer;
    uint32_t _width;
    uint32_t _height;
  };
} // end of namespace xControl

#endif