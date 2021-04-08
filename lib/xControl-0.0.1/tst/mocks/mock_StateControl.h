#ifndef __MOCK_STATECONTROL_H__
#define __MOCK_STATECONTROL_H__

#include "StateControl.h"
#include <gmock/gmock.h>

template <typename T>
class MockStateControl : public xControl::StateControlBase<T>
{
public:
  MockStateControl(/* args */) {}
  virtual ~MockStateControl() {}

  MOCK_METHOD(T, GetState, (), (override));
  MOCK_METHOD(void, SetState, (T state), (override));
  MOCK_METHOD(void, StartDelay, (uint32_t delay), (override));
  MOCK_METHOD(bool, DelayExpired, (), (override));
  MOCK_METHOD(void, ResetDelay, (), (override));
  MOCK_METHOD(bool, OnEnter, (), (override));
};

#endif