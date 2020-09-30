#include "view.h"
#include "logging.h"
#include "renderer.h"
#include "WString.h"

#if 0
#include "AsyncTelegram.h"
static AsyncTelegram _bot_;
const char* token = "1051397850:AAGZUAx4Dby6QdZriTyLwU73fo05dE0xdhQ";
#endif

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
    _stateControl(NULL),
    _distanceConverter(0, 1000, 10)
  {
  }


  SSD1306View::SSD1306View(Renderer *renderer, StateControlBase<State>* stateControl)
  : SSD1306View()
  {
    _renderer = renderer;
    _stateControl = stateControl;
  }

  SSD1306View::~SSD1306View()
  {
  }

  void SSD1306View::Init(Renderer* renderer, StateControlBase<State>* stateControl, uint32_t width, uint32_t height)
  {
    _renderer = renderer;
    _stateControl = stateControl;
    _label.Init(_renderer, width, height );
    _level.Init(_renderer, width, height );

    if(_renderer != NULL)
    {
      _stateControl->SetState(State::ShowSplash);
      Process();
    }
#if 0
    _bot_.setUpdateTime(2000);
    _bot_.setTelegramToken(token);
#endif    

  }

  void SSD1306View::Show( const ViewData& viewData)
  {
    _data = viewData;
#if 0    
    Logging::Debug("\nTest Telegram connection...");
    bool ret = _bot_.begin();
    ret ? Logging::Debug("OK") : Logging::Debug("NOK");
    Logging::Debug("Bot name: @%s", _bot_.userName.c_str());	
    Logging::Debug("Wifi.status() - status: %d",WiFi.status());
#endif    
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
      switch(_stateControl->GetState())
      {
        case State::ShowSplash:
        {
          if(_stateControl->OnEnter())
          {
            xControl::Image splash = xControl::Splash();
            _renderer->drawBitmap(0, 0, splash.Data(), splash.Width(), splash.Height(), 1);
            _renderer->Updateframe();
            _stateControl->StartDelay(2000);

          }

          if(_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowTemp);
            
          break;
        }
        case State::ShowLevel:
        {
          if(_stateControl->OnEnter())
            _stateControl->StartDelay(10000);

          _level.Set(_distanceConverter.ToPercent(_data.Distance()));
          _level.Show();

          if(_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowTemp);

          break;
        }
        case State::ShowTemp:
        {
          if(_stateControl->OnEnter())
            _stateControl->StartDelay(2000);
          
          String temperature(_data.Temperature());
          temperature += (char)248;
          temperature += "C";

          _icon = xControl::Temperature();
          _label.SetIcon(_icon);
          _label.Text(temperature.c_str());

          if(_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowHumidity);

          break;
        }
        case State::ShowHumidity:
        {
          if(_stateControl->OnEnter())
            _stateControl->StartDelay(2000);

          String humidity(_data.Humidity());
          humidity += "%";

          _icon = xControl::Humidity();
          _label.SetIcon(_icon);
          _label.Text(humidity.c_str());

          if(_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowLevel);
            
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