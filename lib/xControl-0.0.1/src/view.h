#pragma once

#include "label.h"
#include "view.h"
#include "level.h"

class Renderer;

namespace xControl
{

//------------------------------------------------------
// SSD1306View
//------------------------------------------------------

/**
 * @brief ViewData - data for display view
 * @distance - current distance value
 * @temperature - current temperature value
 * @pressure - current air pressure value
 * @humidity - current air humidity value
 */

class ViewData
{
public:
  ViewData()
  : _distance(0),
    _temperature(0.0),
    _pressure(0),
    _humidity(0.0)
  {
  }

  void SetDistance(int distance) { _distance = distance; }
  void SetTemperature(float temperature) { _temperature = temperature; }
  void SetHumidity(float humidity) { _humidity = humidity; }
  void SetPressure(float pressure) { _pressure = pressure; }
  int Distance() const { return _distance; }
  float Temperature() const { return _temperature; }
  float Humidity() const { return _humidity; }
  float Pressure() const { return _pressure; }

private:
  int _distance;
  float _temperature;
  float _humidity;
  float _pressure;
};

//------------------------------------------------------
// PercentConverter
//------------------------------------------------------

class PercentConverter
{
public:

  PercentConverter(uint32_t minValue,uint32_t maxValue, uint32_t step );
  ~PercentConverter();
  uint32_t ToPercent(uint32_t value);

private:
  uint32_t _minValue;
  uint32_t _maxValue;
  uint32_t _step;
};

//------------------------------------------------------
// StateControl
//------------------------------------------------------

enum State
{
  Unknown = 0,
  ShowSplash,
  Delay,
  ShowLevel,
  ShowTemp,
  ShowHumidity,
  ShowPressure
};

template<typename T>
class StateControl
{
public:

  StateControl()
  : _state(T::Unknown),
    _startTime(0),
    _delayTime(0),
    _onEnter(false)
  { 
  }

  T GetState()
  {
    return _state;
  }

  void SetState(T state, uint32_t delay = 0)
  {
    if(state == T::Delay)
    {
      _delayTime = delay;
      _startTime = millis();
    }

    if(_state != state)
      _onEnter = true;

    _state = state;
  }

  void StartDelay(uint32_t delayIn_ms)
  {
    _delayTime = delayIn_ms;
    _startTime = millis();
  }

  bool DelayExpired()
  {
    bool ret = ((millis() - _startTime) >= _delayTime);
    if(ret)
    {
      ResetDelay();
    }
    return ret;
  }

  void ResetDelay()
  {
    _startTime = 0;
    _delayTime = 0;
  }

  bool OnEnter()
  {
    if(_onEnter == true)
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

//------------------------------------------------------
// SSD1306View
//------------------------------------------------------


class SSD1306View
{
public:
  SSD1306View();
  SSD1306View(Renderer *renderer);
  virtual ~SSD1306View();

  void Init(Renderer *renderer, uint32_t width, uint32_t height);
  void Show(const ViewData &data);
  void Step();
    
private:
  void Process();

  Renderer* _renderer;
  PercentConverter _distanceConverter;
  StateControl<State> _stateControl;
  ViewData _data;
  Label _label;
  Level _level;
  Image _icon;
};

} // namespace xControl