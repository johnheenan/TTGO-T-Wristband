// Edits and additions by John Heenan are as is and with no warranty
#pragma once
#include <Arduino.h>
#include <Preferences.h> // now recommended to use NVS with Preferences library rather than EEPROM library
#include <NTP.h>         // for enums
#include "timezones.hpp"

#define MAGBIAS_FLOATS_MAX 3
void storeMagBiasEEPROM(float *magbias);
void getMagBiasEEPROM(float *magbias);

// Additions by John Heenan, 2020 are as is ans with no warranty

typedef struct
{
  int eeprom_ver;
  float magbias[3];
  bool imu_skip;     // if true them IMU is set to deep sleep and not to cause esp32 to wake up
  bool tz_uses_dst;  // if user last choose a dst name then this is true, if user last choose a UTC offset then this is false
  int tz_offset;  // only used when tz_uses_dst is false, however it is remembered
  char dst_region[DST_REGION_STRLEN_MAX + 1]; //only used when tz_uses_dst is true, however it is remembered 

} settings_t;

extern settings_t settings;
size_t storeSettings();
size_t loadSettings();
char *strcpy_settings(char *to, const char *from, size_t strlen_max);

