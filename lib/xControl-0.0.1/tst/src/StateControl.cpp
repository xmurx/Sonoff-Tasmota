#include "gmock/gmock.h"
#include "mocks/tasmota.h"
#include "StateControl.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Eq;

namespace xControl
{
  enum TestStates
  {
    State_0,
    State_1,
    State_2,
    State_3,
    State_4
  };

  template<>
  struct StateTrait<TestStates> { static const TestStates initialValue = TestStates::State_0;
  };

  TEST_F(TasmotaFixture, ChangeStateAfterTimeOut)
  { 
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2001));
    StateControl<TestStates> stateControl;
    TestStates initialState = stateControl.GetState();
    stateControl.SetState(TestStates::State_1);
    stateControl.StartDelay(2000);
    bool delayExpired = stateControl.DelayExpired();
    EXPECT_TRUE((initialState == TestStates::State_0) && delayExpired );
  }

  TEST_F(TasmotaFixture, ChangeStateAfterTimeOutAndCallOnEnterOnce)
  { 
    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(1).WillOnce(Return(0)).WillOnce(Return(2001));
    StateControl<TestStates> stateControl;
    TestStates initialState = stateControl.GetState();

    stateControl.SetState(TestStates::State_1);
    stateControl.StartDelay(2000);
    bool onEnter = stateControl.OnEnter();
    onEnter = onEnter && stateControl.OnEnter();
    EXPECT_TRUE((initialState == TestStates::State_0) && !onEnter );
  }
  
  TEST_F(TasmotaFixture, ChangeStateWithOutTimeOutAndCallOnEnterOnce)
  { 
    StateControl<TestStates> stateControl;
    TestStates initialState = stateControl.GetState();
    stateControl.SetState(TestStates::State_1);

    TestStates changedState = stateControl.GetState();

    bool onEnter = stateControl.OnEnter();
    onEnter = onEnter && stateControl.OnEnter();

    EXPECT_TRUE((initialState == TestStates::State_0) && !onEnter && (changedState == TestStates::State_1));
  }

  TEST_F(TasmotaFixture, StartDelayAndWaitForExpire)
  { 
    StateControl<TestStates> stateControl;
    TestStates initialState = stateControl.GetState();

    EXPECT_CALL(*TasmotaFixture::_time, Millis()).Times(2).WillOnce(Return(0)).WillOnce(Return(2000));
    
    stateControl.SetState(TestStates::State_3);
    TestStates changedState = stateControl.GetState();
    stateControl.StartDelay(1500);
    
    bool expired = stateControl.DelayExpired();
    EXPECT_TRUE((initialState == TestStates::State_0) && expired && (changedState == TestStates::State_3));
  }

  TEST_F(TasmotaFixture, CheckTimeBetweenStateChange)
  { 
    StateControl<TestStates> stateControl;
    TestStates initialState = stateControl.GetState();
    
    stateControl.SetState(TestStates::State_2);
    TestStates state = stateControl.GetState();
    EXPECT_TRUE(initialState == TestStates::State_0 && state == TestStates::State_2);
  }
} // end of namespace