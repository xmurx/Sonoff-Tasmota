#include "view.h"
#include "logging.h"
#include "renderer.h"
#include "WString.h"

unsigned long millis();
namespace xControl
{
  //------------------------------------------------------
  // PercentConverter
  //------------------------------------------------------    

  PercentConverter::PercentConverter(uint32_t minValue, uint32_t maxValue, uint32_t step )
  : _minValue(minValue),
    _maxValue(maxValue),
    _step(step)
  {
  }

  PercentConverter::~PercentConverter()
  {
  }

  uint32_t PercentConverter::ToPercent(uint32_t value)
  {
    uint32_t percent = (value * 100)/(_maxValue-_minValue);
    return percent > 100 ? 100 : percent;
  }

  //------------------------------------------------------
  // SSD1306View
  //------------------------------------------------------

  SSD1306View::SSD1306View()
  : _renderer(NULL),
    _distanceConverter(0, 1000, 10)
  {
  }


  SSD1306View::SSD1306View(Renderer *renderer)
  : SSD1306View()
  {
    _renderer = renderer;
  }

  SSD1306View::~SSD1306View()
  {
  }

  void SSD1306View::Init(Renderer* renderer, uint32_t width, uint32_t height)
  {
    _renderer = renderer;
    _label.Init(_renderer, width, height );
    _level.Init(_renderer, width, height );

    if(_renderer != NULL)
    {
      _stateControl.SetState(State::ShowSplash);
      Process();
    }
  }

  void SSD1306View::Show( const ViewData& viewData)
  {
    _data = viewData;
    Process();        
  }

  void SSD1306View::Step()
  {
    Process();
  }

  void SSD1306View::Process()
  {
    if (_renderer)
    {
      switch(_stateControl.GetState())
      {
        case State::ShowSplash:
        {
          xControl::Image splash = xControl::Splash();
          _renderer->drawBitmap(0, 0, splash.Data(), splash.Width(), splash.Height(), 1);
          _renderer->Updateframe();
          _stateControl.SetState(State::Delay, 2000);
          break;
        }
        case State::Delay:
        {
          if(_stateControl.DelayExpired())
              _stateControl.SetState(State::ShowLevel);
          
          break;
        }
        case State::ShowLevel:
        {
          if(_stateControl.OnEnter())
            _stateControl.StartDelay(10000);

          _level.Set(_distanceConverter.ToPercent(_data.Distance()));
          _level.Show();

          if(_stateControl.DelayExpired())
            _stateControl.SetState(State::ShowTemp);

          break;
        }
        case State::ShowTemp:
        {
          if(_stateControl.OnEnter())
            _stateControl.StartDelay(2000);
          
          String temperature(_data.Temperature());
          temperature += (char)247;
          temperature += "C";

          _icon = xControl::Temperature();
          _label.SetIcon(_icon);
          _label.Text(temperature.c_str());

          if(_stateControl.DelayExpired())
            _stateControl.SetState(State::ShowHumidity);

          break;
        }
        case State::ShowHumidity:
        {
          if(_stateControl.OnEnter())
            _stateControl.StartDelay(2000);

          String humidity(_data.Humidity());
          humidity += "%";

          _icon = xControl::Humidity();
          _label.SetIcon(_icon);
          _label.Text(humidity.c_str());

          if(_stateControl.DelayExpired())
            _stateControl.SetState(State::ShowLevel);
            
          break;
        }
        case State::Unknown:
          break;
        default:
          break;
      }
    }
  }
} // namespace View