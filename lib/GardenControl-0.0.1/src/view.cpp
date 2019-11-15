#include "view.h"
#include "images.h"
#include "renderer.h"

namespace View
{

    SSD1306View::StateControl::StateControl()
    : _state(Unknown),
      _startTime(0),
      _delayTime(0)
    {
    }

    void SSD1306View::StateControl::SetState(State state) { _state = state; }
    SSD1306View::StateControl::State SSD1306View::StateControl::GetState() { return _state; }

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
    // SSD1306View
    //------------------------------------------------------

    SSD1306View::SSD1306View(Renderer *renderer) : _renderer(renderer) { }
    SSD1306View::~SSD1306View() { }

    void SSD1306View::Execute( Command command, const ViewData& viewData)
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

                    _stateControl.StartDelay(1000);
                    _stateControl.SetState(StateControl::Delay);
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
                    _renderer->setCursor(12,10);
                    _renderer->printf( "%d mm", viewData.Distance());
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