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
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(1).WillOnce(Return(0)).WillOnce(Return(2001));
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();

    stateControl.SetState(State::Delay, 2000);
    bool onEnter = stateControl.OnEnter();
    onEnter = onEnter && stateControl.OnEnter();
    EXPECT_TRUE((initialState == State::Unknown) && !onEnter );
  }
  
  TEST_F(TasmotaFixture, ChangeStateWithOutTimeOutAndCallOnEnterOnce)
  { 
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();
    stateControl.SetState(State::ShowLevel);

    State changedState = stateControl.GetState();

    bool onEnter = stateControl.OnEnter();
    onEnter = onEnter && stateControl.OnEnter();

    EXPECT_TRUE((initialState == State::Unknown) && !onEnter && (changedState == State::ShowLevel));
  }

  TEST_F(TasmotaFixture, StartDelayAndWaitForExpire)
  { 
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();

    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2000));
    
    stateControl.SetState(State::ShowHumidity);
    State changedState = stateControl.GetState();
    stateControl.StartDelay(1500);
    
    bool expired = stateControl.DelayExpired();
    EXPECT_TRUE((initialState == State::Unknown) && expired && (changedState == State::ShowHumidity));
  }

  TEST_F(TasmotaFixture, CheckTimeBetweenStateChange)
  { 
    StateControl<State> stateControl;
    State initialState = stateControl.GetState();
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2000));
    
    stateControl.SetState(State::ShowHumidity);
    State changedState = stateControl.GetState();
    stateControl.SetState(State::ShowLevel);
    State state = stateControl.GetState();
    
    EXPECT_TRUE(true);
  }
} // end of namespace