#include <Arduino.h>
#include <EasyButton.h>
#include "pages.hpp"

int8_t page = 0;
EasyButton tp_button(TP_PIN_PIN, 80, true, false);
uint32_t time_out = millis();
uint16_t max_time_out = 15000;
bool handlingAction = false;
bool initialLoad = true;

void initButton()
{
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button.begin();
  initButtonHandlers();
  page = 0;
  showPage();
}

void handleUi()
{
  if (millis() - time_out > max_time_out && !handlingAction)
  {

#ifdef DEEP_SLEEP_ALWAYS
    handleSleep();
#else
  #ifndef DEEP_SLEEP_NEVER
    #ifdef DEEP_SLEEP_PLUGGED_IN_NEVER
      if (!isPluggedIn())
        handleSleep();
    #else
      handleSleep();
    #endif // DEEP_SLEEP_PLUGGED_IN_NEVER
  #endif // DEEP_SLEEP_NEVER
#endif // DEEP_SLEEP_ALWAYS


  }
  tp_button.read();
  if (!handlingAction)
    showPage();
}
/* todo jh if plugged in and not handlingAction then store state in settings, can leave screen on and go into some sleep state
as there is no point in remaining on at full belt. Can use tp pin or rtc alarm int to wake every minute to update clock (if 
using clock), to check if still charging and so should turn charging led off. Still need to use rtc to wake up in case unplugged,
battery discharges and gets plugged in again. */

void increasePage()
{
  time_out = millis();
  page++;
  if (page == (MAX_PAGES))
  {
#ifdef DEEP_SLEEP_ALWAYS
    handleSleep();
#else
  #ifndef DEEP_SLEEP_NEVER
    if (!isPluggedIn())
      handleSleep();
    else
      page = 0;
  #else
      page = 0;
  #endif // DEEP_SLEEP_NEVER
#endif // DEEP_SLEEP_ALWAYS
  }
  initialLoad = true;
}

void showPage()
{
  switch (page)
  {
  case 0:
    max_time_out = 8000;
    pageClock(initialLoad);
    break;
  case 1:
    max_time_out = 15000;
    pageRtc(initialLoad);
    break;
  case 2:
    max_time_out = 15000;
    pageBattery(initialLoad);
    break;
  case 3:
    if (!settings.imu_skip)
    {
      max_time_out = 60000;
      pageBearing(initialLoad);
      break;
    }
    else
      page++;
  case 4:
    if (!settings.imu_skip)
    {
      max_time_out = 30000;
      pageTemperature(initialLoad);
      break;
    }
    else
      page++;
  case 5:
    max_time_out = 15000;
    pageOta(initialLoad);
    break;
  }
  initialLoad = false;
}

void handleAction()
{
  handlingAction = true;
  time_out = millis();
  switch (page)
  {
  case 0:
    actionClock();
    break;
  case 1:
    actionTZ();
    break;
  case 2:
    actionIMU();
    break;
  case 3:
    actionBearing();
    break;
  case 5:
    waitOta();
    page = 0;
    break;
  }
  time_out = millis();
  handlingAction = false;
  initialLoad = true;
}

void initButtonHandlers()
{
  tp_button.onPressedFor(1000, handleAction);
  tp_button.onPressed(increasePage);
}
