/*
  xdrv_98_deepsleep.ino - deep sleep support for Sonoff-Tasmota

  Copyright (C) 2019  Fazzyer

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define USE_DEEP_SLEEP

#ifdef USE_DEEP_SLEEP

#define XDRV_98             98


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

uint32_t GetRemainingTime()
{
  uint8_t buffer[4];
  ESP.rtcUserMemoryRead(0, (uint32_t *)&buffer, sizeof(buffer));

  return buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0];
}

void SetRemainingTime(uint32_t time)
{
  uint8_t buffer[4];
  buffer[0] = time;
  buffer[1] = time >> 8;
  buffer[2] = time >> 16;
  buffer[3] = time >> 24;

  ESP.rtcUserMemoryWrite(0, (uint32_t *)&buffer, sizeof(buffer));
}


uint8_t _loopCount = 0;


void EnterDeepSleep(uint32_t newRemainingTime)
{
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
  SetRemainingTime(newRemainingTime);
  ESP.deepSleep(1e6);
  delay(100);
}


bool Xdrv98(uint8_t function)
{
  uint32_t remainingTime = GetRemainingTime();

  switch (function)
  {
  case FUNC_SETTINGS_OVERRIDE:
    if (remainingTime > 0)
    {
      EnterDeepSleep(remainingTime > 5 ? 5 : (remainingTime - 1));
    }
    break;
  case FUNC_LOOP:
    Serial.println("I have outslept :)");
    _loopCount++;
    if (_loopCount == 2)
    {
      Serial.println("Sleeping for 5 secs...");
      EnterDeepSleep(5);
    }
    break;
  }

  return true;
}



#endif // USE_DEEP_SLEEP_DRIVER