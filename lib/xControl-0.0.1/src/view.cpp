#include "view.h"
#include "WString.h"
#include "logging.h"
#include "renderer.h"

unsigned long millis();

extern void ExecuteCommand(const char* cmnd, uint32_t source);

namespace xControl
{
  //------------------------------------------------------
  // PercentConverter
  //------------------------------------------------------

  PercentConverter::PercentConverter(uint32_t minValue, uint32_t maxValue, uint32_t step) :
  _minValue(minValue),
  _maxValue(maxValue),
  _step(step)
  {
  }

  PercentConverter::~PercentConverter()
  {
  }

  uint32_t PercentConverter::ToPercent(uint32_t value)
  {
    uint32_t percent = (value * 100) / (_maxValue - _minValue);
    return percent > 100 ? 100 : percent;
  }

  //------------------------------------------------------
  // SSD1306View
  //------------------------------------------------------

  SSD1306View::SSD1306View() :
  _renderer(NULL),
  _stateControl(NULL),
  _distanceConverter(0, 1000, 10)
  {
  }

  SSD1306View::SSD1306View(Renderer* renderer, StateControlBase<State>* stateControl) :
  SSD1306View()
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
    _label.Init(_renderer, width, height);
    _level.Init(_renderer, width, height);

    if (_renderer != NULL)
    {
      _stateControl->SetState(State::ShowSplash);
      Process();
    }
  }

  void SSD1306View::Show(const ViewData& viewData)
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
      switch (_stateControl->GetState())
      {
        case State::ShowSplash:
        {
          if (_stateControl->OnEnter())
          {
            xControl::Image splash = xControl::Splash();
            _renderer->drawBitmap(0, 0, splash.Data(), splash.Width(), splash.Height(), 1);
            _renderer->Updateframe();
            _stateControl->StartDelay(2000);

            ::ExecuteCommand("TmState 1", 0); //enable telegram sending
          }

          if (_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowTemp);

          break;
        }
        case State::ShowLevel:
        {
          if (_stateControl->OnEnter())
            _stateControl->StartDelay(10000);

          uint32_t levelInPercent = _distanceConverter.ToPercent(_data.Distance());
          _level.Set(levelInPercent);
          _level.Show();

          if (_stateControl->DelayExpired())
          {
            if (levelInPercent < 20)
            {
              String cmd = "TmSend Remaining tank level is only: " + String(levelInPercent) + String("%.");
              Logging::Debug("Telegram command: %s", cmd.c_str());
              ::ExecuteCommand(cmd.c_str(), 0);
            }
            _stateControl->SetState(State::ShowTemp);
          }

          break;
        }
        case State::ShowTemp:
        {
          if (_stateControl->OnEnter())
            _stateControl->StartDelay(2000);

          String temperature(_data.Temperature());
          temperature += (char)248;
          temperature += "C";

          _icon = xControl::Temperature();
          _label.SetIcon(_icon);
          _label.Text(temperature.c_str());

          if (_stateControl->DelayExpired())
            _stateControl->SetState(State::ShowHumidity);

          break;
        }
        case State::ShowHumidity:
        {
          if (_stateControl->OnEnter())
            _stateControl->StartDelay(2000);

          String humidity(_data.Humidity());
          humidity += "%";

          _icon = xControl::Humidity();
          _label.SetIcon(_icon);
          _label.Text(humidity.c_str());

          if (_stateControl->DelayExpired())
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
} // namespace xControl