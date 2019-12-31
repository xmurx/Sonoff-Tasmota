#include "view.h"
#include "images.h"
#include "renderer.h"
#include "logging.h"

namespace xControl
{
    SSD1306View::StateControl::StateControl()
    : _state(Unknown),
      _startTime(0),
      _delayTime(0),
      _onEnter(false)
    { 
    }

    void SSD1306View::StateControl::SetState(State state, uint32_t delay )
    { 
        if(state == Delay)
        {
          _delayTime = delay;
          _startTime = millis();
        }

        if(_state != state)
          _onEnter = true;

        _state = state;
    }

    SSD1306View::StateControl::State SSD1306View::StateControl::GetState()
    {
        return _state;
    }

    void SSD1306View::StateControl::StartDelay(uint32_t delayIn_ms)
    {
        _delayTime = delayIn_ms;
        _startTime = millis();
    }

    void SSD1306View::StateControl::ResetDelay()
    { 
        _startTime = 0;
        _delayTime = 0;
    }

    bool SSD1306View::StateControl::DelayExpired()
    {
        bool ret = ((millis() - _startTime) >= _delayTime);
        if(ret)
        {
          ResetDelay();
        }
        return ret;
    }

    bool SSD1306View::StateControl::OnEnter()
    {
      if(_onEnter == true)
      {
        _onEnter = false;
        return true;
      }
      return _onEnter;
    }

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

        if(_renderer != NULL)
        {
          _stateControl.SetState(StateControl::ShowSplash);
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
                case StateControl::ShowSplash:
                {
                    View::Image splash = View::Splash();
                    _renderer->drawBitmap(0, 0, splash.Data(), splash.Width(), splash.Height(), 1);
                    _renderer->Updateframe();
                    _stateControl.SetState(StateControl::Delay, 2000);
                    break;
                }
                case StateControl::Delay:
                {
                    if(_stateControl.DelayExpired())
                        _stateControl.SetState(StateControl::ShowLevel);
                    
                    break;
                }
                case StateControl::ShowLevel:
                {
                  if(_stateControl.OnEnter())
                    _stateControl.StartDelay(10000);

                  static uint32_t lastValue = _data.Distance();
                  uint32_t currentValue = _data.Distance();
                  if(lastValue != currentValue)                                      
                  {
                    enum{width=128, height=32};
                    lastValue = currentValue;
                    _renderer->clearDisplay();
                    _renderer->drawChar(0,3,'E',1,0,2);
                    _renderer->drawRect(12,0,width-24,height-12,1);
                    _renderer->drawChar(118,3,'F',1,0,2);
                    _renderer->fillRect(15,3, (100-_distanceConverter.ToPercent(currentValue)), 26-12, 1);
                    
                    _renderer->setTextSize(1);
                    _renderer->setCursor(28, 23);
                    _renderer->print("Abwassertank");
                    _renderer->Updateframe();
                  }

                  if(_stateControl.DelayExpired())
                    _stateControl.SetState(StateControl::ShowTemp);

                  break;
                }
                case StateControl::ShowTemp:
                {
                  if(_stateControl.OnEnter())
                    _stateControl.StartDelay(2000);
                  
                  String temperature(_data.Temperature());
                  temperature += (char)247;
                  temperature += "C";

                  _label.Text(temperature.c_str());

                  if(_stateControl.DelayExpired())
                    _stateControl.SetState(StateControl::ShowLevel);

                  break;
                }
                case StateControl::Unknown:
                    break;
                default:
                    break;
            }
        }
    }
} // namespace View