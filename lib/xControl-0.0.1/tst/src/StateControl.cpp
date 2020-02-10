#include "gmock/gmock.h"

#include "view.h"
#include "mocks/tasmota.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;

namespace xControl
{
  TEST_F(TasmotaFixture, ChangeStateAfterTimeOut)
  { 
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2001));
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();
    stateControl.SetState(State::Delay, 2000);
    bool delayExpired = stateControl.DelayExpired();
    EXPECT_TRUE((initialState == State::Unknown) && delayExpired );
  }

  TEST_F(TasmotaFixture, ChangeStateAfterTimeOutAndCallOnEnterOnce)
  { 
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2001));
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();

    stateControl.SetState(State::Delay, 2000);
    bool onEnter = stateControl.OnEnter();
    onEnter = onEnter && stateControl.OnEnter();
    EXPECT_TRUE((initialState == State::Unknown) && !onEnter );
  }
} // end of namespace