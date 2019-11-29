#ifndef _VIEW_H_
#define _VIEW_H_

#include "Arduino.h"
#include "logging.h"
#include "renderer.h"
#include "view.h"

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

    void PrintInfo()
    {
        char tempBuffer[8];
        dtostrf(_temperature, 3, 2, tempBuffer);

        char humBuffer[16];
        dtostrf(_humidity, 4, 2, humBuffer);

        char presBuffer[16];
        dtostrf(_pressure, 4, 2, presBuffer);

        Logging::Debug("View - Temp: %s, Hum: %s, Press: %s, Distance: %d",
                       tempBuffer,
                       humBuffer,
                       presBuffer,
                       _distance);
    }

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
    class StateControl
    {
    public:
        enum State
        {
            Unknown,
            ShowSplash,
            Delay,
            ShowLevel,
            ShowTemp,
            ShowHumidity,
            ShowPressure
        };
        StateControl();

        State GetState();
        void SetState(State state, uint32_t delay = 0);

        void StartDelay(uint32_t delayIn_ms);
        bool DelayExpired();
        void ResetDelay();
        bool OnEnter();

    private:
        State _state;
        uint32_t _startTime;
        uint32_t _delayTime;
        bool _onEnter;
    };

    Renderer *_renderer;
    PercentConverter _distanceConverter;
    uint32_t _width;
    uint32_t _height;

    StateControl _stateControl;
    ViewData _data;
};

} // namespace xControl

#endif