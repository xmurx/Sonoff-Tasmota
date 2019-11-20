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
      _distanceConverter(0, 1000, 10)
    {
    }

    SSD1306View::SSD1306View()
    : _renderer(NULL),
      _distanceConverter(0, 1000, 10)
    {
    }

    SSD1306View::~SSD1306View()
    {
    }

    void SSD1306View::Init(Renderer* renderer)
    {
        _renderer = renderer;
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
                    _renderer->setTextFont(0);
                    _renderer->setTextSize(2);
                    _renderer->clearDisplay();
                    _renderer->setCursor(12,10);
                    _renderer->printf( "%d%%", (100-_distanceConverter.ToPercent(_data.Distance())));
                    _renderer->Updateframe();
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