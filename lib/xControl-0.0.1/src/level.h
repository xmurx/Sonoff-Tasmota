#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "viewable.h"

namespace xControl
{

  class Level : public Viewable
  {
  public:
    Level();
    Level(Renderer* renderer, uint32_t width, uint32_t height);
    virtual ~Level();

    void Init(Renderer* renderer, uint32_t width, uint32_t height);
    void Set(uint32_t levelInPercent);
    void Show();

  private:
    void SetDefaults();
    uint32_t _level;
    uint32_t _lastLevel;
  };

} // namespace xControl

#endif