#include "view.h"
#include "images.h"
#include "renderer.h"
#include "logging.h"

namespace xControl
{
    SSD1306View::StateControl::StateControl()
    : _state(Unknown),
      _startTime(0),
      _delayTime(0)
    { 
    }

    void SSD1306View::StateControl::SetState(State state, uint32_t delay )
    { 
        if(state == Delay)
        {
            _delayTime = delay;
            _startTime = millis();
        }
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

    SSD1306View::SSD1306View(Renderer *renderer)
    : _renderer(renderer),
      _distanceConverter(0, 1000, 10),
      _width(0),
      _height(0)
    {
    }

    SSD1306View::SSD1306View()
    : _renderer(NULL),
      _distanceConverter(0, 1000, 10),
      _width(0),
      _height(0)
    {
    }

    SSD1306View::~SSD1306View()
    {
    }

    void SSD1306View::Init(Renderer* renderer, uint32_t width, uint32_t height)
    {
        _renderer = renderer;
        _width = width;
        _height = height;

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
                        _stateControl.SetState(StateControl::ShowData);
                    
                    break;
                }
                case StateControl::ShowData:
                {
                    static uint32_t lastValue = _data.Distance();
                    uint32_t currentValue = _data.Distance();
                    if(lastValue != currentValue)                                      
                    {
                      lastValue = currentValue;
                      _renderer->clearDisplay();
                      _renderer->drawRect(14,0,_width-28,_height,1);
                      _renderer->fillRect(18,3, (100-_distanceConverter.ToPercent(currentValue))-6, 26, 1);
                      _renderer->Updateframe();
                    }
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