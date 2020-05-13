// Created by John Heenan, 2020, as is and with no warranty. Please retain line
#include "pages.hpp"
#include <EasyButton.h>
#include "timezones.hpp"
#include "eeprom.hpp" //for strcpy_settings

#define OFFSET_INCREMENT 30

enum menu_state_t
{
  MENU_STAY,
  MENU_OFFER_CHANGE,
};

bool btnPressed;
void handleBtnPressed()
{
  btnPressed = true;
}
bool btnHeld;
void handleBtnHeld()
{
  btnHeld = true;
}

void actionTZ()
{
  unsigned long oldmilis = millis();
  char buf1[40];
  bool tz_uses_dst = settings.tz_uses_dst;
  menu_state_t menu_tz_uses_dst_true = MENU_STAY;
  menu_state_t menu_tz_uses_dst_false = MENU_STAY;
  int tz_offset = settings.tz_offset;
  int tz_offset_hours = tz_offset / 60;
  int tz_offset_minutes = tz_offset % 60;
  int dst_index_menu = dst_index;
  bool wasPressed = false;
  btnHeld = btnPressed = false;
  tp_button.onPressed(handleBtnPressed);
  tp_button.onPressedFor(1000, handleBtnHeld);

  if (tz_uses_dst)
    sprintf(buf1, "%s", dst_array[dst_index_menu].region);
  else
  {
    if (tz_offset_minutes < 0)
      tz_offset_minutes = -tz_offset_minutes;
    sprintf(buf1, "+/-UTC %02d:%02d", tz_offset_hours, tz_offset_minutes);
  }
  msgBig(buf1);
  while (millis() - oldmilis < 5000)
  {
    tp_button.read();
    if (btnHeld)
      break;
    if (btnPressed)
    {
      btnPressed = false;
      wasPressed = true;
      oldmilis = millis();
      if (tz_uses_dst)
      {
        int dst_index_test = dst_index_menu;
        dst_index_test++;
        if (dst_index_test == dst_length)
        {
          dst_index_test = 0;
          if (menu_tz_uses_dst_true == MENU_STAY)
          {
            menu_tz_uses_dst_true = MENU_OFFER_CHANGE;
            sprintf(buf1, "UTC Offsets");
          }
          else
            menu_tz_uses_dst_true = MENU_STAY;
        }
        if (menu_tz_uses_dst_true == MENU_STAY)
        {
          dst_index_menu = dst_index_test;
          sprintf(buf1, "%s", dst_array[dst_index_menu].region);
        }
      }
      else
      {
        int tz_offset_test = tz_offset;
        tz_offset_test += OFFSET_INCREMENT;
        if (tz_offset_test > 60 * 12)
          tz_offset_test = -60 * 12;
        if (tz_offset_test == 0)
        {
          if (menu_tz_uses_dst_false == MENU_STAY)
          {
            menu_tz_uses_dst_false = MENU_OFFER_CHANGE;
            sprintf(buf1, "DST Regions");
          }
          else
            menu_tz_uses_dst_false = MENU_STAY;
        }
        if (menu_tz_uses_dst_false == MENU_STAY)
        {
          tz_offset = tz_offset_test;
          tz_offset_hours = tz_offset / 60;
          tz_offset_minutes = tz_offset % 60;
          if (tz_offset_minutes < 0)
            tz_offset_minutes = -tz_offset_minutes;
          sprintf(buf1, "+/-UTC %02d:%02d", tz_offset_hours, tz_offset_minutes);
        }
      }
      msgBig(buf1);
    }
  }
  if (!btnHeld)
  {
    msgInfo("No settings changed");
    sleep(2);
    initButtonHandlers();
    return;
  }

  bool isSettingsChanged = false;
  if (wasPressed)
  {
    if (tz_uses_dst && menu_tz_uses_dst_true == MENU_OFFER_CHANGE)
      tz_uses_dst = false;
    if (!tz_uses_dst && menu_tz_uses_dst_false == MENU_OFFER_CHANGE)
      tz_uses_dst = true;
    if (tz_uses_dst != settings.tz_uses_dst)
      isSettingsChanged = true;
    else
    {
      if (tz_uses_dst && strcmp(dst_array[dst_index_menu].region, settings.dst_region))
        isSettingsChanged = true;
      else if (!tz_uses_dst && tz_offset != settings.tz_offset)
        isSettingsChanged = true;
    }
  }

  if (isSettingsChanged)
  {
    time_t utcNow;
    getUTCTime(&utcNow);
    settings.tz_uses_dst = tz_uses_dst;
    if (tz_uses_dst)
    {
      dst_index = dst_index_menu;
      strcpy_settings(settings.dst_region, dst_array[dst_index].region, DST_REGION_STRLEN_MAX);
    }
    else
      settings.tz_offset = tz_offset;
    storeSettings();
    setNtpUtcDst(utcNow);
  }
  else
  {
    msgInfo("No settings changed");
    sleep(2);
  }
  initButtonHandlers();
}
