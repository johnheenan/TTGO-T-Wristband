
// Edits and additions by John Heenan, 2020, are as is with no warranty. Please retain line.
#include "eeprom.hpp"
#include "timezones.hpp"
#include "hal.hpp" //for DEBUG_MSG

Preferences prefs;
settings_t settings;

void storeMagBiasEEPROM(float *magbias)
{
  settings.magbias[0] = magbias[0];
  settings.magbias[1] = magbias[1];
  settings.magbias[2] = magbias[2];
  storeSettings();
}

void getMagBiasEEPROM(float *magbias)
{
  magbias[0] = settings.magbias[0];
  magbias[1] = settings.magbias[1];
  magbias[2] = settings.magbias[2];
}

size_t storeSettings()
{
  return prefs.putBytes("settings", &settings, sizeof(settings));
}

char *strcpy_settings(char *to, const char *from, size_t strlen_max)
{
  size_t len = strlen(from);
  bool shortened = false;
  if (len > strlen_max)
  {
    len = strlen_max;
    shortened = true;
  }
  memcpy(to, from, len);
  to[len] = 0;
  if (!shortened)
    return to + len + 1;
  else
    return to + len;
}

size_t loadSettings() //must be called first before any other functions on this page
{
  prefs.begin("settings");
  size_t settingsLen = 0;
  settingsLen = prefs.getBytesLength("settings");
  if (settingsLen)
  {
    size_t len = prefs.getBytes("settings", &settings, settingsLen);
    if (len && (settings.eeprom_ver != EEPROM_VER || len != sizeof(settings_t)))
      settingsLen = 0;
  }
  if (settingsLen)
      DEBUG_MSG("Using NVS (as EEPROM) settings instead of FLASH settings. Change EEPROM_VER to force reinitialisation \n");
  else
  {
    DEBUG_MSG("Reinitialising NVS (as EEPROM) settings from FLASH settings\n");

    settings.eeprom_ver = EEPROM_VER;
    settings.magbias[0] = 0.0;
    settings.magbias[1] = 0.0;
    settings.magbias[2] = 0.0;
#ifdef IMU_SKIP
    settings.imu_skip = true;
#else
    settings.imu_skip = false;
#endif
#ifdef TZ_USES_DST
    settings.tz_uses_dst = true;
#else
    settings.tz_uses_dst = false;
#endif
#ifdef TZ_OFFSET
    settings.tz_offset = (TZ_OFFSET/30)*30;
#else
    settings.tz_offset = 480;
#endif
#ifdef DST_REGION
    strcpy_settings(settings.dst_region, DST_REGION, DST_REGION_STRLEN_MAX);
#else
    strcpy_settings(settings.dst_region, dst_array[0].region, DST_REGION_STRLEN_MAX);
#endif
  }

  bool dst_found = false;
  int i = 0;
  while (i < DST_MAX)
  {
    if (!strcmp("NULL", dst_array[i].region))
      break;
    if (!dst_found && !strcmp(settings.dst_region, dst_array[i].region))
    {
      dst_found = true;
      dst_index = i;
    }
    i++;
  }
  dst_length = i;
  if (!dst_found)
  {
    dst_index = 0;
    strcpy_settings(settings.dst_region, dst_array[dst_index].region, DST_REGION_STRLEN_MAX); // copy over again
  }
  if (!settingsLen)
    settingsLen = storeSettings();
  return settingsLen;
}
