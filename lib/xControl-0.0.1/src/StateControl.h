#ifndef __STATE_CONTROL_H__
#define __STATE_CONTROL_H__

#include <stdint.h>

extern "C" unsigned long millis();
namespace xControl
{
  template <typename T>
  class StateControlBase
  {
  public:
    StateControlBase() {}
    virtual ~StateControlBase() {}

    virtual T GetState() = 0;
    virtual void SetState(T state) = 0;
    virtual void StartDelay(uint32_t delay) = 0;
    virtual bool DelayExpired() = 0;
    virtual void ResetDelay() = 0;
    virtual bool OnEnter() = 0;
  };

  template <typename T>
  struct StateTrait
  {
    static const T initialValue;
  };

  template <typename T>
  class StateControl : public StateControlBase<T>
  {
  public:
    StateControl() :
    _state(StateTrait<T>::initialValue),
    _startTime(0),
    _delayTime(0),
    _onEnter(false)
    {
    }

    virtual T GetState()
    {
      return _state;
    }

    virtual void SetState(T state)
    {
      if (_state != state)
        _onEnter = true;

      _state = state;
    }

    virtual void StartDelay(uint32_t delayIn_ms)
    {
      _delayTime = delayIn_ms;
      _startTime = ::millis();
    }

    virtual bool DelayExpired()
    {
      bool ret = ((::millis() - _startTime) >= _delayTime);
      if (ret)
      {
        ResetDelay();
      }
      return ret;
    }

    virtual void ResetDelay()
    {
      _startTime = 0;
      _delayTime = 0;
    }

    virtual bool OnEnter()
    {
      if (_onEnter == true)
      {
        _onEnter = false;
        return true;
      }
      return _onEnter;
    }

  private:
    T _state;
    uint32_t _startTime;
    uint32_t _delayTime;
    bool _onEnter;
  };
} // end of namespace xControl

#endif