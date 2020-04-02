#include "gmock/gmock.h"
#include "view.h"
#include "mock_renderer.h"
#include "mock_StateControl.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::AnyNumber;

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

    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, GetState()).Times(5).WillOnce(Return(State::ShowSplash)).WillOnce(Return(State::ShowTemp)).WillOnce(Return(State::ShowHumidity)).WillOnce(Return(State::ShowLevel)).WillOnce(Return(State::ShowTemp));

    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, OnEnter()).Times(AnyNumber()).WillRepeatedly(Return(true));
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, DelayExpired()).Times(AnyNumber()).WillRepeatedly(Return(true));

    Sequence s;
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, SetState(State::ShowTemp)).InSequence(s);
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, SetState(State::ShowHumidity)).InSequence(s); 
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, SetState(State::ShowLevel)).InSequence(s); 
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, SetState(State::ShowTemp)).InSequence(s); 
    EXPECT_CALL(*SSD1306ViewFixture::_stateControl, SetState(State::ShowHumidity)).InSequence(s); 

    view.Step();
    view.Step();
    view.Step();
    view.Step();
    view.Step();
  }
} // end of namespace