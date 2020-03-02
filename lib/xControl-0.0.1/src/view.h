#pragma once
#include "StateControl.h"
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

template<>
struct StateTrait<State>{ static const State initialValue = State::Unknown; };

//------------------------------------------------------
// SSD1306View
//------------------------------------------------------

class SSD1306View
{
public:
  SSD1306View();
  SSD1306View(Renderer *renderer, StateControlBase<State>* stateControl);
  virtual ~SSD1306View();

  void Init(Renderer *renderer, StateControlBase<State>* stateControl, uint32_t width, uint32_t height);
  void Show(const ViewData &data);
  void Step();
    
private:
  void Process();

  Renderer* _renderer;
  StateControlBase<State>* _stateControl;
  PercentConverter _distanceConverter;
  ViewData _data;
  Label _label;
  Level _level;
  Image _icon;
};

} // namespace xControl