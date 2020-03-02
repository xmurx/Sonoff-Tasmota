#include "gmock/gmock.h"
#include "view.h"
#include "mock_renderer.h"
#include "mock_StateControl.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;

namespace xControl
{
  class SSD1306ViewFixture : public ::testing::Test
  {
  public:
    SSD1306ViewFixture()
    {
      _renderer.reset(new testing::NiceMock<MockRenderer>());
      _stateControl.reset( new testing::NiceMock<MockStateControl<xControl::State>>());
    }

    virtual ~SSD1306ViewFixture()
    {
      _renderer.reset();
      _stateControl.reset();
    }

    static std::unique_ptr<MockRenderer> _renderer;
    static std::unique_ptr<MockStateControl<xControl::State>> _stateControl;
  };

  std::unique_ptr<MockRenderer> SSD1306ViewFixture::_renderer;
  std::unique_ptr<MockStateControl<State>> SSD1306ViewFixture::_stateControl;

  TEST_F(SSD1306ViewFixture, CheckStateHandling)
  { 
    xControl::SSD1306View view{};
    view.Init(SSD1306ViewFixture::_renderer.get(), SSD1306ViewFixture::_stateControl.get(), 132, 28);
    EXPECT_TRUE(true);
  }
} // end of namespace