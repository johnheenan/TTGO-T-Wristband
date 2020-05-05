// Edits and additions by John Heenan
#pragma once
#include <Arduino.h>
#include <Preferences.h> // now recommended to use NVS with Preferences library rather than EEPROM library
#include <NTP.h>         // for enums

#define MAGBIAS_FLOATS_MAX 3
void storeMagBiasEEPROM(float *magbias);
void getMagBiasEEPROM(float *magbias);

// Following added by John Heenan, 2020

#define SETTINGS_STRLEN_MAX 10

typedef struct
{
  float magbias[3];
  bool imu_skip;
  bool dst_none;

  bool tz_uses_dst;
  int tz_offset;
  week_t tz_week;
  dow_t tz_wday;
  char tz_dst_name[SETTINGS_STRLEN_MAX + 1];
  month_t tz_dst_month;
  int8_t tz_dst_hour;
  int tz_dst_offset;
  char tz_std_name[SETTINGS_STRLEN_MAX + 1];
  month_t tz_std_month;
  int8_t tz_std_hour;
  int tz_std_offset;

} settings_t;

extern settings_t settings;
size_t storeSettings();
size_t loadSettings();
